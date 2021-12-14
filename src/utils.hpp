#ifndef UTILS_HPP
#define UTILS_HPP

#include "log.hpp"
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
        bool showDebug{};
        int daysToKeepFile{};
        char osSep{};
        bool isWindows{};
        std::string execFileName{};
        std::string execPath{};

        void inline setShowLogDebug(bool x){ this->showDebug = x; }
        void inline setDaysToKeepLogFiles(int x){ this->daysToKeepFile = x; }
        void inline setOsSep(char x){ this->osSep = x; }
        void inline setIsWindows(bool x){ this->isWindows = x; }

        void setConstants();
        void setExecPath();
        void setExecFileName();

    public:
        Utils();
        explicit Utils(bool);
        Utils(bool, int);
        ~Utils();

        int inline getShowLogDebug() const { return this->showDebug;}
        int inline getDaysToKeepLogFiles() const { return this->daysToKeepFile; }
        char inline getSep() const { return this->osSep; }
        bool inline getIsWindows() const { return this->isWindows; }
        std::string inline getIniFilePath() { return this->execPath + DEFAULT_CONFIG_DIRNAME + Utils::getSep() + Utils::getIniFileName(); }
        std::string inline getExecFileName() const{ return this->execFileName; }
        std::string inline getExecPath() const{ return this->execPath; }

        static std::string inline getVersion() { return VERSION; }
        static std::string inline getDefaultConfigDirName() { return DEFAULT_CONFIG_DIRNAME; }
        static std::string inline getIniFileName() { return INI_FILENAME; }
        static std::string getIsoTimeStr();
        static bool gzipFile(std::string&, std::string&);
        static const char* str2char(std::string);
        static bool isFileOlderThanXDays(std::string const&, int);

        static void print(std::string_view);
        bool deleteFile(std::string&);
        void log(const char*, std::string_view);
        unsigned int getHash(const char*, int);
        std::string get_ini_value(std::string const&, std::string const&);
        std::map<std::string, std::string> get_ini_section(std::string const&);
};

#endif
