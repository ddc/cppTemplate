#ifndef SQLITE_HPP
#define SQLITE_HPP

#include "utils.hpp"
#include "constants.hpp"
#include "exceptions.hpp"
#include <iostream>
#include <string>


class Sqlite
{
    private:
        std::string sqliteFilePath{};


        static std::string inline getSqliteFileName() { return SQLITE_FILENAME; }
        static std::string getSqliteFilePath();




    public:
        Sqlite();
        ~Sqlite();






};

#endif
