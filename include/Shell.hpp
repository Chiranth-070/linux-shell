#pragma once
#include <string>

class Shell
{
public:
    Shell();
    void run();

private:
    void loadHistory();
    void saveHistory();
    void handleInput(const std::string &input);
    void setupReadline();

    std::string histfile;
};
