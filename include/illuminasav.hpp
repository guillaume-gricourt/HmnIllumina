// Copyright 2022 guillaume-gricourt
#ifndef INCLUDE_ILLUMINASAV_HPP_
#define INCLUDE_ILLUMINASAV_HPP_

#include <string>
#include <vector>

#include "args.hpp"
#include "interop/util/xml_parser.h"
#include "result.hpp"

class IlluminaSav {
private:
  Args &args;
  Result result;
  size_t thread_count;

public:
  explicit IlluminaSav(Args &);

  void read_file_parameters(const std::string &) INTEROP_THROW_SPEC(
      (xml::xml_file_not_found_exception, xml::bad_xml_format_exception,
       xml::empty_xml_format_exception, xml::missing_xml_element_exception,
       xml::xml_parse_exception));

  int showParams();
  int showMetrics();
  int showSamples();
  static float getMean(std::vector<float> &);
  int writeOutput() const;
};

#endif // INCLUDE_ILLUMINASAV_HPP_
