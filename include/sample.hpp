#ifndef SAMPLE_INCLUDED
#define SAMPLE_INCLUDED

#include <string>
#include <vector>

class Sample {
private:
  size_t id;
  std::string index1;
  std::string index2;
  std::vector<float> fraction_mapped;
  uint64_t cluster_count;
  std::string sample_id;
  std::string project_name;

public:
  Sample(size_t, std::string, std::string, float, uint64_t, std::string,
         std::string);
  Sample();
  ~Sample() = default;
  Sample(Sample const &) = default;
  // Getters Setters
  void setId(size_t);
  void setIndex1(const std::string &);
  void setIndex2(const std::string &);
  void setFractionMapped(float);
  void setClusterCount(uint64_t);
  void setSampleId(const std::string &);
  void setProjectName(const std::string &);

  size_t getId() const;
  std::string getIndex1() const;
  std::string getIndex2() const;
  float getFractionMapped() const;
  uint64_t getClusterCount() const;
  std::string getSampleId() const;
  std::string getProjectName() const;

  void updateFractionMapped(float);
  void updateClusterCount(uint64_t);
};

#endif // ARGS_INCLUDED
