#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "HUSKYLENS.h"
#include "Wire.h"


class Camera {
private:
  HUSKYLENS& huskylens;

  unsigned long last_request = 0, request_interval = 1000;

public:

  enum class CameraMode {
    FACE_RECOGNITION,
    OBJECT_TRACKING,
    OBJECT_RECOGNITION,
    LINE_TRACKING,
    COLOR_RECOGNITION,
    TAG_RECOGNITION
  };

  Camera(HUSKYLENS& huskylens);
  ~Camera() = default;

  // Delete copy constructor and operator to prevent copying
  Camera operator=(const Camera& other) = delete;
  Camera(const Camera& other) = delete;



  /** 
   * @brief starts communication with the HUSKY Lens. This operation blocks until communication is established.
   */
  void begin();

  /**
   * @brief called every loop to communicate with the camera.
   */
  void update();

  /**
   * @brief will return the biggest learned block that is currently in frame.
   *
   * This is useful when looking for roadsigns as the biggest one is probably cloasest.
   *
   * @param[out] result is contains the resulting block read by the HUSKYLENS
   *
   * @return if a block was recognized
   */
  boolean getBiggestBlock(HUSKYLENSResult* result);

  /**
   * @brief will return the biggest arrow that is currently in frame.
   *
   * This is useful when looking for the current driving line.
   *
   * @return the HUSKYLENSResult result arrow object
   */
  //HUSKYLENSResult getBiggestArrow();

  /**
   * @brief switch to the specified camera mode
   */
  void switchMode(CameraMode mode);

};



#endif //CAMERA_HPP