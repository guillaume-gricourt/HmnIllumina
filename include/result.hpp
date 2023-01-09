#ifndef RESULT_INCLUDED
#define RESULT_INCLUDED

#include <map>
#include <string>

#include "result.hpp"
#include "sample.hpp"

class Result {
private:
  // Params
  std::map<std::string, std::string> params;
  // flowcell
  std::map<std::string, float> flowcell;
  // Read
  std::map<int, std::map<std::string, float>> read;
  // lane
  std::map<int, std::map<std::string, float>> lane;
  // sample
  std::vector<Sample> samples;

public:
  // Accessors
  std::map<std::string, std::string> const &getParams() const;
  std::map<std::string, float> const &getFlowcell() const;
  std::map<int, std::map<std::string, float>> const &getRead() const;
  std::map<int, std::map<std::string, float>> const &getLane() const;
  std::vector<Sample> getSamples() const;

  void setParams(const std::string &, const std::string &);
  void setFlowcell(const std::string &, const float &);
  void setRead(const int &, const std::map<std::string, float> &);
  void setLane(const int &, const std::map<std::string, float> &);
  void setSamples(const std::vector<Sample> &);

  std::string serializeJson() const;
};

#endif // RESULT_INCLUDED
