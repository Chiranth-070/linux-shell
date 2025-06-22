#include "Shell.hpp"
#include "CommandParser.hpp"
#include "BuiltInCommands.hpp"
#include "AutoCompleter.hpp"
#include "Utils.hpp"

#include <readline/readline.h>
#include <readline/history.h>
#include <iostream>
#include <cstdlib>

Shell::Shell()
{
    const char *hf = std::getenv("HISTFILE");
    if (hf)
        histfile = std::string(hf);
}

void Shell::setupReadline()
{
    rl_attempted_completion_function = AutoComplete::completer;
}

void Shell::loadHistory()
{
    if (!histfile.empty())
    {
        read_history(histfile.c_str());
    }
}

void Shell::saveHistory()
{
    if (!histfile.empty())
    {
        write_history(histfile.c_str());
    }
}

void Shell::run()
{
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    setupReadline();
    loadHistory();

    while (true)
    {
        char *line = readline("$ ");
        if (!line)
            break;

        std::string input(line);
        free(line);

        if (!input.empty())
        {
            add_history(input.c_str());
            handleInput(input);
        }
    }

    saveHistory();
}

void Shell::handleInput(const std::string &input)
{
    auto tokens = Parser::split(input, " ");
    if (tokens.empty())
        return;

    if (BuiltIn::handle_builtin(tokens))
        return;

    std::string fullPath;
    if (Utils::locate(tokens[0], fullPath))
    {
        system(input.c_str());
    }
    else
    {
        std::cout << tokens[0] << ": command not found\n";
    }
}
