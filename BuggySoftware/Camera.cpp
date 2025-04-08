#include "Camera.hpp"
#include "EmbeddedLogger.hpp"


using EmbeddedLogger::logger;
using logLevel = EmbeddedLogger::LogLevel;


Camera::Camera(HUSKYLENS& huskylens) : huskylens(huskylens) {
}


void Camera::begin() {
  Wire.begin();
  while (!huskylens.begin(Wire)) {
    logger << logLevel::WARNING << "Begin failed!" << EmbeddedLogger::endl;
    logger << logLevel::WARNING << "1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>I2C)" << EmbeddedLogger::endl;
    logger << logLevel::WARNING << "2.Please recheck the connection." << EmbeddedLogger::endl;
    delay(100);
  }
}


void Camera::update() {
  if (this->last_request + this->request_interval > millis()) return;
  if (!huskylens.request()) {
    logger << logLevel::WARNING << "Fail to request data from HUSKYLENS, recheck the connection!" << EmbeddedLogger::endl;
  }
  this->last_request = millis();
}    
  
void Camera::switchMode(CameraMode mode) {
  switch (mode) {
    case CameraMode::FACE_RECOGNITION:
      this->huskylens.writeAlgorithm(ALGORITHM_FACE_RECOGNITION); break;
    case CameraMode::OBJECT_TRACKING:
      this->huskylens.writeAlgorithm(ALGORITHM_OBJECT_TRACKING); break;
    case CameraMode::OBJECT_RECOGNITION:
      this->huskylens.writeAlgorithm(ALGORITHM_OBJECT_RECOGNITION); break;
    case CameraMode::LINE_TRACKING:
      this->huskylens.writeAlgorithm(ALGORITHM_LINE_TRACKING); break;
    case CameraMode::COLOR_RECOGNITION:
      this->huskylens.writeAlgorithm(ALGORITHM_COLOR_RECOGNITION); break;
    case CameraMode::TAG_RECOGNITION:
      this->huskylens.writeAlgorithm(ALGORITHM_TAG_RECOGNITION); break;
  }
}


bool Camera::getBiggestBlock(HUSKYLENSResult *result) {
  int count, best_size, tmp_size;
  HUSKYLENSResult tmp;
  *result = HUSKYLENSResult{}; 

  count = this->huskylens.countBlocksLearned();
  logger << logLevel::DEBUG << "COUNT: " << count << EmbeddedLogger::endl;
  if (count < 1) return false;

  *result = this->huskylens.getBlockLearned(0);
  best_size = result->width * result->height;
  for (int i=1;i<count;++i) {
    tmp = this->huskylens.getBlockLearned(i);
    tmp_size = tmp.width * tmp.height;
    if (tmp_size > best_size) {
      *result = tmp;
      best_size = tmp_size;
    }
  }
  logger << logLevel::DEBUG << "Biggest block in frame: " << result->ID << EmbeddedLogger::endl;
  return true;
}
/*
HUSKYLENSResult Camera::getBiggestArrow() {
  int count, best_size;
  HUSKYLENSResult result, tmp;

  this->update();
  count = this->huskylens.countArrows();
  if (count < 1) return HUSKYLENSResult{};

  result = this->huskylens.getArrow(0);
  best_size = sqrt( (  ) );
  for (int i=1;i<count;++i) {
    tmp = getBlockLearned(i);
    size = tmp.width * tmp.height;
    if (size > best_size) result = tmp;
  }
  return result;
}

RoadSign Camera::readRoadSign() {
  
} */