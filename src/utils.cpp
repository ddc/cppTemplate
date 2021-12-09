#include "utils.hpp"
#include "log.hpp"
#include <sys/stat.h>
#include <fstream>
#include <string_view>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <regex>


namespace ch = std::chrono;
namespace fs = std::filesystem;
namespace bios = boost::iostreams;


Utils::Utils()
{
    this->setExecDir();
    this->setExecFileName();
    this->setShowLogDebug(false);
    this->setDaysToKeepLogFiles(30);
}

Utils::Utils(bool showDebugLogs)
{
    this->setExecDir();
    this->setExecFileName();
    this->setShowLogDebug(showDebugLogs);
    this->setDaysToKeepLogFiles(30);
}

Utils::Utils(bool showDebugLogs, int daysToKeepLogFile)
{
    this->setExecDir();
    this->setExecFileName();
    this->setShowLogDebug(showDebugLogs);
    this->setDaysToKeepLogFiles(daysToKeepLogFile);
}

Utils::~Utils()
{

}

void Utils::setExecDir()
{
    if(this->getIsWindows())
    {
        char buffer[MAX_PATH];
        GetModuleFileNameA(NULL, buffer, MAX_PATH);
        std::string::size_type pos = std::string(buffer).find_last_of(this->getSep());
        this->execDir = std::string(buffer).substr(0, pos);
    }
    else
    {
        this->execDir = "logs";
    }
}

void Utils::setExecFileName()
{
    if(this->getIsWindows())
    {
        char buffer[MAX_PATH];
        GetModuleFileNameA(nullptr, buffer, MAX_PATH);
        std::string::size_type pos = std::string(buffer).find_last_of("\000");
        auto dir = std::string(buffer).substr(0, pos);
        this->execFileName = dir.substr(dir.find_last_of(Utils::getSep()) + 1);
    }
    else
    {
        this->execFileName = "main";
        //char result[PATH_MAX];
        //ssize_t count = this->readlinkCount;
        //std::string appPath = std::string(result, (count > 0) ? count : 0);
        //std::size_t found = appPath.find_last_of(this->getSep());
        //this->execFileName = appPath.substr(0, found);
    }
}

void Utils::print(std::string_view x)
{
    std::cout << x << std::endl;
}

bool Utils::isFileOlderThanXDays(std::string const &fPath, int kdays)
{
    struct stat fileInfo{};
    const char *flPath = fPath.c_str();
    int statFile = stat(flPath, &fileInfo);

    if(statFile == 0)
    {
        ch::system_clock::time_point now;
        ch::system_clock::time_point cutoffTime;
        std::time_t cutoffTimeT;
        long long fctime = fileInfo.st_ctime;

        now = ch::system_clock::now();

        if(kdays <= 1)
        {
            std::stringstream ftime;
            ftime << std::put_time(std::localtime(&fctime), "%Y-%m-%d");
            std::string ftimeStr = ftime.str();
            int fyear = std::stoi(ftimeStr.substr(0, 4));
            int fmonth = std::stoi(ftimeStr.substr(5).substr(0, 2));
            int fday = std::stoi(ftimeStr.substr(ftimeStr.find_last_of('-') + 1));

            std::stringstream time;
            std::time_t nowT = ch::system_clock::to_time_t(now);
            time << std::put_time(std::localtime(&nowT), "%Y-%m-%d");
            std::string timeStr = time.str();
            int year = std::stoi(timeStr.substr(0, 4));
            int month = std::stoi(timeStr.substr(5).substr(0, 2));
            int day = std::stoi(timeStr.substr(timeStr.find_last_of('-') + 1));

            if(fyear <= year && fmonth <= month)
            {
                if(fday < day)
                    return true;
            }
        }
        else
            cutoffTime = now - ch::hours(kdays * 24);

        cutoffTimeT = ch::system_clock::to_time_t(cutoffTime);
        if(fctime < cutoffTimeT)
            return true;
    }
    return false;
}

std::string Utils::getIsoTimeStr()
{
    auto now = ch::system_clock::now();
    auto ms = ch::time_point_cast<ch::milliseconds>(now).time_since_epoch().count() % 1000;
    std::time_t nowT = ch::system_clock::to_time_t(now);

    std::stringstream isoTime;
    isoTime << std::put_time(std::localtime(&nowT), "%Y-%m-%dT%H:%M:%S")
            << '.' << std::setfill('0') << std::setw(3) << ms;
    return isoTime.str();
}

std::string Utils::get_ini_value(std::string const& section, std::string const& key)
{
    std::string value;
    std::stringstream appendSS;
    appendSS << this->execDir << this->getSep() << this->getSep() << this->getCfgFileName();
    std::string configFile = appendSS.str();

    this->log("debug", "[Config file]: "+configFile);

    std::ifstream ifs(configFile);
    if(!ifs.good()) throw std::exception();

    static const std::regex comment_regex{R"x(\s*[;#])x"};
    static const std::regex section_regex{R"x(\s*\[([^\]]+)\])x"};
    static const std::regex value_regex{R"x(\s*(\S[^ \t=]*)\s*=\s*((\s?\S+)+)\s*$)x"};
    std::string current_section;
    std::smatch pieces;
    bool sectionFound = false;
    bool keyFound = false;

    for (std::string line; std::getline(ifs, line);)
    {
        if (line.empty() || std::regex_match(line, pieces, comment_regex))
        {
            // skip comment lines and blank lines
        }
        else if (std::regex_match(line, pieces, section_regex))
        {
            if (pieces.size() == 2)
            {
                current_section = pieces[1].str();
                if (pieces[1].str() == section)
                    sectionFound = true;
            }
        }
        else if (std::regex_match(line, pieces, value_regex))
        {
            if (pieces.size() == 4)
            {
                this->iniSections[current_section][pieces[1].str()] = pieces[2].str();
                if (sectionFound && !keyFound)
                {
                    if (pieces[1].str() == key)
                    {
                        keyFound = true;
                        value = pieces[2].str();
                        break;
                    }
                }
            }
        }
    }

    return value;
}

bool Utils::gzipFile(std::string &filePathIn, std::string &filePathOut)
{
    std::stringstream msg;
    try
    {
        filePathOut = (!fs::is_regular_file(filePathOut)) ? filePathOut.append(".gz")  : filePathOut.append("_2.gz");
        std::ifstream inStream(filePathIn, std::ios_base::binary);
        std::ofstream outStream(filePathOut, std::ios_base::binary);
        bios::filtering_streambuf<bios::input> in;
        in.push(bios::gzip_compressor(bios::gzip_params(bios::gzip::best_compression)));
        in.push(inStream);
        bios::copy(in, outStream);
        inStream.close();
        outStream.close();
    }
    catch(bios::gzip_error& err)
    {
        msg << "["<<this->getIsoTimeStr()<<"]:"
            << "[FATAL]:[Could not compress file]:"
            << "["<<filePathIn<<"]:"
            << err.what();
        return false;
    }
    return true;
}

bool Utils::deleteFile(std::string &filePath)
{
    std::stringstream msg;
    try
    {
        std::error_code ec;
        fs::remove(filePath, ec);
        if(ec)
        {
            msg << "["<<this->getIsoTimeStr()<<"]:"
                << "[ERROR]:[Could not delete file]:"
                << "["<<filePath<<"]:"
                << "[Error Number: " << ec.value() << "]: "
                << ec.message();
            this->log("debug", msg.str());
            return false;
        }
        else if(this->showLogDebug)
        {
            msg << "["<<this->getIsoTimeStr()<<"]:"
                << "[DEBUG]:File successfully deleted: "
                << filePath;
            this->log("debug", msg.str());
        }
    }
    catch(fs::filesystem_error& err)
    {
        msg = std::stringstream();
        msg << "["<<this->getIsoTimeStr()<<"]:"
            << "[FATAL]:[Could not delete file]:"
            << "["<<filePath<<"]:"
            << err.what();
        this->log("error", msg.str());
        return false;
    }
    return true;
}

const char* Utils::str2char(std::string str)
{
    const char* str_cstr = &*str.begin();
    return str_cstr;
}

unsigned int Utils::getHash(const char* str, int h = 0)
{
    return !str[h] ? 5381 : (getHash(str, h+1) * 33) ^ str[h];
}

void Utils::log(const char *level, std::string_view msg)
{
    int dtkLogFiles = this->getDaysToKeepLogFiles();
    bool debug = this->getShowLogDebug();
    Log log(debug, dtkLogFiles);
    unsigned int levelInt = this->getHash(level);

    switch(levelInt)
    {
        case 172710356: // debug
        case 209478068: // DEBUG
           log.debug(msg);
           break;
        case 194581501: // error
        case 229190685: // ERROR
           log.error(msg);
           break;
        case 173482715: // fatal
        case 217013051: // FATAL
           log.fatal(msg);
           break;
        case 2888369135: // warning
        case 47734607: // WARNING
           log.warning(msg);
           break;
        case 2087906475: // info
        case 2089022635: // INFO
        default:
           log.info(msg);
           break;
    }
}
