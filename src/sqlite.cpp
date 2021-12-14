#include "sqlite.hpp"
#include <cstdio>
#include <sqlite3.h>


Sqlite::Sqlite()
{
    this->setSqliteFilePath();
    this->testSqliteConnection();







}


Sqlite::~Sqlite()
= default;


void Sqlite::setSqliteFilePath()
{
    Utils utils;
    std::stringstream ss;
    ss << utils.getExecPath()
       << utils.getSep()
       << DEFAULT_CONFIG_DIRNAME
       << utils.getSep()
       << Sqlite::getSqliteFileName();
    this->sqliteFilePath = ss.str();
}


bool Sqlite::testSqliteConnection()
{
    Utils utils;
    sqlite3* DB;
    int rs = sqlite3_open(this->getSqliteFilePath().c_str(), &DB);
    utils.log("debug", "SqliteFilePath: "+ this->getSqliteFilePath());

    if (rs)
    {
        std::stringstream ss;
        ss << "Error open SQLite Database " << sqlite3_errmsg(DB);
        utils.log("error", ss.str());
        return false;
    }
    else
        utils.log("debug", "SQLite Database Opened Successfully!");

    sqlite3_close(DB);
    return true;
}
