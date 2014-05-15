#include "utils.h"

#include <cassert>
#include <cpuid.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>

unsigned int Utils::getNbCores() {
   unsigned int nbCores = 0;
   
   nbCores = sysconf(_SC_NPROCESSORS_ONLN);
   if (nbCores >= 1) {
      return nbCores;
   }

   // Second try when the sysconf fails
   std::ifstream cpuinfo("/proc/cpuinfo");

   if (!cpuinfo) {
      return 0;
   }
      
   std::string buf;
   while (cpuinfo) {
      std::getline(cpuinfo, buf);
      if (buf.compare(0, 9, std::string("processor")) == 0) {
         nbCores++;
      }
   }

   return nbCores;
}

std::vector<unsigned int> Utils::getCoresOnSameUnit(unsigned int id) {
   unsigned int cpuid[4];
   std::stringstream ss;
   std::ifstream ifd;
   std::vector<unsigned int> res;

   // all Linux files are broken in some versions... first rely on the manufacturer
   __get_cpuid(0, cpuid + 3, cpuid, cpuid + 2, cpuid + 1);
   cpuid[3] = 0;

   // Intel platforms have a single frequency domain
   if (std::string((char *) cpuid) == std::string("GenuineIntel")) {
      ss << "/sys/devices/system/cpu/cpu" << id 
         << "/topology/core_siblings_list";
      ifd.open(ss.str().c_str());
   } else {
      // prefer the more recent freq_domain_cpus over related_cpus
      ss << "/sys/devices/system/cpu/cpu" << id 
         << "/cpufreq/freqdomain_cpus";
      ifd.open(ss.str().c_str());
      if (!ifd) {
         ss.clear();
         ss << "/sys/devices/system/cpu/cpu" << id
            << "/cpufreq/related_cpus";
         ifd.open(ss.str().c_str());
      }
   }

   if (!ifd) {
      std::cerr << "Cannot read related core files, aborting" << std::endl;
      return res;
   }

   // parse the file
   // supports space-separated list of values and condensed format 
   // (comma-separated list with dash notation for contiguous lists)
   while (!ifd.eof()) {
      unsigned int val;
      char sep;

      if (ifd.peek() == '\n') {
         break;
      }

      ifd >> val;
      if (ifd.fail()) {
         std::cerr << "Invalid file format for file " << ss.str();
         return std::vector<unsigned int>();
      }

      ifd.get(sep);
      if (sep == ' ' || sep == ',' || sep == '\n' || sep == '\t') {
         res.push_back(val);
         continue;
      }

      // if it's not a space or a comma, we expect a dash
      assert(sep == '-');

      unsigned int lastVal;
      ifd >> lastVal;
      if (ifd.fail()) {
         std::cerr << "Invalid file format for file " << ss.str();
         return std::vector<unsigned int>();
      }

      for (unsigned int i = val; i <= lastVal; ++i) {
         res.push_back(i);
      }

      if (ifd.peek() == ',') {
         ifd.get(sep);
      }
   }

   return res;
}
