#pragma once

#include <cassert>
#include <map>
#include <vector>

#include "dvfsunit.h"

class DVFSSystem {
   public:
      
      /**
       * Creates a view of the DVFS control on the system.
       */
      DVFSSystem();

      /**
       * Destroy the DVFS controller and restores OS-driven DVFS control.
       */
      ~DVFSSystem();

      /**
       * Returns the number of DVFS units on the system. A DVFS unit is a set
       * of processor cores that must work using the same frequency.
       *
       * @return The number of DVFS units on the system. 
       */
      inline unsigned int getNbDVFSUnits() const {
         return units_.size();
      }

      /**
       * Returns the DVFS unit with the given id. A DVFS id is a number between
       * 0 and the number of DVFS units - 1.
       *
       * @param id The DVFS unit id.
       */
      inline DVFSUnit &getDVFSUnit(unsigned int id) {
         assert(id < units_.size());

         return *units_[id];
      }

      /**
       * Returns the unit to which a core belongs.
       *
       * @param coreId is The system id of the core whose unit is searched for.
       */
      inline DVFSUnit &getCoreDVFSUnit(unsigned int id) {
         assert(core2unit_.find(id) != core2unit_.end());

         return *core2unit_[id];
      }

   private:

      /** The set of DVFS units on the system */
      std::vector<DVFSUnit *> units_;

      /** Associates a unit to every core */
      std::map<unsigned int, DVFSUnit *> core2unit_;
};
