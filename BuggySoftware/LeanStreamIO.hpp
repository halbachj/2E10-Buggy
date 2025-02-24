#ifndef LEANSTREAMIO_LEANSTREAMIO_HPP
#define LEANSTREAMIO_LEANSTREAMIO_HPP

#include <string>
#include <cstddef>
#include <cstring>
#include "BuggyTCP.hpp"

extern "C" size_t _write(size_t fd, const char * data, size_t count);
extern TcpServer server;

namespace mcu {

    class LeanStreamIO {
    public:
        using Radix_t = enum class RadixEnum {
            Binary = 2, Octal = 8, Decimal = 10, Hexadecimal = 16,
        };
        static constexpr char* endl = "\n";

    protected:
        size_t mFileDescriptor;
        Radix_t mRadixSetting = Radix_t::Decimal;

    public:
        explicit LeanStreamIO(size_t fd) : mFileDescriptor (fd), mRadixSetting(RadixEnum::Hexadecimal) {}

    public:
        LeanStreamIO &operator<<(const char *string) {
            const size_t length = std::strlen(string);
            _write(mFileDescriptor, string, length);

            return *this;
        }

        LeanStreamIO &operator<<(const std::string &msg);
        LeanStreamIO &operator<<(unsigned int value);
        LeanStreamIO &operator<<(Radix_t radix);

        size_t Descriptor() const { return mFileDescriptor; }
    };
    
    extern LeanStreamIO logger;

};



#endif //LEANSTREAMIO_LEANSTREAMIO_HPP
