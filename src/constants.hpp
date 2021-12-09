#ifndef LOGS_CONSTANTS_H
#define LOGS_CONSTANTS_H

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
#define CFG_FILENAME "config/settings.ini"








#endif //LOGS_CONSTANTS_H
