#pragma once

#include <vector>

/**
 * Abstract class to represent a DVFS-capable entity such as a socket or a
 * processor core.
 */
class DVFSCapable {
   public:
      
      /**
       * Never call that destructor.
       */
      virtual ~DVFSCapable() {}

      /**
       * Returns the list of available frequencies. The list is ensured to be
       * sorted by increasing frequency.
       *
       * @return The list of available frequencies (in kHz).
       */
      virtual const std::vector<unsigned int> &getFrequencies() const {
         return frequencies_;
      }

      /**
       * Sets the frequency to the given value. The frequency can be expressed
       * as a Hz value or as its order in the frequency list (0 being the
       * lowest available frequency).
       *
       * @param freq The frequency values (in kHz) or its relative rank in the
       *  list of frequencies. In case of conflict, @p freq is considered as
       *  being an absolute frequency value in kHz.
       */
      virtual void setFrequency(unsigned int freq) = 0;

      /**
       * Returns the last frequency set.
       * 
       * @return The last frequency set (in kHz).
       */
      unsigned int getFrequency() const {
         return frequencies_[lastFreqId_];
      }

      /**
       * Returns the id of the last frequency set.
       *
       * @return The position of the last frequency set in the list of
       *  sorted frequencies. 0 is the id of the lowest frequency.
       */
      inline unsigned int getFrequencyId() const {
         return lastFreqId_;
      }

      /**
       * Temporarily stop controlling the frequency. The default controller is
       * temporarily re-activated. For instance ondemand is activated when
       * possible on Linux. The next call to setFrequency will restart the
       * frequency control.
       */
      virtual void releaseControl() = 0;

   protected:

      /** List of available frequencies. Must be increasingly sorted. */
      std::vector<unsigned int> frequencies_;

      /** Id of the last frequency set */
      unsigned int lastFreqId_;
};
