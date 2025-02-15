#ifndef LEANSTREAMIO_LOGGER_HPP
#define LEANSTREAMIO_LOGGER_HPP

#include "LeanStreamIO.hpp"

namespace mcu {

    class Logger {
    public:
        using Loglevel_t = enum class LogLevel {
            None, Debug, Info, Warning, Error
        };

    protected:
        Loglevel_t mLogLevel;
    private:
        LeanStreamIO stream;
    public:
        explicit Logger(size_t fd) : stream(fd) {};

        Logger& operator<<(const char* message) {
            const char* level = "";
            switch(this->mLogLevel) {
                case LogLevel::Debug:
                    level = "[DEBUG]";
                    break;
                case LogLevel::Info:
                    level = "[INFO]";
                    break;
                case LogLevel::Warning:
                    level = "[WARNING]";
                    break;
                case LogLevel::Error:
                    level = "[ERROR]";
                    break;
                case LogLevel::None: // Remove Logging level print for subsequent calls
                default:
                    break;
            }
            this->mLogLevel = LogLevel::None;
            this->stream << level << message;
            return *this;
        }

        Logger& operator<<(LogLevel level) {
            this->mLogLevel = level;
            return *this;
        }

        Logger& operator<<(int value) {
          this->stream << value;
          return *this;
        }

    };
    //extern Logger logger;

};

#endif //LEANSTREAMIO_LOGGER_HPP
