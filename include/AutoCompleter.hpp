#pragma once
#include <vector>
#include <string>

namespace AutoComplete
{
    extern std::vector<std::string> commands;

    char *command_generator(const char *text, int state);

    char **completer(const char *text, int start, int end);
}