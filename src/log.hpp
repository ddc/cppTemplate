#ifndef LOG_H
#define LOG_H

#include <string>
#include <string_view>
#include <fstream>
#include <filesystem>


namespace fs = std::filesystem;


class Log
{
    private:
        int daysToKeepFile{};
        bool showDebug{};
        std::string_view logLevel{};
        std::string_view fileName{};
        std::string dirLogs{};
        std::string filePath{};
        std::ofstream logFile{};

        void inline setDaysToKeep(int x){ this->daysToKeepFile = x; }
		int inline getDaysToKeep() const{ return this->daysToKeepFile; }

        void inline setShowDebug(bool x){ this->showDebug = x; }
		int inline getShowDebug() const{ return this->showDebug; }

        void inline setLogLevel(std::string_view x){ this->logLevel = x; }
		std::string_view inline getLogLevel() const{ return this->logLevel; }

        void inline setFileName(std::string_view x){ this->fileName = x; }
        std::string_view inline getFileName() const{ return this->fileName; }

        void inline setDirLogs(std::string const &x){ this->dirLogs = x; }
        std::string inline getDirLogs() const{ return this->dirLogs; }

        void inline setFilePath(std::string const &x){ this->filePath = x; }
        std::string inline getFilePath() const{ return this->filePath; }

        void setLogPaths();
        void initFuncCalls();
        void checkLogDirPath() const;
        void openLogFile();
        void compressOldLogs();
        void removeOldLogs();
        void writeMsg(std::string_view);
        void setMsg(std::string_view);
        static int getFileCTime(fs::path const&);

    public:
        Log();
        explicit Log(bool);
        Log(bool, int);
        ~Log();

        void inline debug(std::string_view msg) { this->setLogLevel("DEBUG"); this->setMsg(msg); };
        void inline error(std::string_view msg) { this->setLogLevel("ERROR"); this->setMsg(msg); };
        void inline fatal(std::string_view msg) { this->setLogLevel("FATAL"); this->setMsg(msg); };
        void inline info(std::string_view msg) { this->setLogLevel("INFO"); this->setMsg(msg); };
        void inline warning(std::string_view msg) { this->setLogLevel("WARNING"); this->setMsg(msg); };
};

#endif
