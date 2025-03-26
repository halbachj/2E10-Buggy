/**
 * @file EmbeddedLogger.hpp
 * @author Johannes Halbach
 *
 * This is a stream based logger inspired by a talk in cpp con on better embedded system design
 */


#ifndef EMBEDDED_LOGGER_H
#define EMBEDDED_LOGGER_H

#include <stdint.h>  // Fixed-width types
#include <stddef.h>  // size_t
#include "Arduino.h"

namespace EmbeddedLogger {

// Log levels
enum class LogLevel {
    INFO, DEBUG, WARNING, ERROR
};

// Number formatting modes
enum class NumFormat {
  FMT_DECIMAL, FMT_HEX, FMT_BINARY, FMT_FIXED // cant use HEX because of arduino macro, grrr
};

enum class LoggerAction {
  ENDL, FLUSH
};

// Interface for log targets (e.g., serial, file, etc.)
class ILogTarget {
public:
    virtual void write(const char* data, size_t length) = 0;
    virtual ~ILogTarget() = default;
};

// Logger class
class Logger {
private:
  static constexpr size_t BUFFER_SIZE = 128;
  char buffer[BUFFER_SIZE];
  size_t pos;
  ILogTarget* target;
  LogLevel currentLevel;
  NumFormat numFormat;

  const char newline = '\n';

  int floatPrecision;

  void flushBuffer();
  void appendToBuffer(const char* str);
  void appendToBuffer(const __FlashStringHelper* str);
  void appendNumber(int32_t num);
  void appendHex(uint32_t num);
  void appendBinary(uint32_t num);
  void appendFloat(float num);
  int32_t powerOf10(int exp);
  const char* logLevelToStr(LogLevel level);

public:
  explicit Logger(ILogTarget* tgt);
  void setLogLevel(LogLevel level);
  
  struct Precision {
    int value;
  };

  Logger& operator<<(const char* str);
  Logger& operator<<(const __FlashStringHelper* str);
  Logger& operator<<(char c);
  Logger& operator<<(int32_t num);
  Logger& operator<<(int num);
  Logger& operator<<(uint32_t num);
  Logger& operator<<(unsigned int num);
  Logger& operator<<(float num);
  Logger& operator<<(double num);
  Logger& operator<<(LogLevel level);

  void flush();
  // Manipulators
  Logger& operator<<(NumFormat fmt);

  Logger& operator<<(Precision p);

  // Actions
  Logger& operator<<(LoggerAction act);
};

// Manipulators
inline NumFormat hex = NumFormat::FMT_HEX;
inline NumFormat dec = NumFormat::FMT_DECIMAL;
inline NumFormat bin = NumFormat::FMT_BINARY;
inline NumFormat fixed = NumFormat::FMT_FIXED;

inline LoggerAction endl = LoggerAction::ENDL;
inline LoggerAction flush = LoggerAction::FLUSH; 

inline Logger::Precision precision(int p) { return { p }; };

extern Logger logger;

}; // namespace EmbeddedLogger

#endif // EMBEDDED_LOGGER_H