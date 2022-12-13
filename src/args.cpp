#include "args.hpp"

#include <sys/stat.h>
#include <sys/types.h>

#include <iostream>
#include <string>

using namespace std;

void Args::setInput(const string& fid) { finput = fid; }
void Args::setOutput(const string& fid) { foutput = fid; }

string Args::getInput() const { return finput; }
string Args::getOutput() const { return foutput; }

// Others
bool Args::isValid() const {
    /*
    Args valid if:
            * input is folder
            * output : basename is folder
    */
    // finput
    bool finput_isdir = Args::isDir(finput);

    // foutput
    size_t found;
    found = foutput.find_last_of("/\\");
    string folder = foutput.substr(0, found);
    bool foutput_isdir = Args::isDir(folder);

    return finput_isdir && foutput_isdir;
}

bool Args::isDir(const string& path) {
    struct stat info;
    if (stat(path.c_str(), &info) == 0 && info.st_mode && S_IFDIR) return true;
    return false;
}
