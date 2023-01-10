// Copyright 2022 guillaume-gricourt
#ifndef INCLUDE_ARGS_HPP_
#define INCLUDE_ARGS_HPP_

#include <string>

#define PATHMAX 256

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

#endif // INCLUDE_ARGS_HPP_
