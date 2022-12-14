// Copyright 2022 guillaume-gricourt
#include "args.hpp"

#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <iostream>
#include <string>

void Args::setInput(const std::string &fid) { finput = fid; }
void Args::setOutput(const std::string &fid) {
  char abs_output[PATHMAX];
  realpath(fid.c_str(), abs_output);
  foutput = abs_output;
}

std::string Args::getInput() const { return finput; }
std::string Args::getOutput() const { return foutput; }

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
  size_t found = foutput.find_last_of("/\\");
  std::string folder = foutput.substr(0, found);
  bool foutput_isdir = Args::isDir(folder);

  return finput_isdir && foutput_isdir;
}

bool Args::isDir(const std::string &path) {
  struct stat info;
  if (stat(path.c_str(), &info) != 0)
    return false;
  else if (info.st_mode & S_IFDIR)
    return true;
  return false;
}
