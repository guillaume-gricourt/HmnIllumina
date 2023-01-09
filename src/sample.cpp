// Copyright 2022 guillaume-gricourt
#include "sample.hpp"

#include <numeric>
#include <string>
#include <vector>

Sample::Sample(size_t i, std::string i1, std::string i2, float fm, uint64_t cc,
               std::string si, std::string pn)
    : id(i), index1(i1), index2(i2), fraction_mapped(1, fm), cluster_count(cc),
      sample_id(si), project_name(pn) {}
Sample::Sample() {}

void Sample::setId(size_t i) { id = i; }
void Sample::setIndex1(const std::string &n) { index1 = n; }
void Sample::setIndex2(const std::string &n) { index2 = n; }
void Sample::setFractionMapped(float i) {
  fraction_mapped.clear();
  fraction_mapped.push_back(i);
}
void Sample::setClusterCount(uint64_t i) { cluster_count = i; }
void Sample::setSampleId(const std::string &n) { sample_id = n; }
void Sample::setProjectName(const std::string &n) { project_name = n; }

size_t Sample::getId() const { return id; }
std::string Sample::getIndex1() const { return index1; }
std::string Sample::getIndex2() const { return index2; }
// Calculate mean for all lanes
float Sample::getFractionMapped() const {
  return accumulate(fraction_mapped.begin(), fraction_mapped.end(), 0.0f) /
         static_cast<int>(fraction_mapped.size());
}
uint64_t Sample::getClusterCount() const { return cluster_count; }
std::string Sample::getSampleId() const { return sample_id; }
std::string Sample::getProjectName() const { return project_name; }
void Sample::updateFractionMapped(float i) { fraction_mapped.push_back(i); }
void Sample::updateClusterCount(uint64_t i) { cluster_count += i; }
