#include "LeanStreamIO.hpp"
#include <Arduino.h>
#include "PacketFactory.hpp"

extern TcpServer logging_server;
extern "C" size_t _write(size_t fd, const char * data, size_t count) {
    size_t written_count = 0;
    switch (fd) {
        case 1: // Serial
            if(!Serial) // Check if serial exists
                Serial.begin(115200);
            Serial.write(data, count);
            written_count = count;
            break;
        case 2: // WiFi logger
            logging_server.sendPacket(PacketFactory::createLogPacket(count, data));
            break;
        case 3: // Serial + WiFi
            logging_server.sendPacket(PacketFactory::createLogPacket(count, data));
            if(!Serial) // Check if serial exists
                Serial.begin(115200);
            Serial.write(data, count);
            written_count = count;
            break;
        default:
            return -1; // Error for unsupported fd
    }

    return count;
}

namespace mcu {

    LeanStreamIO &LeanStreamIO::operator<<(const std::string &msg) {
        const size_t length = msg.length();
        _write(mFileDescriptor, msg.c_str(), length);

        return *this;
    }


    LeanStreamIO& LeanStreamIO::operator<<(unsigned int value) {
        const char *const digits = "0123456789ABCDEF";
        const auto radix = static_cast<unsigned int>(mRadixSetting);
        static constexpr size_t MaxFieldWidth = 34;
        char fieldData[MaxFieldWidth];
        size_t fieldIndex = 0;
        int first = 0;
        switch (mRadixSetting) {
            case RadixEnum::Binary:
                fieldData[0] = '0';
                fieldData[1] = 'b';
                fieldIndex += 2;
                first += 2;
                break;
            case RadixEnum::Octal:
                fieldData[0] = '0';
                fieldData[1] = 'o';
                fieldIndex += 2;
                first += 2;
                break;
            case RadixEnum::Decimal:
                break;
            case RadixEnum::Hexadecimal:
                fieldData[0] = '0';
                fieldData[1] = 'x';
                fieldIndex += 2;
                first += 2;
                break;
        }
        // calculate digits (they will be reversed)
        unsigned int remainder;
        do {
            remainder = value % radix;
            fieldData[fieldIndex] = digits[remainder];
            value /= radix;
            fieldIndex += 1;
        } while (value != 0);
        // reverse order
        size_t last = fieldIndex - 1;
        while ((last - first) > 0) {
            char tmp = fieldData[last];
            fieldData[last] = fieldData[first];
            fieldData[first] = tmp;
            first += 1;
            last -= 1;
        }
        _write(mFileDescriptor, fieldData, fieldIndex);
        return *this;
    }

    LeanStreamIO& LeanStreamIO::operator<<(Radix_t radix) {
        mRadixSetting = radix;
        return *this;
    }

};
