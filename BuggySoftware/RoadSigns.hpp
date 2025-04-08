#ifndef ROADSIGNS_HPP
#define ROADSIGNS_HPP

#include "BuggyState.hpp"
#include "Camera.hpp"
#include "Buggy.hpp"


class BuggyState;
class Buggy;
class RoadSignRecognition;


/** 
 * @brief A RoadSign structure containing necessarry information about a roadsign.
 *
 * Contains the ID of the sign, a name and an associated state.
 */
struct RoadSign {
  int id;               ///< The ID of the sign
  const char *name;     ///< The name of the sign
  void (*callback)(RoadSignRecognition& signRecognition, Buggy* buggy);   ///< The associated callback function of the sign
};



/** 
 * @brief The IdentifiedRoadSign structure holds the information about a sign that was recognized.
 *
 * Contains the RoadSign itself and some meta information to identify which sign gets priority.
 */
struct IdentifiedRoadSign {
  RoadSign sign;               ///< The observed sign
  unsigned long timestamp;     ///< The timestamp when it was recognized
  unsigned int size;           ///< The size of the sign in pix^2
  unsigned int estDistance;    ///< The estimated ditsance to the sign
};


class RoadSignRecognition {
private:
  Camera& camera;
  static const size_t maxObservedSigns = 3;
  IdentifiedRoadSign *observed_signs[maxObservedSigns];
  IdentifiedRoadSign *nextRoadSign;
  
  Buggy* buggy;
  bool initialized = false;

  void updateObservedSigns();
public:
  RoadSignRecognition(Camera& camera);

  void begin(Buggy* buggy);
  void clearSign();
  void update();
};

#endif //ROADSIGNS_HPP