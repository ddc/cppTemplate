#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <iostream>
#include <exception>
#include <utility>

class GetException: public std::exception
{
    private:
        std::string file;
        int line;
        std::string func;
        std::string info;

    public:
    	GetException(exception msg, std::string& file_, int line_, std::string& func_, std::string& info_ = (std::string &) ""):
            std::exception(std::move(msg)),
            file (file_),
            line (line_),
            func (func_),
            info (info_)
        {

        };

        const std::string& getFile() const { return file; }
        int getLine() const { return line; }
		const std::string& getFunc() const { return func; }
        const std::string& getInfo() const { return info; }

};

#endif
