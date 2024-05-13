#ifndef CRC_H
#define CRC_H

#include <cstdint>

uint32_t Crc32_Calc(uint8_t *begin, std::size_t size, uint32_t initValue);


#endif // CRC_H
