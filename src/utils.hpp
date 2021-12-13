#ifndef UTILS_HPP
#define UTILS_HPP

#include "constants.hpp"
#include "exceptions.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <filesystem>
#include <map>


namespace fs = std::filesystem;


class Utils
{
    private:
        std::string execFileName{};
        std::string execPath{};
        int daysToKeepLogFiles{};
        bool showLogDebug{};

        void inline setDaysToKeepLogFiles(int x){ this->daysToKeepLogFiles = x; }
        void inline setShowLogDebug(bool x){ this->showLogDebug = x; }

        void setExecPath();
        void setExecFileName();

    public:
        Utils();
        explicit Utils(bool);
        Utils(bool, int);
        ~Utils();

        int inline getDaysToKeepLogFiles() const{ return this->daysToKeepLogFiles; }
		int inline getShowLogDebug() const{ return this->showLogDebug; }

        static bool inline getIsWindows() { return IS_WINDOWS; }
        static char inline getSep() { return OS_SEP; }
        static std::string inline getVersion() { return VERSION; }

        static std::string inline getIniFileName() { return INI_FILENAME; }
        std::string inline getIniFilePath() { return this->execPath + DEFAULT_CONFIG_DIRNAME + Utils::getSep() + Utils::getIniFileName(); }

        std::string inline getExecFileName() const{ return this->execFileName; }
        std::string inline getExecPath() const{ return this->execPath; }

        static std::string getIsoTimeStr();
        void print(std::string_view);
        bool gzipFile(std::string&, std::string&);
        bool deleteFile(std::string&);
        bool isFileOlderThanXDays(std::string const&, int);
        const char* str2char(std::string);
        unsigned int getHash(const char*, int);
        void log(const char*, std::string_view);
        std::string get_ini_value(std::string const&, std::string const&);
        std::map<std::string, std::string> get_ini_section(std::string const&);
};

#endif
