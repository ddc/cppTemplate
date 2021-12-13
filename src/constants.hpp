#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
    #include <Windows.h>
    #define IS_WINDOWS 1
    #define OS_SEP '\\'
#else
    #include <unistd.h>
    #include <limits.h>
    #define IS_WINDOWS 0
    #define OS_SEP '/'
    #define READLINK = readlink("/proc/self/exe", result, PATH_MAX)
#endif


#define VERSION "1.0.0"
#define DEFAULT_CONFIG_DIRNAME "config"
#define INI_FILENAME "settings.ini"
#define SQLITE_FILENAME "database.db"
#define LINUX_SFTP_BIN = "/usr/bin/sftp"






#endif
