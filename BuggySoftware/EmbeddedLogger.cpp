#include "EmbeddedLogger.hpp"

namespace EmbeddedLogger {

void Logger::flushBuffer() {
  if (pos > 0 && target) {
    target->write(buffer, pos);
    pos = 0;
  }
}

void Logger::appendToBuffer(const char* str) {
  while (*str && pos < BUFFER_SIZE - 1) {
      buffer[pos++] = *str++;
  }
}
void Logger::appendNumber(int32_t num) {
  char numBuffer[12];
  int len = 0;
  if (num < 0) {
      buffer[pos++] = '-';
      num = -num;
  }
  do {
      numBuffer[len++] = '0' + (num % 10);
      num /= 10;
  } while (num && len < 11);
  while (len-- > 0 && pos < BUFFER_SIZE - 1) {
      buffer[pos++] = numBuffer[len];
  }
}

void Logger::appendHex(uint32_t num) {
  const char* hexDigits = "0123456789ABCDEF";
  char hexBuffer[9];
  int len = 0;
  do {
      hexBuffer[len++] = hexDigits[num % 16];
      num /= 16;
  } while (num && len < 8);
  buffer[pos++] = '0';
  buffer[pos++] = 'x';
  while (len-- > 0 && pos < BUFFER_SIZE - 1) {
      buffer[pos++] = hexBuffer[len];
  }
}
void Logger::appendBinary(uint32_t num) {
  char binBuffer[33];
  int len = 0;
  do {
      binBuffer[len++] = (num & 1) ? '1' : '0';
      num >>= 1;
  } while (num && len < 32);
  buffer[pos++] = '0';
  buffer[pos++] = 'b';
  while (len-- > 0 && pos < BUFFER_SIZE - 1) {
      buffer[pos++] = binBuffer[len];
  }
}

void Logger::appendFloat(float num) {
  int32_t intPart = (int32_t)num;
  int32_t fracPart = (int32_t)((num - intPart) * powerOf10(floatPrecision));
  appendNumber(intPart);
  buffer[pos++] = '.';
  appendNumber(fracPart);
}

int32_t Logger::powerOf10(int exp) {
  int32_t result = 1;
  while (exp-- > 0) result *= 10;
  return result;
}

const char* Logger::logLevelToStr(LogLevel level) {
  switch (level) {
    case LogLevel::INFO: return "[INFO] ";
    case LogLevel::DEBUG: return "[DEBUG] ";
    case LogLevel::WARNING: return "[WARNING] ";
    case LogLevel::ERROR: return "[ERROR] ";
    default: return "[UNKNOWN] ";
  }
}

Logger::Logger(ILogTarget* tgt) : target(tgt), pos(0), currentLevel(LogLevel::INFO), numFormat(NumFormat::FMT_DECIMAL), floatPrecision(2) {}

void Logger::setLogLevel(LogLevel level) {
  currentLevel = level;
}

Logger& Logger::operator<<(const char* str) {
    appendToBuffer(str);
    return *this;
}

Logger& Logger::operator<<(char c) {
    if (pos < BUFFER_SIZE - 1) buffer[pos++] = c;
    return *this;
}

Logger& Logger::operator<<(int32_t num) {
    if (numFormat == NumFormat::FMT_HEX) appendHex(num);
    else if (numFormat == NumFormat::FMT_BINARY) appendBinary(num);
    else appendNumber(num);
    return *this;
}

Logger& Logger::operator<<(int num) {
    this->operator<<((int32_t)num);
    return *this;
}

Logger& Logger::operator<<(uint32_t num) {
    if (numFormat == NumFormat::FMT_HEX) appendHex(num);
    else if (numFormat == NumFormat::FMT_BINARY) appendBinary(num);
    else appendNumber(num);
    return *this;
}

Logger& Logger::operator<<(unsigned int num) {
    this->operator<<((uint32_t)num);
    return *this;
}

Logger& Logger::operator<<(float num) {
    appendFloat(num);
    return *this;
}

Logger& Logger::operator<<(double num) {
    appendFloat((float)num);
    return *this;
}

Logger& Logger::operator<<(LogLevel level) {
    appendToBuffer(logLevelToStr(level));
    return *this;
}

void Logger::flush() { flushBuffer(); }

// Manipulators
Logger& Logger::operator<<(NumFormat fmt) {
    numFormat = fmt;
    return *this;
}

Logger& Logger::operator<<(Precision p) {
    floatPrecision = (p.value >= 0 && p.value <= 6) ? p.value : 2;
    return *this;
}

Logger& Logger::operator<<(LoggerAction act) {
  switch (act) {
    case LoggerAction::ENDL:
      this->appendToBuffer(&this->newline);
    case LoggerAction::FLUSH:
      this->flushBuffer();
    default:
      break;
  }
  return *this;
}

};
