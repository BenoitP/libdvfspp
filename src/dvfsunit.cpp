#include "dvfsunit.h"

#include <algorithm>
#include <cerrno>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>

DVFSUnit::DVFSUnit(std::vector<unsigned int> cores) :
   cores_(cores), hasControl_(false)
{
   // open the frequency setter files
   for (unsigned int i = 0; i < cores_.size(); ++i) {
      std::stringstream ss;

      ss << "/sys/devices/system/cpu/cpu" << cores_[i]
         << "/cpufreq/scaling_setspeed";

      std::ofstream *fd = new std::ofstream(ss.str().c_str());
   
      if (!*fd) {
         std::cerr << "Cannot open " << ss.str() << " for writing: "
            << strerror(errno) << std::endl;
         break;
      }

      freqFiles_.push_back(fd);
   }

   // read the available frequencies on the first core
   if (cores.size() > 0) {
      std::stringstream ss;
      ss << "/sys/devices/system/cpu/cpu" << cores[0]
         << "/cpufreq/scaling_available_frequencies";

      std::ifstream ifd(ss.str().c_str());

      while (ifd) {
         unsigned int freq;

         ifd >> freq;
         if (ifd.fail()) {
            break;
         }
         
         frequencies_.push_back(freq);
      }
      ifd.close();

      std::sort(frequencies_.begin(), frequencies_.end());

      // also read the available governors
      ss.str("");
      ss << "/sys/devices/system/cpu/cpu" << cores[0]
         << "/cpufreq/scaling_available_governors";

      ifd.open(ss.str().c_str());

      while (ifd) {
         std::string gov;

         ifd >> gov;

         if (ifd.fail()) {
            break;
         }

         availGovs_.insert(gov);
      }
   }
}

DVFSUnit::~DVFSUnit() {
   // close the frequency files
   for (unsigned int i = 0; i < freqFiles_.size(); ++i) {
      delete freqFiles_[i];
   }
   
   // set ondemand before quitting
   if (hasControl_) {
      releaseControl();
   }
}

void DVFSUnit::setFrequency(unsigned int freq) {
   if (freq < frequencies_[0]) {
      freq = frequencies_[freq];
   }

   if (!hasControl_) {
      setGovernor("userspace");
   }

   for (unsigned int i = 0; i < freqFiles_.size(); ++i) {
      std::cout << "core " << i << " setting freq " << freq << std::endl;

      *(freqFiles_[i]) << freq;

      if (freqFiles_[i]->fail()) {
         std::cerr << "Failed to write in frequency setter for core "
            << cores_[i] << ": " << strerror(errno) << std::endl;
         return;
      }
      
      freqFiles_[i]->flush();
   }
}

void DVFSUnit::releaseControl() {
   setGovernor("ondemand");
}

void DVFSUnit::setGovernor(const std::string &gov) {
   if (availGovs_.find(gov) == availGovs_.end()) {
      std::cerr << "Requested governor " << gov << " is not available"
         << std::endl;
      return;
   }

   for (unsigned int i = 0; i < cores_.size(); ++i) {
      std::stringstream ss;

      ss << "/sys/devices/system/cpu/cpu" << cores_[i]
         << "/cpufreq/scaling_governor";

      std::ofstream fd(ss.str().c_str());

      if (!fd) {
         std::cerr << "Cannot open " << ss.str() << " for writing: "
            << strerror(errno) << std::endl;
         return;
      }

      fd << gov;

      fd.flush();
      fd.close();
   }

   hasControl_ = gov == std::string("userspace");
}

