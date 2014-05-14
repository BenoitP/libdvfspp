#pragma once

#include "dvfscapable.h"

#include <fstream>
#include <set>
#include <string>
#include <vector>

class DVFSUnit : public DVFSCapable {
   public:
      /**
       * Creates a DVFS unit from a set of core ids on the unit.
       */
      DVFSUnit(std::vector<unsigned int> cores);

      /**
       * Releases memory and restore automated control of the cores on the
       * unit.
       */
      ~DVFSUnit();

      /**
       * Sets the frequency to the given value. The frequency can be expressed
       * as a Hz value or as its order in the frequency list (0 being the
       * lowest available frequency).
       *
       * @param freq The frequency values (in kHz) or its relative rank in the
       *  list of frequencies. In case of conflict, @p freq is considered as
       *  being an absolute frequency value in kHz.
       */
      void setFrequency(unsigned int freq);

      /**
       * Temporarily stop controlling the frequency. The default controller is
       * temporarily re-activated. For instance ondemand is activated when
       * possible on Linux. The next call to setFrequency will restart the
       * frequency control.
       */
      void releaseControl();

   private:

      /**
       * Prevent copy
       */
      DVFSUnit(const DVFSUnit &);

      /**
       * Prevent assignment
       */
      DVFSUnit &operator=(const DVFSUnit &);

      /** Cores we are in charge of */
      std::vector<unsigned int> cores_;

      /** Frequency control file for every core */
      std::vector<std::ofstream *> freqFiles_;

      /** Available governors */
      std::set<std::string> availGovs_;

      /** True when using the userspace governor */
      bool hasControl_;

      /**
       * Sets the requested cpufreq governor on all the cores of the unit.
       *
       * @param gov The governor name.
       */
      void setGovernor(const std::string &gov);

};
