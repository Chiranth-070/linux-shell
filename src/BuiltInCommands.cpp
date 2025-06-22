#include "BuiltInCommands.hpp"
#include "Utils.hpp"
#include <unordered_map>
#include <unistd.h>
#include <iostream>
#include <filesystem>
#include <readline/readline.h>
#include <readline/history.h>
#include <fstream>

namespace BuiltIn
{
    int last_appended_index = 0;

    using BuiltInFunc = void (*)(const std::vector<std::string> &);

    static std::unordered_map<std::string, BuiltInFunc> builtins = {
        {"exit", handle_exit},
        {"cd", handle_cd},
        {"pwd", handle_pwd},
        {"history", handle_history},
        {"type", handle_type}};

    void handle_exit(const std::vector<std::string> &args)
    {
        exit(0);
    }

    void handle_cd(const std::vector<std::string> &args)
    {
        const char *path = nullptr;

        if (args.size() < 2 || args[1] == "~")
        {
            path = getenv("HOME");
            if (!path)
                path = "/";
        }
        else
        {
            path = args[1].c_str();
        }

        if (chdir(path) != 0)
        {
            std::cout << "cd: " << path << ": ";
            perror("");
        }
    }

    void handle_pwd(const std::vector<std::string> &args)
    {
        std::cout << std::filesystem::current_path().c_str() << std::endl;
    }

    void handle_history(const std::vector<std::string> &args)
    {
        HIST_ENTRY **hist_list = history_list();

                if (args.size() >= 3)
        {
            const std::string &option = args[1];
            const std::string &filepath = args[2];

            if (option == "-r")
            {
                if (read_history(filepath.c_str()) != 0)
                {
                    perror(("Failed to read history from " + filepath).c_str());
                }
                return;
            }

            if (option == "-w")
            {
                if (write_history(filepath.c_str()) != 0)
                {
                    perror(("Failed to write history to " + filepath).c_str());
                }
                return;
            }

            if (option == "-a")
            {
                if (!hist_list)
                    return;

                int total = 0;
                while (hist_list[total])
                    ++total;

                std::ofstream out(filepath, std::ios::app);
                if (!out.is_open())
                {
                    perror(("Failed to open file: " + filepath).c_str());
                    return;
                }

                for (int i = last_appended_index; i < total; ++i)
                {
                    out << hist_list[i]->line << std::endl;
                }

                last_appended_index = total;
                out.close();
                return;
            }
        }

        int n = -1;
        if (args.size() > 1 && args[1][0] != '-')
        {
            try
            {
                n = std::stoi(args[1]);
            }
            catch (...)
            {
                std::cout << "history: invalid number: " << args[1] << std::endl;
                return;
            }
        }

        if (hist_list)
        {
            int total = 0;
            while (hist_list[total])
                ++total;

            int count = (n == -1 || n > total) ? total : n;
            for (int i = total - count; i < total; ++i)
            {
                std::cout << "  " << (i + 1) << "  " << hist_list[i]->line << std::endl;
            }
        }
    }

    void handle_type(const std::vector<std::string> &args)
    {
        if (args.size() < 2)
        {
            std::cout << "Usage: type <command>" << std::endl;
        }

        std::string program = args[1];
        std::string fullPath;
        if (program == "exit" || program == "type" || program == "echo" || program == "history" || program == "pwd")
        {
            std::cout << program << " is a shell builtin" << std::endl;
        }
        else if (Utils::locate(program, fullPath))
        {
            std::cout << program << " is " << fullPath << std::endl;
        }
        else
        {
            std::cout << program << ": not found" << std::endl;
        }
    }

    bool handle_builtin(const std::vector<std::string> &args)
    {
        if (args.empty())
            return false;

        auto it = builtins.find(args[0]);
        if (it != builtins.end())
        {
            it->second(args);
            return true;
        }
        return false;
    }
}
