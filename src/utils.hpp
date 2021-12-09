#ifndef LOGS_UTILS_H
#define LOGS_UTILS_H

#include "constants.hpp"
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
        int daysToKeepLogFiles{};
        bool showLogDebug{};
        std::unordered_map<std::string, std::unordered_map<std::string, std::string> > iniSections;
        void setExecDir();
        void setExecFileName();

        void inline setDaysToKeepLogFiles(int x){ this->daysToKeepLogFiles = x; }
		int inline getDaysToKeepLogFiles() const{ return this->daysToKeepLogFiles; }

        void inline setShowLogDebug(bool x){ this->showLogDebug = x; }
		int inline getShowLogDebug() const{ return this->showLogDebug; }


    public:
        Utils();
        explicit Utils(bool);
        Utils(bool, int);
        ~Utils();

        void print(std::string_view);
        bool gzipFile(std::string&, std::string&);
        bool deleteFile(std::string&);
        bool isFileOlderThanXDays(std::string const&, int);
        const char* str2char(std::string);
        unsigned int getHash(const char*, int);
        void log(const char*, std::string_view);
        bool inline getIsWindows() { return IS_WINDOWS; }
        char inline getSep() { return OS_SEP; }
        std::string getIsoTimeStr();
        std::string inline getVersion() { return VERSION; }
        std::string inline getCfgFileName() { return CFG_FILENAME; }
        std::string inline getExecFileName() const{ return this->execFileName; }
        std::string inline getExecDir() const{ return this->execDir; }
        std::string get_ini_value(std::string const&, std::string const&);




};


#endif //LOGS_UTILS_H
