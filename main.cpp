#include "src/utils.hpp"
#include "src/sqlite.hpp"


bool showLogDebug{};
int daysToKeepLogFiles{};


int main(int argc, char **argv)
{
    try
    {
        showLogDebug = false;
        if(argc > 1)
        {
            std::string argv1 = argv[1];
            if(!std::isdigit(*argv[1]))
                if(argv1 == "-d" or argv1 == "--debug")
                    showLogDebug = true;
        }

        Utils utils;
        daysToKeepLogFiles = stoi(utils.get_ini_value("main", "daysToKeepLogs"));
        utils.log("info", "Iniciando v" + Utils::getVersion());

        Sqlite sqlite;




    }
    catch(GetException& e)
    {
        Utils utils;
        utils.log("error", e.what() + e.getInfo());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
