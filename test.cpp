#include "src/libdvfs2.h"

#include <iostream>
#include <unistd.h>

int main(int argc, char **argv) {
   DVFSSystem dvfs;

   for (unsigned int i = 0; i < dvfs.getNbDVFSUnits(); ++i) {
      std::cout << "Unit " << i << std::endl;
      std::cout << "Available frequencies: ";
      const std::vector<unsigned int> &freqs = dvfs.getDVFSUnit(i).getFrequencies();

      for (unsigned int f = 0; f < freqs.size(); ++f) {
         std::cout << freqs[f] << ", ";
      }
      std::cout << std::endl;

      std::cout << "Setting average frequency: " << freqs.size() / 2 << std::endl;
      dvfs.getDVFSUnit(i).setFrequency(freqs.size() / 2);

      sleep(2);

      std::cout << "Setting maximal frequency: " << freqs[freqs.size() - 1] << std::endl;
      dvfs.getDVFSUnit(i).setFrequency(freqs[freqs.size() - 1]);

      sleep(2);
   }

   return 0;
}
