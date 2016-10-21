#ifndef BASE_H
#define BASE_H

#include <arduino.h>

typedef uint8_t byte_t;
typedef bool bool_t;
typedef uint8_t result_t;

#define ERR_OK 0
#define ERR_FAILED 0xFF

#ifndef sbi
#define sbi(port, bit) _SFR_BYTE(port) |= _BV(bit)
#endif

#ifndef cbi
#define cbi(port, bit) _SFR_BYTE(port) &= ~(_BV(bit))
#endif


#endif
