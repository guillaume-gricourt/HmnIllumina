// Copyright 2022 guillaume-gricourt
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

/** Getters **/
std::map<std::string, std::string> const &Result::getParams() const {
  return params;
}
std::map<std::string, float> const &Result::getFlowcell() const {
  return flowcell;
}
std::map<int, std::map<std::string, float>> const &Result::getRead() const {
  return read;
}
std::map<int, std::map<std::string, float>> const &Result::getLane() const {
  return lane;
}
std::vector<Sample> Result::getSamples() const { return samples; }

/** Setters **/
void Result::setParams(const std::string &key, const std::string &value) {
  params[key] = value;
}
void Result::setFlowcell(const std::string &key, const float &value) {
  flowcell[key] = value;
}
void Result::setRead(const int &key,
                     const std::map<std::string, float> &value) {
  read[key] = value;
}
void Result::setLane(const int &key,
                     const std::map<std::string, float> &value) {
  lane[key] = value;
}
void Result::setSamples(const std::vector<Sample> &s) { samples = s; }

std::string Result::serializeJson() const {
  // Init
  rapidjson::Document document;

  document.SetObject();
  rapidjson::Document::AllocatorType &allocator = document.GetAllocator();

  // Params
  rapidjson::Value vparam(rapidjson::kObjectType);
  for (std::map<std::string, std::string>::const_iterator it = params.begin();
       it != params.end(); ++it) {
    rapidjson::Value key(it->first.c_str(), allocator);
    rapidjson::Value value(it->second.c_str(), allocator);
    vparam.AddMember(key, value, allocator);
  }
  document.AddMember("parameters", vparam, allocator);

  // Flowcell
  rapidjson::Value vflowcell(rapidjson::kObjectType);
  for (std::map<std::string, float>::const_iterator it = flowcell.begin();
       it != flowcell.end(); ++it) {
    rapidjson::Value key(it->first.c_str(), allocator);
    float fvalue = it->second;
    rapidjson::Value value(std::to_string(fvalue).c_str(), allocator);
    vflowcell.AddMember(key, value, allocator);
  }
  document.AddMember("flowcell", vflowcell, allocator);

  // Read
  rapidjson::Value vread(rapidjson::kObjectType);
  for (std::map<int, std::map<std::string, float>>::const_iterator it =
           read.begin();
       it != read.end(); ++it) {
    rapidjson::Value vread_values(rapidjson::kObjectType);
    for (std::map<std::string, float>::const_iterator it2 = it->second.begin();
         it2 != it->second.end(); ++it2) {
      rapidjson::Value key(it2->first.c_str(), allocator);
      float fvalue = it2->second;
      rapidjson::Value value(std::to_string(fvalue).c_str(), allocator);
      vread_values.AddMember(key, value, allocator);
    }
    int read_number = it->first;

    rapidjson::Value vread_number;

    const char *cstr = std::to_string(read_number).c_str();
    vread_number.SetString(cstr, allocator);

    vread.AddMember(vread_number, vread_values, allocator);
  }
  document.AddMember("read", vread, allocator);

  // Lane
  rapidjson::Value vlane(rapidjson::kObjectType);
  for (std::map<int, std::map<std::string, float>>::const_iterator it =
           lane.begin();
       it != lane.end(); ++it) {
    rapidjson::Value vlane_values(rapidjson::kObjectType);

    for (std::map<std::string, float>::const_iterator it2 = it->second.begin();
         it2 != it->second.end(); ++it2) {
      rapidjson::Value key(it2->first.c_str(), allocator);

      float fvalue = it2->second;
      rapidjson::Value value(std::to_string(fvalue).c_str(), allocator);
      vlane_values.AddMember(key, value, allocator);
    }

    int lane_number = it->first + 1;

    rapidjson::Value vlane_number;

    const char *cstr = std::to_string(lane_number).c_str();
    vlane_number.SetString(cstr, allocator);

    vlane.AddMember(vlane_number, vlane_values, allocator);
  }
  document.AddMember("lane", vlane, allocator);

  // Samples
  rapidjson::Value vsamples(rapidjson::kObjectType);
  for (auto &it : samples) {
    rapidjson::Value vsample(rapidjson::kObjectType);

    rapidjson::Value k_id("sample_id", allocator);
    rapidjson::Value v_id(std::to_string(it.getId()).c_str(), allocator);
    vsample.AddMember(k_id, v_id, allocator);

    rapidjson::Value k_ix1("index1", allocator);
    rapidjson::Value v_ix1(it.getIndex1().c_str(), allocator);
    vsample.AddMember(k_ix1, v_ix1, allocator);

    rapidjson::Value k_ix2("index2", allocator);
    rapidjson::Value v_ix2(it.getIndex2().c_str(), allocator);
    vsample.AddMember(k_ix2, v_ix2, allocator);

    rapidjson::Value k_fm("fraction_mapped", allocator);
    rapidjson::Value v_fm(std::to_string(it.getFractionMapped()).c_str(),
                          allocator);
    vsample.AddMember(k_fm, v_fm, allocator);

    rapidjson::Value k_cc("cluster_count", allocator);
    rapidjson::Value v_cc(std::to_string(it.getClusterCount()).c_str(),
                          allocator);
    vsample.AddMember(k_cc, v_cc, allocator);

    rapidjson::Value k_pn("project_name", allocator);
    rapidjson::Value v_pn(it.getProjectName().c_str(), allocator);
    vsample.AddMember(k_pn, v_pn, allocator);

    rapidjson::Value vsample_id;
    vsample_id.SetString(it.getSampleId().c_str(), allocator);

    vsamples.AddMember(vsample_id, vsample, allocator);
  }
  document.AddMember("samples", vsamples, allocator);

  // Write
  rapidjson::StringBuffer buffer;
  rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
  document.Accept(writer);

  std::string res = buffer.GetString();
  return res;
}
