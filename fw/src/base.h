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

#define zero(mem, size) memset((mem), 0, (size))

//callback defines
typedef void (*callback_fn_t)(void*);
typedef struct _callback_ {
    callback_fn_t cb_fn;
    void* cb_param;
} callback_t;




#endif
