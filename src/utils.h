#ifndef LOGS_UTILS_H
#define LOGS_UTILS_H

#include "constants.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <filesystem>


namespace fs = std::filesystem;


class Utils
{
    private:
        std::string execFileName{};
        std::string execDir{};
        std::string logger{};
        int daysToKeepLogFiles{};
        bool showLogDebug{};
        void setExecDir();
        void setExecFileName();

        void inline setDaysToKeepLogFiles(int x){ this->daysToKeepLogFiles = x; }
		int inline getDaysToKeepLogFiles() const{ return this->daysToKeepLogFiles; }

        void inline setShowLogDebug(bool x){ this->showLogDebug = x; }
		int inline getShowLogDebug() const{ return this->showLogDebug; }

    public:
        Utils();
        Utils(bool);
        Utils(bool, int);
        ~Utils();

        void print(std::string_view const) const;
        bool gzipFile(std::string&, std::string &);
        bool deleteFile(std::string&);
        bool isFileOlderThanXDays(std::string const&, int);
        const char* str2char(std::string str);
        unsigned int getHash(const char*, int);
        void log(const char *level, std::string_view msg);
        bool inline getIsWindows() const{ return IS_WINDOWS; }
        char inline getSep() const{ return OS_SEP; }
        std::string getIsoTimeStr() const;
        std::string inline getVersion() const{ return VERSION; }
        std::string inline getCfgFileName() const{ return CFG_FILENAME; }
        std::string inline getExecFileName() const{ return this->execFileName; }
        std::string inline getExecDir() const{ return this->execDir; }




        void get_ini_section();




};


#endif //LOGS_UTILS_H
