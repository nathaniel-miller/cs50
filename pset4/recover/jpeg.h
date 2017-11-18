# include <stdint.h>

typedef uint8_t BYTE;
typedef uint32_t CHUNK;

typedef struct
{
    BYTE byte_1;
    BYTE byte_2;
    BYTE byte_3;
    BYTE byte_4_min;
    BYTE byte_4_max;
} __attribute__((__packed__))
SIG;


// byte_4 should be between 11100000 (208) & 11101111 (223)