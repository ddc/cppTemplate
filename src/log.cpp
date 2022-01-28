#include "log.hpp"
#include "utils.hpp"
#include <ctime>
#include <sys/stat.h>
#include <charconv>
#include <algorithm>


namespace rg = std::ranges;
namespace fs = std::filesystem;


Log::Log()
{
    if (daysToKeepLogFiles == 0)
        daysToKeepLogFiles = 30;
    this->setLogPaths();
}


Log::Log(bool showDebug)
{
    if (daysToKeepLogFiles == 0)
        daysToKeepLogFiles = 30;
    Log::setShowDebug(showDebug);
    this->setLogPaths();
}


Log::Log(bool showDebug, int daysToKeep)
{
    if (daysToKeep == 0 || daysToKeepLogFiles == 0)
        daysToKeep = 30;
    Log::setShowDebug(showDebug);
    Log::setDaysToKeep(daysToKeep);
    this->setLogPaths();
}

Log::~Log()
{
    this->logFile.close();
}


void Log::setLogPaths()
{
    Utils utils;
    std::stringstream appendSS;

    std::string filename = utils.getExecFileName();
    filename = filename.substr(0, filename.length() - 3);
    appendSS << filename << "log";
    this->setFileName(appendSS.str());
    appendSS = std::stringstream();

    std::string execPath = utils.getExecPath();
    appendSS << execPath << utils.getSep() << "logs";
    this->setDirLogs(appendSS.str());
    appendSS = std::stringstream();

    appendSS << this->getDirLogs() << utils.getSep() << this->getFileName();
    this->setFilePath(appendSS.str());
    appendSS = std::stringstream();

    this->initFuncCalls();
}


void Log::initFuncCalls()
{
    this->checkLogDirPath();
    this->removeOldLogs();
    this->compressOldLogs();
    this->openLogFile();
}


void Log::checkLogDirPath() const
{
    try
    {
        if(auto dirlogs = fs::path(this->getDirLogs()); !fs::is_directory(dirlogs))
            fs::create_directories(dirlogs);
    }
    catch(const fs::filesystem_error& err)
    {
        Utils utils;
        std::stringstream msg;
        msg << "["<<Utils::getIsoTimeStr()<<"]:"
            << "[FATAL]:" << this->getDirLogs() << "]:"
            << err.what();
        Utils::print(msg.str());
        throw std::exception();
    }
}


void Log::openLogFile()
{
    auto filepath = fs::path(this->getFilePath());
    if(fs::is_regular_file(filepath))
        this->logFile.open(filepath, std::ios_base::app);
    else
        this->logFile.open(filepath);
}


int Log::getFileCTime(fs::path const &fpath)
{
    std::stringstream ss;
    ss << fpath.string();
    struct stat fileInfo{};
    std::string fPath;
    fPath = ss.str();
    const char *flPath = fPath.c_str();
    stat(flPath, &fileInfo);
    long long fctime = fileInfo.st_ctime;
    ss = std::stringstream();
    ss << std::put_time(std::localtime(&fctime), "%Y%m%d");
    std::string_view fctimeStr;
    fctimeStr = ss.str();
    ss = std::stringstream();

    int dateFctime;
    if(const std::from_chars_result result = std::from_chars(fctimeStr.data(), fctimeStr.data() + fctimeStr.size(), dateFctime);
        result.ec == std::errc::invalid_argument || result.ec == std::errc::result_out_of_range)
        throw std::exception();
    return dateFctime;
}


void Log::compressOldLogs()
{
    Utils utils;
    for(auto &file : fs::directory_iterator(this->getDirLogs()))
    {
        std::string filePathIn = file.path().string();
        std::string filePathInExt = filePathIn.substr(filePathIn.find_last_of('.') + 1);
        bool isFileOlderThanXDays = Utils::isFileOlderThanXDays(filePathIn, 1);

        if(filePathInExt == "log" && fs::is_regular_file(file) && isFileOlderThanXDays)
        {
            int fileCtime = Log::getFileCTime(file.path());
            std::string filename = filePathIn.substr(filePathIn.find_last_of(utils.getSep()) + 1);
            std::stringstream ss;
            ss << this->getDirLogs() << utils.getSep() << filename.substr(0,filename.length() - 4) << "_" << fileCtime << ".log";
            std::string filePathOut;
            filePathOut = ss.str();

            if(Utils::gzipFile(filePathIn, filePathOut))
                utils.deleteFile(filePathIn);
        }
    }
}


void Log::removeOldLogs()
{
    Utils utils;
    for(auto &file : fs::directory_iterator(this->getDirLogs()))
    {
        std::string curFilePath = file.path().string();
        std::string curFileExt = curFilePath.substr(curFilePath.find_last_of('.') + 1);
        bool isFileOlderThanXDays = Utils::isFileOlderThanXDays(curFilePath, Log::getDaysToKeep());

        if(curFileExt == "gz" && fs::is_regular_file(file) && isFileOlderThanXDays)
            utils.deleteFile(curFilePath);
    }
}


void Log::writeMsg(std::string_view fullMsg)
{
    Utils::print(fullMsg);
    this->logFile << fullMsg << std::endl;
}


void Log::setMsg(std::string_view msg)
{
    Utils utils;
    std::stringstream fullMsg;
    fullMsg << "["<<Utils::getIsoTimeStr()<<"]:"
            << "["<<this->getLogLevel()<<"]:"
            << msg;

    if(Log::getShowDebug())
        this->writeMsg(fullMsg.str());
    else
    {
        if(this->getLogLevel() != "DEBUG")
            this->writeMsg(fullMsg.str());
    }
}
