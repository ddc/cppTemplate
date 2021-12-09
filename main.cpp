#include "src/utils.hpp"

int main(int argc, char **argv)
{
    try
    {
        bool debug = false;
        if(argc > 1)
        {
            std::string argv1 = argv[1];
            if(!std::isdigit(*argv[1]))
                if(argv1 == "-d" or argv1 == "--debug")
                    debug = true;
        }

        Utils utils(debug);
        utils.log("warning", "Iniciando v"  + utils.getVersion());

        std::string days_to_keep_logs;
        days_to_keep_logs = utils.get_ini_value("main", "days_to_keep_logs");
        utils.log("warning", "days_to_keep_logs: " + days_to_keep_logs);




    }
    catch(const std::exception&)
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
