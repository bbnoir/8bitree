#pragma once
#define BIT_NUM 8

#if BIT_NUM == 8
#define SYM_MIN -128 // min value of int8_t
#define SYM_MAX 127  // max value of int8_t
#define SYM_NUM 256
#define ARRAY_SIZE 50

#elif BIT_NUM == 4
#define SYM_MIN -8 // min value of int4_t
#define SYM_MAX 7  // max value of int4_t
#define SYM_NUM 16
#define ARRAY_SIZE 36

#endif

enum class INIT_MODE
{
    BALANCED,
    HUFFMAN,
    FOURBIT
};

