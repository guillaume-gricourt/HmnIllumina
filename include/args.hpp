#ifndef ARGS_INCLUDED
#define ARGS_INCLUDED

#include <string>

class Args {
private:
    std::string finput;
    std::string foutput;

public:
    // Getters Setters
    void setInput(const std::string &);
    void setOutput(const std::string &);

    std::string getInput() const;
    std::string getOutput() const;

    // Others
    bool isValid() const;

private:
    static bool isDir(const std::string &);
};

#endif  // ARGS_INCLUDED
