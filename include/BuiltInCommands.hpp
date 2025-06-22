#pragma once
#include <vector>
#include <string>

namespace BuiltIn
{
    void handle_exit(const std::vector<std::string> &args);
    void handle_cd(const std::vector<std::string> &args);
    void handle_pwd(const std::vector<std::string> &args);
    void handle_history(const std::vector<std::string> &args);
    void handle_type(const std::vector<std::string> &args);

    bool handle_builtin(const std::vector<std::string> &args);

    extern int last_appended_index;
}
