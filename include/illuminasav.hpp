#ifndef ILLUMINASAV_INCLUDED
#define ILLUMINASAV_INCLUDED

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
  IlluminaSav(Args &);

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

#endif // ILLUMINASAV_INCLUDED
