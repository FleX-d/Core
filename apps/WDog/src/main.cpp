/*
Copyright (c) 2018, Globallogic s.r.o.
All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
 * Neither the name of the Globallogic s.r.o. nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL GLOBALLOGIC S.R.O. BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * File:   main.cpp
 * Author: Branislav Podkonicky
 *
 * Created on July 29, 2018, 09:45 AM
 */

#include "IPCInterface.h"
#include "INIParser.h"
#include "FleXdWatchdog.h"
#include "FleXdWatchdogEvent.h"
#include <FleXdEpoll.h>
#include <FleXdEvent.h>
#include <FleXdTimer.h>
#include <FleXdLogger.h>
#include <FleXdIPCConnector.h>
#include <FleXdIPCMsgTypes.h>
#include <sys/inotify.h>
#include <sys/stat.h>
#include <dirent.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <thread>
#include <unistd.h>

using namespace flexd::icl::ipc;

static std::vector<std::unique_ptr<FleXdWatchdog>> watchdogs;
static std::vector<std::string> watchedDirectories;
static const std::string pathToConfigFile{"../resources/config.ini"};
static int watchdogIpcId(100);
static int coreAppIpcId(111);
static int restartTimeout(60);

void sendIpcMsgToCoreApp (IPCInterface & ipc, std::string appName, std::string msg) {
    uint8_t msgType(flexd::icl::ipc::FleXdIPCMsgTypes::IPCMsg);
    std::string payloadString{appName + " >> " + msg};
    std::vector<uint8_t> payload(payloadString.begin(), payloadString.end());
    auto msgPtr = std::make_shared<flexd::icl::ipc::FleXdIPCMsg>(msgType, std::move(payload));
    ipc.send(coreAppIpcId, msgPtr);
    FLEX_LOG_INFO("SendIpcMsgToCoreApp: ",  payloadString , "\n"); 
}

bool hasEnding (std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

std::vector<std::string> returnPidFilesPaths(char* dirPath) {
    std::vector<std::string> pidFilesVector;
    DIR *dir;
    struct dirent *entry;
    dir = opendir(dirPath);
    if(!dir) {
        return pidFilesVector;
    }
    while ( (entry = readdir(dir)) != NULL) {
        if ( entry->d_name[0] !=  '.') {
            std::string filePath = std::string(dirPath) + "/" + std::string(entry->d_name);
            if ( hasEnding(filePath, ".pid")){
                pidFilesVector.push_back(filePath);
            }
        }
    }
    return pidFilesVector;
}

void createWatchdogsForAllExistingPidFiles(FleXdEpoll& poller, IPCInterface & ipc, std::vector<std::string> pathVector) {
    for (unsigned int i = 0; i < pathVector.size(); i++) {
        std::string pathStr = pathVector.at(i);
        char* dirPathPtr = &pathStr[0];
        std::vector<std::string> filesPaths = returnPidFilesPaths(dirPathPtr);
        for (unsigned j = 0; j < filesPaths.size(); j++){
            watchdogs.push_back(std::make_unique<FleXdWatchdog>(poller, ipc, filesPaths.at(j), restartTimeout, coreAppIpcId));
            std::string pidFileName = filesPaths.at(j).substr(filesPaths.at(j).rfind("/") + 1);
            std::string appName = pidFileName.substr(0, pidFileName.length() - 4);
            sendIpcMsgToCoreApp(ipc, appName, "application is running.");
        }
    }
}

void removeUnusedWatchdogsAndOldPidFiles() {
    for (unsigned int it = 0; it < watchdogs.size(); it++) {
        if (!watchdogs.at(it).get()->isValid()) {
            std::remove(watchdogs.at(it).get()->getPath().c_str());
            std::vector<std::unique_ptr<FleXdWatchdog>>::iterator nth = watchdogs.begin() + it;
            watchdogs.erase(nth);
        }
    }
}

std::string getAppNameFromEventName(std::string eventName) {
    std::string appName, helper;
    if (eventName.at(0) == '.'){         
        helper = eventName.substr(1, eventName.length());
        appName = helper.substr(0, helper.find('.'));
    } else {
        appName = eventName.substr(0, eventName.find('.'));   
    }
    return appName;
}

static void onEvent(FleXdEpoll& poller, IPCInterface& ipc, int fd, int * wd, std::vector<std::string> folders) {
    char buf[4096]
            __attribute__((aligned(__alignof__(struct inotify_event))));
    const struct inotify_event *event;
    int i;
    ssize_t len;
    char *ptr;
    
    for (;;) {
        len = read(fd, buf, sizeof buf);
        if (len == -1 && errno != EAGAIN) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        if (len <= 0)
            break;
        for (ptr = buf; ptr < buf + len;
                ptr += sizeof (struct inotify_event) +event->len) {
            event = (const struct inotify_event *) ptr;
            std::string actualFolder{""};
            for (i = 0; i < (int) folders.size(); ++i) {
                if (wd[i] == event->wd) {
                    actualFolder = folders.at(i).c_str();
                }
            } 
            std::string fileName = event->name;
            std::string appName, fileType, newWatch;
            appName = getAppNameFromEventName(fileName);
            fileType = fileName.substr(fileName.length() - 3, fileName.length());
            newWatch = actualFolder + "/" + fileName;
            
            if (event->mask & IN_ALL_EVENTS) {
                for (unsigned int it = 0; it < watchdogs.size(); it++) {
                    if (!watchdogs.at(it).get()->getAppName().compare(appName)){
                        watchdogs.at(it).get()->resetTimer();
                        break;
                    }
                }          
            }
            
            if (!fileType.compare("pid")) { 
                
                if (event->mask & IN_CREATE) {
                    bool sendCreateMsg = true;
                    for (unsigned int it = 0; it < watchdogs.size(); it++) {
                        if (!watchdogs.at(it).get()->getAppName().compare(appName)){
                            sendCreateMsg = false;
                        }
                    }
                    if (sendCreateMsg){
                        watchdogs.push_back(std::make_unique<FleXdWatchdog>(poller, ipc, newWatch, restartTimeout, coreAppIpcId));
                        sendIpcMsgToCoreApp(ipc, appName, "application started.");
                        break;
                    }
                }
                
                if (event->mask & IN_DELETE) {
                    for (unsigned int it = 0; it < watchdogs.size(); it++) {
                        if (!watchdogs.at(it).get()->getAppName().compare(appName)){
                            std::vector<std::unique_ptr<FleXdWatchdog>>::iterator nth = watchdogs.begin() + it;
                            watchdogs.erase(nth);
                            sendIpcMsgToCoreApp(ipc, fileName, "application closed.");
                            break;
                        }
                    }          
                }
            }
        }
    }
}

void loadConfiguration() {
    ini::INIParser::getInstance().parseFiles(pathToConfigFile);
    int helperInt = 0;
    std::string watchedDirectoriesString = ini::INIParser::getInstance().get("watched_directories:path", std::string(""));
    std::istringstream stream(watchedDirectoriesString);
    for (std::string line; std::getline(stream, line);) {
        watchedDirectories.push_back(line);
    }
    watchdogIpcId = ini::INIParser::getInstance().get("ipc_settings:watchdogId", helperInt);
    coreAppIpcId = ini::INIParser::getInstance().get("ipc_settings:coreAppId", helperInt);
    restartTimeout = ini::INIParser::getInstance().get("timing:restartTimeout", helperInt);
}

int* inotifyWatchInit(int& fd, std::vector<std::string> watchedDirectories) {
    fd = inotify_init1(IN_NONBLOCK);
    if (fd == -1) {
        perror("inotify_init1");
        exit(EXIT_FAILURE);
    }
    int * wd = (int*) calloc(watchedDirectories.size(), sizeof (int));
    if (wd == NULL) {
        perror("calloc");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < (int) watchedDirectories.size(); i++) {
        std::string * myString = &watchedDirectories.at(i);
        FLEX_LOG_INFO("\n Add watch for directory: ", myString->c_str(), "\n");
        const char * charPtr = myString->c_str();
        wd[i] = inotify_add_watch(fd, charPtr,
                 IN_CREATE | IN_DELETE);
        if (wd[i] == -1) {
            FLEX_LOG_INFO("\n Cannot watch: ", watchedDirectories.at(i).c_str(), "\n");
            perror("inotify_add_watch");
            exit(EXIT_FAILURE);
        }
    }
    return wd;
}

int main(int argc, char** argv) {
    loadConfiguration();
    FleXdEpoll poller(10);
    FLEX_LOG_INIT(poller, "FleX-d_Watchdog");   
    IPCInterface ipcInterface(watchdogIpcId, poller);
    ipcInterface.addPeer(coreAppIpcId);
    
    int fd;
    int * wd = inotifyWatchInit(fd, watchedDirectories);
    FleXdWatchdogEvent event(poller, ipcInterface, watchedDirectories, fd, wd, onEvent);
    std::thread handlerPoller(&FleXdEpoll::loop, &poller);
    event.init();
    
    createWatchdogsForAllExistingPidFiles(poller, ipcInterface, watchedDirectories);

    while (true) {
        fflush(stdout);
        FLEX_LOG_INFO("main loop \n");
        removeUnusedWatchdogsAndOldPidFiles();
        sleep(1);
    }
    
    poller.endLoop();
    handlerPoller.join();
    ::close(fd);
    exit(EXIT_SUCCESS);
}
