#ifndef ROADSIGNS_HPP
#define ROADSIGNS_HPP

#include "BuggyState.hpp"


/** 
 * @brief A RoadSign structure containing necessarry information about a roadsign.
 *
 * Contains the ID of the sign, a name and an associated state.
 */
struct RoadSign {
  int id;             ///< The ID of the sign
  const char* name;   ///< The name of the sign
  BuggyState* state;  ///< The associated state of the sign
};


const RoadSign signs[] = {RoadSign{1, "LOOOOL", &JustDriveState::instance()}};


#endif //ROADSIGNS_HPP