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
#include <Logger.h>

using namespace flexd::icl::ipc;

static std::vector<std::unique_ptr<FleXdWatchdog>> watchdogs;

static void onEvent(FleXdEpoll& poller, int fd, int * wd, std::vector<std::string> folders)
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
        
            std::string actualFolder = "";
            for (i = 0; i < (int) folders.size(); ++i) {
                if (wd[i] == event->wd) {
                    actualFolder = folders.at(i).c_str();
                }
            } 
            std::string newWatch = actualFolder + "/" + event->name;
            
            if (event->mask & IN_CREATE){
                printf("IN_CREATE: ");
                watchdogs.push_back(std::make_unique<FleXdWatchdog>(poller, fd, newWatch));
            }
            if (event->mask & IN_DELETE){
                printf("IN_DELETE: ");
            }
            printf("actual folder + event.name: %s\n", newWatch.c_str());
        }
    }
}

int main(int argc, char** argv) {
    FleXdEpoll poller(10);
    FLEX_LOG_INIT(poller, "FleX-d_Watchdog");
    
    ini::INIParser::getInstance().parseFiles("../resources/config.ini");
    std::string watchedDirectoriesString = ini::INIParser::getInstance().get("watched_directories:path", std::string(""));
    std::istringstream stream(watchedDirectoriesString);
    std::vector<std::string> watchedDirectories;
    for (std::string line; std::getline(stream, line);) {
        watchedDirectories.push_back(line);
    }
    
    int fd = inotify_init1(IN_NONBLOCK);
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
        printf("add watch for folder: '%s'\n", myString->c_str());
        const char * charPtr = myString->c_str();
        wd[i] = inotify_add_watch(fd, charPtr,
                 IN_CREATE | IN_DELETE);
        if (wd[i] == -1) {
            fprintf(stderr, "Cannot watch '%s'\n", watchedDirectories.at(i).c_str());
            perror("inotify_add_watch");
            exit(EXIT_FAILURE);
        }
    }
    
    FleXdWatchdogEvent event(poller, fd, wd, watchedDirectories, onEvent);
    std::thread handlerPoller(&FleXdEpoll::loop, &poller);
    event.init();
    
    int i = 300;
    printf("main loop was started \n");
    while (i--) {
        fflush(stdout);
        printf("main loop \n");
        sleep(1);
    }
    
    poller.endLoop();
    handlerPoller.join();
    ::close(fd);
    exit(EXIT_SUCCESS);
}