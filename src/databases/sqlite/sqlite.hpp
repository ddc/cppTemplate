#ifndef SQLITE_HPP
#define SQLITE_HPP

#include "../../utils.hpp"
#include <iostream>
#include <string>


class Sqlite
{
    private:
        std::string sqliteFilePath{};

        static std::string inline getSqliteFileName() { return SQLITE_FILENAME; }
        void setSqliteFilePath();

    public:
        Sqlite();
        ~Sqlite();

        std::string getSqliteFilePath() { return this->sqliteFilePath; }
        bool testSqliteConnection();

        void execute(std::string&);
        void select(std::string&);
};

#endif
