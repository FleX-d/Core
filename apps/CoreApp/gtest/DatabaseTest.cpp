#include <cstdlib>
#include <iostream>
#include <gtest/gtest.h>

#include "CoreAppDatabase.h"

std::string name = "CoreAppDb";
flexd::core::CoreAppDatabase data("./CoreAppDb.db", name);

/*SQLite structure, ID,AppName, AppVer, Install, Uninstall, Start, Stop, Freez, Unfreez, UpdateDB, MD5*/
/*ID- int autoincremental other text not Null*/

std::string val = "docker install";
std::string val1 = "docker update incorect file";
std::string key = "Install";
std::string key1 = "UpdateDB";
std::string key2 = "Stop";
std::string appName = "Ssnake";
std::string appName1 = "ssnake";
std::string appName2 = "pokus";
std::string ver = "1.00";
std::string ver1 = "2.00";
std::string ver2 = "1.02";
std::string temp, temp1, ID = "ID";

TEST(Database, Fileexist) {
    ASSERT_TRUE(data.fileExists("./CoreAppDb.db"));
    ASSERT_FALSE(data.fileExists("./Core.db"));
}

TEST(Database, add_record_to_db) {
    ASSERT_TRUE(data.addRecord(name, appName, ver));
    ASSERT_TRUE(data.getRecord(name, appName, ver, temp, ID));
    std::cout << "nacitana hodnota je: " << temp << std::endl;
}

TEST(Database, add_record_with_edit) {
    ASSERT_TRUE(data.addRecord(name, appName1, ver1));
    ASSERT_TRUE(data.editRecord(name, appName1, ver1, val, key1));
    ASSERT_TRUE(data.getRecord(name, appName1, ver1, temp, key1));
    std::cout << "nacitana hodnota je: " << temp << std::endl;
}

TEST(Database, multiple_edit_record) {
    ASSERT_TRUE(data.editRecord(name, appName2, ver2, val, key));
    ASSERT_TRUE(data.getRecord(name, appName2, ver2, temp, key));
    std::cout << "nacitana hodnota je: " << temp << std::endl;

    ASSERT_TRUE(data.editRecord(name, appName2, ver2, val1, key2));
    ASSERT_TRUE(data.getRecord(name, appName2, ver2, temp, key2));
    std::cout << "nacitana hodnota je: " << temp << std::endl;
}

TEST(Database, erase_record_from_db) {
    ASSERT_TRUE(data.eraseRecord(name, appName, ver));
    ASSERT_TRUE(data.eraseRecord(name, appName1, ver1));
}