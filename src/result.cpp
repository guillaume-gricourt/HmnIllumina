#include "result.hpp"

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/stringbuffer.h"
#include "sample.hpp"

using namespace rapidjson;
using namespace std;

/** Getters **/
map<string, string> const &Result::getParams() const { return params; }
map<string, float> const &Result::getFlowcell() const { return flowcell; }
map<int, map<string, float>> const &Result::getRead() const { return read; }
map<int, map<string, float>> const &Result::getLane() const { return lane; }
vector<Sample> Result::getSamples() const { return samples; }

/** Setters **/
void Result::setParams(const string &key, const string &value) {
  params[key] = value;
}
void Result::setFlowcell(const string &key, const float &value) {
  flowcell[key] = value;
}
void Result::setRead(const int &key, const map<string, float> &value) {
  read[key] = value;
}
void Result::setLane(const int &key, const map<string, float> &value) {
  lane[key] = value;
}
void Result::setSamples(const std::vector<Sample> &s) { samples = s; }

string Result::serializeJson() const {
  // Init
  Document document;

  document.SetObject();
  Document::AllocatorType &allocator = document.GetAllocator();

  // Params
  Value vparam(rapidjson::kObjectType);
  for (map<string, string>::const_iterator it = params.begin();
       it != params.end(); ++it) {
    Value key(it->first.c_str(), allocator);
    Value value(it->second.c_str(), allocator);
    vparam.AddMember(key, value, allocator);
  }
  document.AddMember("parameters", vparam, allocator);

  // Flowcell

  Value vflowcell(rapidjson::kObjectType);
  for (map<string, float>::const_iterator it = flowcell.begin();
       it != flowcell.end(); ++it) {
    Value key(it->first.c_str(), allocator);
    float fvalue = it->second;
    Value value(to_string(fvalue).c_str(), allocator);
    vflowcell.AddMember(key, value, allocator);
  }
  document.AddMember("flowcell", vflowcell, allocator);

  // Read
  Value vread(rapidjson::kObjectType);
  for (map<int, map<string, float>>::const_iterator it = read.begin();
       it != read.end(); ++it) {
    Value vread_values(rapidjson::kObjectType);
    for (map<string, float>::const_iterator it2 = it->second.begin();
         it2 != it->second.end(); ++it2) {
      Value key(it2->first.c_str(), allocator);
      float fvalue = it2->second;
      Value value(to_string(fvalue).c_str(), allocator);
      vread_values.AddMember(key, value, allocator);
    }
    int read_number = it->first;

    Value vread_number;

    const char *cstr = to_string(read_number).c_str();
    vread_number.SetString(cstr, allocator);

    vread.AddMember(vread_number, vread_values, allocator);
  }
  document.AddMember("read", vread, allocator);

  // Lane
  Value vlane(rapidjson::kObjectType);
  for (map<int, map<string, float>>::const_iterator it = lane.begin();
       it != lane.end(); ++it) {
    Value vlane_values(rapidjson::kObjectType);

    for (map<string, float>::const_iterator it2 = it->second.begin();
         it2 != it->second.end(); ++it2) {
      Value key(it2->first.c_str(), allocator);

      float fvalue = it2->second;
      Value value(to_string(fvalue).c_str(), allocator);
      vlane_values.AddMember(key, value, allocator);
    }

    int lane_number = it->first + 1;

    Value vlane_number;

    const char *cstr = to_string(lane_number).c_str();
    vlane_number.SetString(cstr, allocator);

    vlane.AddMember(vlane_number, vlane_values, allocator);
  }
  document.AddMember("lane", vlane, allocator);

  // Samples
  // Samples
  Value vsamples(rapidjson::kObjectType);
  for (auto &it : samples) {
    Value vsample(rapidjson::kObjectType);

    Value k_id("sample_id", allocator);
    Value v_id(to_string(it.getId()).c_str(), allocator);
    vsample.AddMember(k_id, v_id, allocator);

    Value k_ix1("index1", allocator);
    Value v_ix1(it.getIndex1().c_str(), allocator);
    vsample.AddMember(k_ix1, v_ix1, allocator);

    Value k_ix2("index2", allocator);
    Value v_ix2(it.getIndex2().c_str(), allocator);
    vsample.AddMember(k_ix2, v_ix2, allocator);

    Value k_fm("fraction_mapped", allocator);
    Value v_fm(to_string(it.getFractionMapped()).c_str(), allocator);
    vsample.AddMember(k_fm, v_fm, allocator);

    Value k_cc("cluster_count", allocator);
    Value v_cc(to_string(it.getClusterCount()).c_str(), allocator);
    vsample.AddMember(k_cc, v_cc, allocator);

    Value k_pn("project_name", allocator);
    Value v_pn(it.getProjectName().c_str(), allocator);
    vsample.AddMember(k_pn, v_pn, allocator);

    Value vsample_id;
    vsample_id.SetString(it.getSampleId().c_str(), allocator);

    vsamples.AddMember(vsample_id, vsample, allocator);
  }
  document.AddMember("samples", vsamples, allocator);

  // Write
  StringBuffer buffer;
  PrettyWriter<StringBuffer> writer(buffer);
  document.Accept(writer);

  string res = buffer.GetString();
  return res;
}
