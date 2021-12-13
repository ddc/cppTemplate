#include "sqlite.hpp"


Sqlite::Sqlite()
{
    this->sqliteFilePath = Sqlite::getSqliteFilePath();
    std::cout << this->sqliteFilePath << std::endl;








}


Sqlite::~Sqlite()
= default;


std::string Sqlite::getSqliteFilePath()
{
    Utils utils;
    std::stringstream ss;
    ss << utils.getExecPath()
       << Utils::getSep()
       << DEFAULT_CONFIG_DIRNAME
       << Utils::getSep()
       << Sqlite::getSqliteFileName();
    return ss.str();
}
