#include "AutoCompleter.hpp"
#include <readline/readline.h>
#include <readline/history.h>

namespace AutoComplete
{
    std::vector<std::string> commands = {"pwd", "exit", "type", "echo", "history"};

    char *command_generator(const char *text, int state)
    {
        static size_t index;
        static size_t len;

        if (state == 0)
        {
            index = 0;
            len = strlen(text);
        }

        while (index < commands.size())
        {
            const std::string &cmd = commands[index++];
            if (cmd.compare(0, len, text) == 0)
            {
                return strdup(cmd.c_str());
            }
        }

        return nullptr;
    }

    char **completer(const char *text, int start, int end)
    {
        return rl_completion_matches(text, command_generator);
    }
}