#include <stdint.h>
#include <immintrin.h>
#include <emmintrin.h>

#include "conversion.h"

void to_carthesian(unsigned __int128 bm1pi, __int128 *real, __int128 *imag) {
    //DONE: implement to_carthesian
    *real = 0;
    *imag = 0;

    if (bm1pi == 0) return;

    unsigned __int128 mask = 1;
    __int128 power = 1;
    for (int i = 0; i < 16; ++i) {

        //0,8,16,...
        if (bm1pi & mask) {
            *real += power;
        }
        mask <<= 1;

        //1,9,17,...
        if (bm1pi & mask) {
            *real -= power;
            *imag += power;
        }
        mask <<= 1;
        power <<= 1;

        //2,10,18,...
        if (bm1pi & mask) {
            *imag -= power;
        }
        mask <<= 1;

        //3,11,19,..
        if (bm1pi & mask) {
            *real += power;
            *imag += power;
        }
        mask <<= 1;
        power <<= 1;

        //4,12,20,...
        if (bm1pi & mask) {
            *real -= power;
        }
        mask <<= 1;

        //5,13,21,...
        if (bm1pi & mask) {
            *real += power;
            *imag -= power;
        }
        mask <<= 1;
        power <<= 1;

        //6,14,22,...
        if (bm1pi & mask) {
            *imag += power;
        }
        mask <<= 1;

        //i=7,15,23,...
        if (bm1pi & mask) {
            *real -= power;
            *imag -= power;
        }
        mask <<= 1;
        power <<= 1;
    }
}

unsigned __int128 to_bm1pi(__int128 real, __int128 imag) {
    //DONE: implement to_bm1pi
    unsigned __int128 result = 0;
    unsigned __int128 mask = (unsigned __int128) 1;

    for (int i = 0; i < 128 && (real || imag); ++i, mask <<= 1) {
        __int128 temp = real; //secure the current value of real
        if ((real & 1) == (imag & 1)) {
            // (imag - real) and (-temp - imag) are both even -> no remainder -> don't change result
            real = (imag - real) >> 1;
            imag = (-temp - imag) >> 1;
        } else {
            // (imag - real) and (-temp - imag) are both odd ->add one to them because of remainder -> change results current bit to 1
            real = (imag - real + 1) >> 1;
            imag = (-temp - imag + 1) >> 1;
            result |= mask;
        }
    }
    return result;
}

//  DONE: alternative implementation of to_carthesian for performance comparison

void to_carthesian_V1(unsigned __int128 bm1pi, __int128 *real, __int128 *imag) {
    *real = 0;
    *imag = 0;

    //  The conversion from bm1pi to cartesian can also be done as a matrix (2 by 128) - vector (128 bits) multiplication or a
    //  matrix a (2 by 8) - matrix (8 by 16) - matrix (16 by 1) multiplication realized with simd as the first variant is not going
    //  fit in the 128 bits simd registers. For proof of correctness visit the documentation (../Ausarbeitung)

    const __m128i real_row = _mm_setr_epi16(1, -1, 0, 2, -4, 4, 0, -8);
    const __m128i imag_row = _mm_setr_epi16(0, 1, -2, 2, 0, -4, 8, -8);
    __m128i temp_real, temp_imag, col;

    int16_t bits[16][8];
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 8; j++) {
            bits[i][j] = (int16_t) (bm1pi & 1);
            bm1pi >>= 1;
        }
    }

    for (int i = 0; i < 16; i++) {
        col = _mm_loadu_si128((__m128i *) &bits[i]);
        temp_real = _mm_madd_epi16(real_row, col);
        temp_real = _mm_hadd_epi32(temp_real, temp_real);
        temp_real = _mm_hadd_epi32(temp_real, temp_real);

        temp_imag = _mm_madd_epi16(imag_row, col);
        temp_imag = _mm_hadd_epi32(temp_imag, temp_imag);
        temp_imag = _mm_hadd_epi32(temp_imag, temp_imag);

        // Extract the result from the sum vector
        *real += ((__int128) (_mm_cvtsi128_si32(temp_real)) << (4 * i));
        *imag += ((__int128) (_mm_cvtsi128_si32(temp_imag)) << (4 * i));
    }
}

void to_carthesian_V2(unsigned __int128 bm1pi, __int128 *real, __int128 *imag) {
    //DONE: implement to_carthesian

    __int128 baseReal = -1;
    __int128 baseImag = 1;

    __int128 powZero = (bm1pi & (0b1));
    powZero -= (bm1pi & 0b10) >> 1;
    __int128 powOne = (bm1pi & 0b10) >> 1;

    __int128 sumReal = 0;
    __int128 sumImag = 0;

    unsigned __int128 mask = 1;

    __int128 temp; //Secure the current value of real

    bm1pi >>= 2;
    while (bm1pi > 0) {

        temp = baseReal;
        baseReal = (-baseReal) - baseImag;
        baseImag = temp - baseImag;

        if (bm1pi & mask) {
            sumReal += baseReal; //Sum of real part
            sumImag += baseImag; //Sum of imaginary part
        }
        bm1pi >>= 1;
    }

    *real = sumReal + powZero;
    *imag = sumImag + powOne;
}
