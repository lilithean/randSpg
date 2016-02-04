/**********************************************************************
  spgInit.h - Header file for spacegroup initialization functions

  Copyright (C) 2015 - 2016 by Patrick S. Avery

  This source code is released under the New BSD License, (the "License").

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

 ***********************************************************************/

#ifndef SPG_INIT_H
#define SPG_INIT_H

#include <vector>
#include <tuple>
#include <utility>

#include "crystal.h"

// wyckPos is a tuple of a char (representing the Wyckoff letter),
// an int (representing the multiplicity), and a string (that contains the first
// Wyckoff position)
typedef std::tuple<char, int, std::string> wyckPos;

// Each spacegroup has a variable number of wyckoff positions. So each
// spacegroup will have it's own vector of wyckoff positions.
typedef std::vector<wyckPos> wyckoffPositions;

// This assign an atom with a specific atomic number to be placed in a specific
// wyckoff position
// 'uint' is the atomic number
typedef std::pair<wyckPos, uint> atomAssignment;
// This is a vector of atom assignments
typedef std::vector<atomAssignment> atomAssignments;

// number of atoms and atomic number
typedef std::pair<uint, uint> numAndType;

typedef std::pair<std::string, std::string> fillCellInfo;

namespace XtalOpt {
  struct XtalCompositionStruct;
}

class SpgInit {
 public:

  static char getWyckLet(const wyckPos& pos) {return std::get<0>(pos);};
  static uint getMultiplicity(const wyckPos& pos) {return std::get<1>(pos);};
  static std::string getWyckCoords(const wyckPos& pos) {return std::get<2>(pos);};

  /*
   * Obtain the wyckoff positions of a spacegroup from the database
   *
   * @param spg The spacegroup from which to obtain the wyckoff positions
   *
   * @return Returns a constant reference to a vector of wyckoff positions
   * for the spacegroup from the database in wyckoffDatabase.h. Returns an
   * empty vector if an invalid spg is entered.
   */
  static const wyckoffPositions& getWyckoffPositions(uint spg);

  static const fillCellInfo& getFillCellInfo(uint spg);

  static std::vector<std::string> getVectorOfDuplications(uint spg);

  static std::vector<std::string> getVectorOfFillPositions(uint spg);

  static double interpretComponent(const std::string& component,
                                          double x, double y, double z);

  /*
   * Used to determine if a spacegroup is possible for a given set of atoms.
   * It is determined by using the multiplicities in the Wyckoff database.
   *
   * @param spg The spacegroup to check.
   * @param atomTypes A vector of atomic numbers (one for each atom). So if
   *                  our system were Ti1O2, it should be {22, 8, 8}
   *
   * @return True if the spacegroup may be generated. False if it cannot.
   *
   */
  static bool isSpgPossible(uint spg, const std::vector<uint>& atomTypes);

  /*
   * Generates a latticeStruct (contains a, b, c, alpha, beta, and gamma as
   * doubles) with randomly generated parameters for a given
   * spacegroup, mins, and maxes. If a parameter must be constrained due
   * to the spacegroup, it will be.
   *
   * @param spg The spacegroup for which to generate a lattice.
   * @param mins The minimum values for the lattice parameters
   * @param maxes The maximum values for the lattice parameters
   *
   * @return Returns the lattice struct that was generated. Returns a struct
   * with all zero values if a proper lattice struct cannot be generated
   * for a specific spacegroup due to invalid mins or maxes.
   * An error message will be printed to stdout with information if it fails.
   */
  static latticeStruct generateLatticeForSpg(uint spg,
                                             const latticeStruct& mins,
                                             const latticeStruct& maxes);
  /*
   * Attempts to add an atom randomly to a wyckoff position of a given crystal.
   * The position of the atom is constrained by the given wyckoff position.
   * It will attempt to add an atom randomly to satisfy minimum IAD for
   * maxAttempts times, and if it fails, it returns false. If a fixed wyckoff
   * position is given, it will just add the atom to the fixed wyckoff position.
   * After it adds a Wyckoff atom, it attempts to fill the cell with it
   * according to the spacegroup. If it fails, it will remove the new Wyckoff
   * atom and try again.
   *
   * @param crystal The Crystal object for which an atom will be added
   * @param position The wyckoff position to add the atom to
   * @param atomicNum The atomic number of the atom to be added
   * @param spg The spacegroup which we are creating.
   * @param maxAttempts The number of attempts to make to add the atom randomly
   *                    before the function returns false. Default is 1000.
   *
   * @return True if it succeeded, and false if it failed.
   */
  static bool addWyckoffAtomRandomly(Crystal& crystal, wyckPos& position,
                                     uint atomicNum, uint spg,
                                     int maxAttempts = 1000);

  /*
   * Initialze and return a Crystal object with a given spacegroup!
   * The lattice mins and lattice maxes provide constraints for the lattice
   * to be generated. The list of atom types tell it which atoms to be added.
   * Returns a zero-volume Crystal object if it failed to generate it.
   *
   * @param spg The international number for the spacegroup to be generated
   * @param atomTypes A vector of atomic numbers (one for each atom). So if
   *                  our system were Ti1O2, it should be {22, 8, 8}
   * @param latticeMins A latticeStruct that contains the minima for a, b, c,
   *                    alpha, beta, and gamma.
   * @param latticeMaxes A latticeStruct that contains the maxima for a, b, c,
   *                     alpha, beta, and gamma.
   * @param minIADScalingFactor A scaling factor used to scale the minIAD
   * @param numAttempts The max number number of attempts to generate a crystal
   *                    given these conditions. It will still only find all
   *                    combinations once (that's the most time consuming
   *                    step). It will randomly pick combinations for every
   *                    subsequent attempt from the combinations it found
   *                    originally.
   *
   * @return A Crystal object with the given spacegroup, atoms,
   * and lattice within the provided lattice constraints. Returns a Crystal
   * with zero volume if it failed to generate one successfully.
   */
  static Crystal spgInitCrystal(uint spg,
                                const std::vector<uint>& atomTypes,
                                const latticeStruct& latticeMins,
                                const latticeStruct& latticeMaxes,
                                double minIADScalingFactor = 0.5,
                                int numAttempts = 100);

  static std::vector<numAndType> getNumOfEachType(
                                   const std::vector<uint>& atoms);

  static bool containsUniquePosition(const wyckPos& pos);

  static void printAtomAssignments(const atomAssignments& a);

};

#endif