#include <cstdlib>
#include <iostream>
#include <gtest/gtest.h>

#include "iApp.h"
#include <map>
#include <bits/stl_map.h>


flexd::core::iApp appka;
flexd::core::iApp appka1;
flexd::core::iApp a;
std::map<std::string, flexd::core::iApp> listOfApps;

TEST(AppState, State)
{
    ASSERT_FALSE(appka.freeze());
    ASSERT_TRUE(appka.start());
    ASSERT_FALSE(appka.update());
    ASSERT_TRUE(appka.freeze());
    ASSERT_TRUE(appka.stop());
    ASSERT_FALSE(appka.unfreeze());
    ASSERT_TRUE(appka.start());
    ASSERT_TRUE(appka.freeze());
    ASSERT_FALSE(appka.start());
    ASSERT_TRUE(appka.unfreeze());
    ASSERT_TRUE(appka.stop());
    ASSERT_TRUE(appka.update());

    ASSERT_EQ(appka.getState(),3);
    ASSERT_EQ(appka1.getState(),3);
}

TEST(AppState, Testing_with_Map){

    ASSERT_TRUE(listOfApps.insert(std::make_pair("snake", appka)).second); //move and return false if exist pair with snake
    ASSERT_TRUE(listOfApps.insert(std::make_pair("dumy", a)).second);
    listOfApps["earth"] = appka1; //copy and rewrite if exist pair with earth
    ASSERT_FALSE(listOfApps.insert(std::make_pair("earth", appka1)).second);

    if(listOfApps.find("earth") != listOfApps.end())
        ASSERT_TRUE(true);

    ASSERT_TRUE(listOfApps.at("dumy").start());
    ASSERT_TRUE(listOfApps.at("dumy").freeze());
    listOfApps.clear();
}

TEST(AppState, Testing_couting_and_erasing_sharedpointer){
    flexd::core::iApp ppp;
    listOfApps.insert(std::make_pair("a", ppp));
    std::cout<<"deleting copy of make pair"<<std::endl;
    ppp.~iApp();
    std::cout<<"deleting copy of first object"<<std::endl;
}

TEST(AppState, Testing_erasing_map){
    listOfApps.clear();
}
