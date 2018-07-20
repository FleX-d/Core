/*
 C *opyright (c) 2018, G*loballogic s.r.o.
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
 * File:   StateTest.cpp
 * Author: Peter Kocity
 *
 * Created on March 27, 2018, 11:10 AM
 */

#include "iApp.h"
#include "CoreAppTypes.h"
#include <cstdlib>
#include <gtest/gtest.h>
#include <map>
#include <bits/stl_map.h>

namespace {

    TEST(AppState, State)
    {
        flexd::core::iApp app;

        ASSERT_EQ(app.getState(), flexd::core::JobState::stopped);

        //StoppedState->RunningState
        ASSERT_FALSE(app.stop());
        ASSERT_EQ(app.getState(), flexd::core::JobState::stopped);
        ASSERT_FALSE(app.freeze());
        ASSERT_EQ(app.getState(), flexd::core::JobState::stopped);
        ASSERT_FALSE(app.unfreeze());
        ASSERT_EQ(app.getState(), flexd::core::JobState::stopped);
        ASSERT_TRUE(app.start());
        ASSERT_EQ(app.getState(), flexd::core::JobState::running);

        //RunningState->StoppedState
        ASSERT_FALSE(app.start());
        ASSERT_EQ(app.getState(), flexd::core::JobState::running);
        ASSERT_FALSE(app.update());
        ASSERT_EQ(app.getState(), flexd::core::JobState::running);
        ASSERT_FALSE(app.unfreeze());
        ASSERT_EQ(app.getState(), flexd::core::JobState::running);
        ASSERT_TRUE(app.stop());
        ASSERT_EQ(app.getState(), flexd::core::JobState::stopped);

        //StoppedState->RunningState
        ASSERT_FALSE(app.stop());
        ASSERT_EQ(app.getState(), flexd::core::JobState::stopped);
        ASSERT_FALSE(app.freeze());
        ASSERT_EQ(app.getState(), flexd::core::JobState::stopped);
        ASSERT_FALSE(app.unfreeze());
        ASSERT_EQ(app.getState(), flexd::core::JobState::stopped);
        ASSERT_TRUE(app.start());
        ASSERT_EQ(app.getState(), flexd::core::JobState::running);

        //RunningState->FrozenState
        ASSERT_FALSE(app.start());
        ASSERT_EQ(app.getState(), flexd::core::JobState::running);
        ASSERT_FALSE(app.update());
        ASSERT_EQ(app.getState(), flexd::core::JobState::running);
        ASSERT_FALSE(app.unfreeze());
        ASSERT_EQ(app.getState(), flexd::core::JobState::running);
        ASSERT_TRUE(app.freeze());
        ASSERT_EQ(app.getState(), flexd::core::JobState::pending);

        //FrozenState->RunningState
        ASSERT_FALSE(app.freeze());
        ASSERT_EQ(app.getState(), flexd::core::JobState::pending);
        ASSERT_FALSE(app.start());
        ASSERT_EQ(app.getState(), flexd::core::JobState::pending);
        ASSERT_FALSE(app.update());
        ASSERT_EQ(app.getState(), flexd::core::JobState::pending);
        ASSERT_TRUE(app.unfreeze());
        ASSERT_EQ(app.getState(), flexd::core::JobState::running);

        //RunningState->FrozenState
        ASSERT_TRUE(app.freeze());
        ASSERT_EQ(app.getState(), flexd::core::JobState::pending);

        //FrozenState->StoppedState
        ASSERT_TRUE(app.stop());
        ASSERT_EQ(app.getState(), flexd::core::JobState::stopped);

        //StoppedState->UpdatingState
        ASSERT_TRUE(app.update());
        ASSERT_EQ(app.getState(), flexd::core::JobState::busy);

        //UpdatingState->StoppedState
        ASSERT_FALSE(app.update());
        ASSERT_EQ(app.getState(), flexd::core::JobState::busy);
        ASSERT_FALSE(app.start());
        ASSERT_EQ(app.getState(), flexd::core::JobState::busy);
        ASSERT_FALSE(app.freeze());
        ASSERT_EQ(app.getState(), flexd::core::JobState::busy);
        ASSERT_FALSE(app.unfreeze());
        ASSERT_EQ(app.getState(), flexd::core::JobState::busy);
        ASSERT_TRUE(app.stop());
        ASSERT_EQ(app.getState(), flexd::core::JobState::stopped);
    }

    TEST(AppState, Testing_with_Map){
        flexd::core::iApp app;
        flexd::core::iApp app1;
        flexd::core::iApp a;
        std::map<std::string, flexd::core::iApp> listOfApps;
        ASSERT_TRUE(listOfApps.insert(std::make_pair("snake", app)).second); //move and return false if exist pair with snake
        ASSERT_TRUE(listOfApps.insert(std::make_pair("dummy", a)).second);
        listOfApps["earth"] = app1; //copy and rewrite if exist pair with earth
        ASSERT_FALSE(listOfApps.insert(std::make_pair("earth", app1)).second);

        if(listOfApps.find("earth") != listOfApps.end())
            ASSERT_TRUE(true);

        ASSERT_TRUE(listOfApps.at("dummy").start());
        ASSERT_TRUE(listOfApps.at("dummy").freeze());
        listOfApps.clear();
    }

    TEST(AppState, Testing_couting_and_erasing_sharedpointer){
        std::map<std::string, flexd::core::iApp> listOfApps;
        flexd::core::iApp localApp;
        listOfApps.insert(std::make_pair("a", localApp));
        localApp.~iApp();
    }

    TEST(AppState, Testing_erasing_map){
        std::map<std::string, flexd::core::iApp> listOfApps;
        listOfApps.clear();
    }

}
