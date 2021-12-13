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
        utils.log("info", "Iniciando v"  + utils.getVersion());


    }
    catch(const std::exception&)
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
