#include <getopt.h>

#include <iostream>
#include <string>

#include "args.hpp"
#include "illuminasav.hpp"
#include "result.hpp"
#include "version.hpp"

using namespace std;

const string usage() {
  string msg = "";
  msg += "Usage:   IlluminaSav [options]\n\n";
  msg += "Options:\n";
  msg += "    -h/--help    None    show this help and exit (optional)\n";
  msg += "        -v/--version    None      show the version and exit "
         "(optional)\n";
  msg += "        -i/--input    [FOLDER]    folder Interop (required)\n";
  msg += "        -o/--output    [FILE]    file output (required)\n";
  return msg;
}

int main(int argc, char **argv) {
  int c;
  Args args;
  bool showUsage(false);
  bool showVersion(false);
  // test if no args
  if (argc == 1)
    showUsage = true;

  while (!showUsage) {
    static struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {"version", no_argument, 0, 'v'},
        {"input", required_argument, 0, 'i'},
        {"output", required_argument, 0, 'o'},
        {0, 0, 0, 0}};
    /* getopt_long stores the option index here. */
    int option_index = 0;

    c = getopt_long(argc, argv, "i:o:hv", long_options, &option_index);

    /* Detect the end of the options. */
    if (c == -1)
      break; // end of the options

    switch (c) {
    case 'i':
      args.setInput(optarg);
      break;
    case 'o':
      args.setOutput(optarg);
      break;
    case 'h':
      showUsage = true;
      break;
    case 'v':
      showVersion = true;
      break;
    case '?':
      showUsage = true;
      break;
    default:
      showUsage = true;
    }
  }

  // Check if version
  if (showVersion) {
    cout << HMNILLUMINA_VERSION << endl;
    return 0;
  }

  // Check if ok
  // TODO Check if args valid
  if (showUsage) { //|| ! args.isValid()
    cout << usage() << endl;
  } else {
    IlluminaSav illsav(args);
    illsav.showParams();
    illsav.showMetrics();
    illsav.showSamples();
    illsav.writeOutput();
  }
  return 0;
}
