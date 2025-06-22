#include "Utils.hpp"
#include "CommandParser.hpp"
#include <unistd.h>

namespace Utils
{
    bool locate(const std::string &program, std::string &output)
    {
        char *path_env = getenv("PATH");
        if (!path_env)
        {
            return false;
        }

        std::vector<std::string> paths = Parser::split(std::string(path_env), ":");
        for (auto path : paths)
        {
            std::string fullPath = path + "/" + program;

            if (access(fullPath.c_str(), F_OK | X_OK) == 0)
            {
                output = fullPath;
                return true;
            }
        }
        return false;
    }
}