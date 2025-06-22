#include "CommandParser.hpp"

namespace Parser
{
    std::vector<std::string> split(std::string input, std::string delimiter)
    {
        std::vector<std::string> res;

        while (!input.empty())
        {
            size_t index = input.find(delimiter);

            std::string token = input.substr(0, index);
            res.push_back(token);

            if (index == std::string::npos)
                break;

            input.erase(0, index + delimiter.size());
        }
        return res;
    }
}