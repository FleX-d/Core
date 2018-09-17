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
#include <sys/inotify.h>
#include <unistd.h>
#include <thread>
#include <string>
#include <FleXdIPCConnector.h>
#include <FleXdIPCMsgTypes.h>

using namespace flexd::icl::ipc;

static std::vector<std::unique_ptr<FleXdWatchdog>> watchdogs;

void sendIpcMsgToCoreApp (IPCInterface & ipc, std::string appName, std::string msg){
    FLEX_LOG_INFO("FleX-d Watchdog >> sendIpcMsgToCore: \n");
    std::string payloadString{appName};
    uint8_t msgType(flexd::icl::ipc::FleXdIPCMsgTypes::IPCMsg);
    payloadString += " ";
    payloadString += msg;
    std::vector<uint8_t> payload(payloadString.begin(), payloadString.end());
    auto msgPtr = std::make_shared<flexd::icl::ipc::FleXdIPCMsg>(msgType, std::move(payload));
    ipc.send(111, msgPtr);
}

static void onEvent(FleXdEpoll& poller, IPCInterface& ipc, int fd, int * wd, std::vector<std::string> folders)
{
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
            std::string fileType, helper, appName, newWatch;
            if (fileName.at(0) == '.'){
                fileType = fileName.substr(fileName.length() - 3, fileName.length());
                helper = fileName.substr(1, fileName.length());
                FLEX_LOG_INFO("helper: ", helper, " \n");
                appName = helper.substr(0, helper.find('.'));
                newWatch = actualFolder + "/" + event->name;
            } else {
                fileType = fileName.substr(fileName.length() - 3, fileName.length());
                appName = fileName.substr(0, fileName.find('.'));
                newWatch = actualFolder + "/" + event->name;
            }
            FLEX_LOG_INFO("appName: ", appName, " \n");
            FLEX_LOG_INFO("EVENT NAME: ", event->name, " \n");
            FLEX_LOG_INFO("TYPE OF THE FILE: ", fileType, "\n");
            FLEX_LOG_INFO("actual folder + event.name: ", newWatch.c_str(), " \n");
            
            if (event->mask & IN_ALL_EVENTS) {
                   for(unsigned int it = 0; it < watchdogs.size(); it++) {
                        if (!watchdogs.at(it).get()->isValid()){
                            std::vector<std::unique_ptr<FleXdWatchdog>>::iterator nth = watchdogs.begin() + it;
                            watchdogs.erase(nth);
                            FLEX_LOG_INFO("UNUSED WATCHDOG WAS REMOVED: ", appName, " \n");
                        }
                    }
                
                    FLEX_LOG_INFO("IN_ALL_EVENTS: ");
                    for(unsigned int it = 0; it < watchdogs.size(); it++) {
                        FLEX_LOG_INFO("------------COMPARE - appName: ", appName, " \n");
                        FLEX_LOG_INFO("------------COMPARE - watchdogs.at(it).get()->getAppName(): ", watchdogs.at(it).get()->getAppName(), " \n");
                        
                        if (!watchdogs.at(it).get()->getAppName().compare(appName)){
                            watchdogs.at(it).get()->resetTimer();
                            FLEX_LOG_INFO("TIMER WAS RESET: \n");
                            break;
                        }
                    }          
                }
            
            if (!fileType.compare("pid")) {
                FLEX_LOG_INFO("event mask: ", event->mask);
                
                if (event->mask & IN_CREATE) {
                    FLEX_LOG_INFO("IN_CREATE: ");
                    bool sendCreateMsg = true;
                    for(unsigned int it = 0; it < watchdogs.size(); it++) {
                        if (!watchdogs.at(it).get()->getAppName().compare(appName)){
                            sendCreateMsg = false;
                        }
                    }
                    if (sendCreateMsg){
                        watchdogs.push_back(std::make_unique<FleXdWatchdog>(poller, ipc, fd, newWatch));
                        sendIpcMsgToCoreApp(ipc, appName, "application started");
                        break;
                    }
                }
                
                if (event->mask & IN_DELETE) {
                    FLEX_LOG_INFO("IN_DELETE: ");
                    for(unsigned int it = 0; it < watchdogs.size(); it++) {
                        if (!watchdogs.at(it).get()->getAppName().compare(appName)){
                            std::vector<std::unique_ptr<FleXdWatchdog>>::iterator nth = watchdogs.begin() + it;
                            watchdogs.erase(nth);
                            sendIpcMsgToCoreApp(ipc, fileName, "application closed");
                            break;
                        }
                    }          
                }
            }
        }
    }
}

std::vector<std::string> parseWatchedDirsFromConfig(std::string pathToConfigFile){
    std::vector<std::string> dirsVector;
    ini::INIParser::getInstance().parseFiles(pathToConfigFile);
    std::string watchedDirectoriesString = ini::INIParser::getInstance().get("watched_directories:path", std::string(""));
    std::istringstream stream(watchedDirectoriesString);
    for (std::string line; std::getline(stream, line);) {
        dirsVector.push_back(line);
    }
    return dirsVector;
}

int* inotifyWatchInit(int& fd, std::vector<std::string> watchedDirectories){
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
    FleXdEpoll poller(10);
    FLEX_LOG_INIT(poller, "FleX-d_Watchdog");
    FLEX_LOG_INFO("WatchDog main was started \n");
    IPCInterface ipcInterface(100, poller);
    ipcInterface.addPeer(111);
    
    std::vector<std::string> watchedDirectories = parseWatchedDirsFromConfig("../resources/config.ini");
    int fd;
    int * wd = inotifyWatchInit(fd, watchedDirectories);
    FleXdWatchdogEvent event(poller, ipcInterface, watchedDirectories, fd, wd, onEvent);
    std::thread handlerPoller(&FleXdEpoll::loop, &poller);
    event.init();
    
    int i = 300;
    while (i--) {
        fflush(stdout);
        FLEX_LOG_INFO("main loop \n");
        sleep(1);
    }
    
    poller.endLoop();
    handlerPoller.join();
    ::close(fd);
    exit(EXIT_SUCCESS);
}
