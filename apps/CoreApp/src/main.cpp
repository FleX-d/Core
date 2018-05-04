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
 * Author: Peter Kocity
 *
 * Created on February 16, 2018, 2:22 PM
 */


#include <cstdlib>
#include <iostream>
#include "iApp.h"
#include <map>

int main(int argc, char** argv) {
    
    /*TODO ERASE only testig state machine
    flexd::core::iApp appka;
    flexd::core::iApp appka1;
    flexd::core::iApp a;

    std::cout << "Current state app1: " << appka.getState() << std::endl;
    std::cout << "Current state app2: " << appka1.getState() << std::endl;

    std::cout << "Operation:Freez:   bool:" << appka.freezing() << std::endl;
    std::cout << "Operation:Start:   bool:" << appka.starting() << std::endl;
    std::cout << "Operation:Update:  bool:" << appka.updating() << std::endl;
    std::cout << "Operation:Freez:   bool:" << appka.freezing() << std::endl;
    std::cout << "Operation:Stop:    bool:" << appka.stoping() << std::endl;
    std::cout << "Operation:Unfreez: bool:" << appka.unfreezing() << std::endl;
    std::cout << "Operation:Start:   bool:" << appka.starting() << std::endl;
    std::cout << "Operation:Freez:   bool:" << appka.freezing() << std::endl;
    std::cout << "Operation:Start:   bool:" << appka.starting() << std::endl;
    std::cout << "Operation:Unfreez: bool:" << appka.unfreezing() << std::endl;
    std::cout << "Operation:Stop:    bool:" << appka.stoping() << std::endl;
    std::cout << "Operation:Update:  bool:" << appka.updating() << std::endl;
    std::cout << "Operation:Stop:    bool:" << appka.stoping() << std::endl;
    
    std::cout << "Current state app1: " << appka.getState() << std::endl;
    std::cout << "Current state app2: " << appka1.getState() << std::endl;
    
    std::cout << "Operation:Stop:    bool:" << appka1.starting() << std::endl;
    std::cout << "Current state app2: " << appka1.getState() << std::endl << std::endl;
    
    std::map<std::string, flexd::core::iApp> listOfApp;
    listOfApp.insert(std::make_pair("snake", appka)); //move and return false if exist pair with snake
    listOfApp.insert(std::make_pair("dumy", a));
    listOfApp["earth"] = appka1; //copy and rewrite if exist pair with earth
    if (listOfApp.insert(std::make_pair("earth", appka1)).second == false)
        std::cout << "dont insert , pair exist" << std::endl;

    if (listOfApp.find("earth") != listOfApp.end())
        std::cout << "earth app is in list" << std::endl;

    std::cout << listOfApp.at("dumy").starting() << std::endl;
    if (listOfApp.at("dumy").updating() == false)
        std::cout << "Cant update" << std::endl;

    std::cout << listOfApp.at("dumy").freezing() << std::endl << std::endl;
    
    */
    flexd::core::iApp b;
    b.starting();
    b.stoping();
    b.stoping();
    
    std::cout<<"ending"<<std::endl;
    
    return 0;
}