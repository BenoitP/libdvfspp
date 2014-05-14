#include "dvfs.h"
#include "utils.h"

#include <set>

DVFSSystem::DVFSSystem() {
   unsigned int nbCores = Utils::getNbCores();
   std::set<unsigned int> knownCores;

   // add one unit per set of cores on the same unit
   for (unsigned int c = 0; c < nbCores; ++c) {
      // already done
      if (knownCores.find(c) != knownCores.end()) {
         continue;
      }

      std::vector<unsigned int> related = Utils::getCoresOnSameUnit(c);
      knownCores.insert(related.begin(), related.end());

      // create the unit and store it
      DVFSUnit *newUnit = new DVFSUnit(related);
      units_.push_back(newUnit);

      for (unsigned int i = 0; i < related.size(); ++i) {
         core2unit_[related[i]] = newUnit;
      }
   }
}

DVFSSystem::~DVFSSystem() {
   for (unsigned int i = 0; i < units_.size(); ++i) {
      // also restores the automated control of DVFS
      delete units_[i];
   }
}
