#pragma once

#include <vector>

class Utils {
   public:
      /**
       * Returns the number of available processor cores.
       *
       * @return The number of processor cores on the system.
       */
      static unsigned int getNbCores();

      /**
       * Returns the set of cores on the same unit as the core whose id is
       * given in parameter.
       *
       * @param id Id of the core for which the cores on the same unit are
       *  wanted.
       * @return The set of core ids for the cores on the same unit as @p id.
       */
      static std::vector<unsigned int> getCoresOnSameUnit(unsigned int id);
};
