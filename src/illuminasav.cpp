// Copyright 2022 guillaume-gricourt
#include "illuminasav.hpp"

#include <math.h>
#include <stdio.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "args.hpp"
#include "interop/logic/summary/index_summary.h" //Index run metrics
#include "interop/logic/summary/run_summary.h"   //Summarize run metrics
#include "interop/logic/utils/metrics_to_load.h" //Summarize run metrics
#include "interop/model/run/info.h"              //For Params
#include "interop/model/run_metrics.h"           //Run metrics
#include "interop/model/summary/run_summary.h"   //Run metrics
#include "interop/util/filesystem.h"
#include "interop/util/xml_parser.h" //Parse xml file
#include "result.hpp"
#include "sample.hpp"

IlluminaSav::IlluminaSav(Args &arg) : args(arg), result(), thread_count(1) {}

int IlluminaSav::showParams() {
  /*******************
  ** Params Summary **
  *******************/
  illumina::interop::model::run::info run_info;

  run_info.read(args.getInput());

  result.setParams("run_name", run_info.name());
  result.setParams("flowcell", run_info.flowcell_id());
  result.setParams("date", run_info.date());
  result.setParams("instrument_name", run_info.instrument_name());
  result.setParams("paired", (run_info.is_paired_end()) ? "true" : "false");

  std::stringstream ss;
  ss << run_info.total_cycles();
  result.setParams("cycle_total", ss.str());
  ss.clear();

  /**********************
   * Parsing Parameters *
   * *******************/
  read_file_parameters(args.getInput());

  return 0;
}

void IlluminaSav::read_file_parameters(const std::string &run_folder)
    INTEROP_THROW_SPEC((illumina::interop::xml::xml_file_not_found_exception,
                        illumina::interop::xml::bad_xml_format_exception,
                        illumina::interop::xml::empty_xml_format_exception,
                        illumina::interop::xml::missing_xml_element_exception,
                        illumina::interop::xml::xml_parse_exception)) {
  std::string filename = "";
  if (run_folder.find(illumina::interop::io::paths::run_parameters()) !=
          std::string::npos ||
      run_folder.find(illumina::interop::io::paths::run_parameters(true)) !=
          std::string::npos) {
    filename = run_folder;
  } else {
    filename = illumina::interop::io::paths::run_parameters(run_folder, true);
    if (!illumina::interop::io::is_file_readable(filename)) {
      filename = illumina::interop::io::paths::run_parameters(run_folder);
    }
  }
  /**************
   * Parsing xml *
   * ************/
  // open xml file
  rapidxml::xml_document<> doc;
  try {
    rapidxml::file<> xml_file(filename.c_str());
    doc.parse<0>(xml_file.data());
  } catch (const std::runtime_error &ex) {
    INTEROP_THROW(illumina::interop::xml::xml_file_not_found_exception,
                  ex.what());
  } catch (const rapidxml::parse_error &ex) {
    INTEROP_THROW(illumina::interop::xml::xml_parse_exception, ex.what());
  }

  // get root node
  illumina::interop::xml::xml_node_ptr p_root = doc.first_node();

  if (p_root == 0)
    INTEROP_THROW(illumina::interop::xml::empty_xml_format_exception,
                  "Root node not found");
  /*
  if (p_root->name() != string("RunParameters")) {
      INTEROP_THROW(bad_xml_format_exception,
                    "Invalid run parameters xml file");
  }
  */
  /************
   * Get item *
   * *********/

  illumina::interop::xml::xml_node_ptr p_get = nullptr;

  // Chemistry
  std::string chemistry = "None";
  try {
    p_get = p_root->first_node("Chemistry");
    illumina::interop::xml::set_data(p_get, "Chemistry", chemistry);
  } catch (const illumina::interop::xml::missing_xml_element_exception &ex) {
  }
  result.setParams("chemistry", chemistry);

  // Reagent Version
  std::string reagentkitversion = "None";
  try {
    p_get = p_root->first_node("ReagentKitVersion");
    illumina::interop::xml::set_data(p_get, "ReagentKitVersion",
                                     reagentkitversion);
  } catch (const illumina::interop::xml::missing_xml_element_exception &ex) {
  }
  result.setParams("reagent_kit_version", reagentkitversion);

  // Cycles number
  std::string application_name = "None";
  std::string cycles_forward = "None";
  std::string cycles_reverse = "None";
  try {
    p_get = p_root->first_node("Setup");
    p_get = p_get->first_node("ApplicationName");
    illumina::interop::xml::set_data(p_get, "ApplicationName",
                                     application_name);
    std::transform(application_name.begin(), application_name.end(),
                   application_name.begin(), ::tolower);

    if (application_name.rfind("nextseq", 0) == 0) {
      p_get = p_root->first_node("Setup");
      p_get = p_get->first_node("Read1");
      illumina::interop::xml::set_data(p_get, "Read1", cycles_forward);

      p_get = p_root->first_node("Setup");
      p_get = p_get->first_node("Read2");
      illumina::interop::xml::set_data(p_get, "Read2", cycles_reverse);
    } else if (application_name.rfind("miseq", 0) == 0) {
      std::string num_cycles = "";
      std::string number = "";
      p_get = p_root->first_node("Reads");
      for (auto info_reads = p_get->first_node(); info_reads;
           info_reads = info_reads->next_sibling()) {
        for (auto attr = info_reads->first_attribute(); attr;
             attr = attr->next_attribute()) {
          if (strcmp(attr->name(), "Number") == 0) {
            number = attr->value();
          } else if (std::strcmp(attr->name(), "NumCycles") == 0) {
            num_cycles = attr->value();
          } else if (std::strcmp(attr->name(), "IsIndexedRead") == 0) {
            if (strcmp(attr->value(), "N") == 0 && number == "1") {
              cycles_forward = num_cycles;
            } else if (std::strcmp(attr->value(), "N") == 0) {
              cycles_reverse = num_cycles;
            }
          }
        }
      }
    }
  } catch (const illumina::interop::xml::missing_xml_element_exception &ex) {
  }

  result.setParams("cycles_reverse", cycles_reverse);
  result.setParams("cycles_forward", cycles_forward);
}

int IlluminaSav::showMetrics() {
  std::vector<unsigned char> valid_to_load;

  /********************
  ** Metrics Summary **
  ********************/
  illumina::interop::model::metrics::run_metrics run;
  illumina::interop::model::summary::run_summary summary;

  illumina::interop::logic::utils::list_summary_metrics_to_load(valid_to_load);

  run.read(args.getInput(), valid_to_load, thread_count);

  try {
    illumina::interop::logic::summary::summarize_run_metrics(run, summary);
  } catch (const std::exception &ex) {
    std::cerr << ex.what() << std::endl;
    return 1;
  }

  /*** Lane ***/
  // TODO(guillaume-gricourt): Mean Qual
  for (size_t read = 0; read < summary.size(); ++read) {
    if (summary[read].read().is_index()) {
      continue;
    }
    for (size_t lane = 0; lane < summary.lane_count(); ++lane) {
      if (summary[read][lane].tile_count() == 0) {
        continue;
      }

      const illumina::interop::model::summary::lane_summary &lane_sum =
          summary[read][lane];
      std::map<std::string, float> lane_values;
      lane_values["cluster_raw"] =
          lane_sum.reads(); //.reads() == cluster_count().mean()
      lane_values["cluster_pf"] = lane_sum.reads_pf();
      lane_values["cluster_percent_pf"] =
          lane_sum.reads_pf() * 100 / lane_sum.reads();
      lane_values["cluster_density"] = lane_sum.density().mean();
      lane_values["q_30"] = lane_sum.percent_gt_q30();

      result.setLane(static_cast<int>(lane), lane_values);
    }

    /**** Reads ****/
    std::map<std::string, float> read_values;
    read_values["q_30"] = summary[read].summary().percent_gt_q30();
    result.setRead(static_cast<int>(read), read_values);
  }

  /*** Flowcell ***/
  const illumina::interop::model::summary::metric_summary &metric_sum =
      summary.total_summary();
  result.setFlowcell("yield", metric_sum.yield_g());
  result.setFlowcell("q_30", metric_sum.percent_gt_q30());
  result.setFlowcell("error_rate", metric_sum.error_rate());

  // Calcul
  std::vector<float> vec_cluster_raw;
  std::vector<float> vec_cluster_pf;
  std::vector<float> vec_cluster_density;

  for (std::map<int, std::map<std::string, float>>::const_iterator it =
           result.getLane().begin();
       it != result.getLane().end(); ++it) {
    vec_cluster_raw.push_back(it->second.at("cluster_raw"));
    vec_cluster_pf.push_back(it->second.at("cluster_pf"));
    vec_cluster_density.push_back(it->second.at("cluster_density"));
  }

  result.setFlowcell(
      "cluster_raw",
      std::accumulate(vec_cluster_raw.begin(), vec_cluster_raw.end(), 0.0f));
  result.setFlowcell("cluster_pf", std::accumulate(vec_cluster_pf.begin(),
                                                   vec_cluster_pf.end(), 0.0f));
  result.setFlowcell("cluster_density",
                     IlluminaSav::getMean(vec_cluster_density));

  return 0;
}

int IlluminaSav::showSamples() {
  std::vector<unsigned char> valid_to_load;

  /******************
  ** Index Summary **
  ******************/
  illumina::interop::model::metrics::run_metrics run;
  illumina::interop::model::summary::index_flowcell_summary summary_index;
  illumina::interop::logic::utils::list_index_metrics_to_load(valid_to_load);

  run.read(args.getInput(), valid_to_load, thread_count);

  try {
    illumina::interop::logic::summary::summarize_index_metrics(run,
                                                               summary_index);
  } catch (const std::exception &ex) {
    std::cerr << ex.what() << std::endl;
    return 1;
  }

  std::map<size_t, Sample> samples;

  for (size_t lane = 0; lane < summary_index.size(); ++lane) {
    const illumina::interop::model::summary::index_lane_summary
        &index_lane_sum = summary_index[lane];
    for (size_t index = 0; index < index_lane_sum.size(); ++index) {
      const illumina::interop::model::summary::index_count_summary
          &index_count_sum = index_lane_sum[index];

      size_t sample_id = index_count_sum.id();

      if (samples.count(sample_id) == 0) {
        Sample sample(
            index_count_sum.id(), index_count_sum.index1(),
            index_count_sum.index2(), index_count_sum.fraction_mapped(),
            index_count_sum.cluster_count(), index_count_sum.sample_id(),
            index_count_sum.project_name());
        samples.emplace(sample_id, sample);
      } else {
        Sample &sample = samples[sample_id];
        sample.updateFractionMapped(index_count_sum.fraction_mapped());
        sample.updateClusterCount(index_count_sum.cluster_count());
      }
    }
  }

  std::vector<Sample> vsamples;
  for (std::map<size_t, Sample>::const_iterator it = samples.begin();
       it != samples.end(); ++it) {
    vsamples.push_back(it->second);
  }
  result.setSamples(vsamples);
  return 0;
}

float IlluminaSav::getMean(std::vector<float> &vec) {
  return std::accumulate(vec.begin(), vec.end(), 0.0f) / vec.size();
}

int IlluminaSav::writeOutput() const {
  std::string dico = result.serializeJson();

  std::ofstream myfile;
  myfile.open(args.getOutput().c_str());
  myfile << dico;
  myfile.close();

  return 0;
}
