#include "sqlite.hpp"
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
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(this->getSqliteFilePath().c_str(), &db);
    utils.log("debug", "SqliteFilePath: "+ this->getSqliteFilePath());

    if(rc)
    {
        std::stringstream ss;
        ss << "Error open SQLite Database " << sqlite3_errmsg(db);
        utils.log("error", ss.str());
        return false;
    }
    else
        utils.log("debug", "SQLite Database Opened Successfully!");

    sqlite3_close(db);
    return true;
}


void Sqlite::execute(std::string &sql)
{
    sqlite3 *db;
    char *errMsg = nullptr;
    const char *csql = sql.c_str();
    int rc;

    rc = sqlite3_open(this->getSqliteFilePath().c_str(), &db);
    if (rc != SQLITE_OK)
    {
        Utils utils;
        utils.log("error", sqlite3_errmsg(db));
    }

    rc = sqlite3_exec(db, csql, Sqlite::callback, nullptr, &errMsg);
    if (rc != SQLITE_OK)
    {
        Utils utils;
        utils.log("error", sqlite3_errmsg(db));
        sqlite3_free(errMsg);
    }

    sqlite3_close(db);
}


void Sqlite::select(std::string &sql)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    const char *csql = sql.c_str();
    int rc;
    Utils utils;

    rc = sqlite3_open(this->getSqliteFilePath().c_str(), &db);
    if (rc != SQLITE_OK)
    {
        utils.log("error", sqlite3_errmsg(db));
    }

    rc = sqlite3_prepare(db, csql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        utils.log("error", sqlite3_errmsg(db));
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        //std::string_view id = sqlite3_column_text(stmt, 0);
        //std::string_view name = sqlite3_column_text(stmt, 1);
        //utils.print(id);
        //utils.print(name);

        //int id = sqlite3_column_int(stmt, 0);
        //unsigned const char *name = sqlite3_column_text(stmt, 1);

        //utils.print(id);
        //utils.print(name);

        fprintf(stderr, "Id/name:  %i - %s.\n", sqlite3_column_int(stmt, 0)
                                              ,sqlite3_column_text(stmt, 1));
    }

    sqlite3_close(db);
}


//int Sqlite::callback(void *NotUsed, int argc, char **argv, char **azColName)
//{
//	int i;
//	for(i=0; i<argc; i++)
//		std::cout<<azColName[i]<<" = " << (argv[i] ? argv[i] : "NULL")<<"\n";
//	std::cout<<"\n";
//	return 0;
//}

int Sqlite::callback(void *data, int argc, char **argv, char **azColName){
   int i;
   fprintf(stderr, "%s: ", (const char*)data);
   for(i=0; i<argc; i++)
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   printf("\n");
   return 0;
}
