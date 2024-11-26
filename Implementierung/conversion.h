#ifndef CONVERSION_H
#define CONVERSION_H

void to_carthesian(unsigned __int128 bm1pi, __int128 *real, __int128 *imag);

void to_carthesian_V1(unsigned __int128 bm1pi, __int128 *real, __int128 *imag);

unsigned __int128 to_bm1pi(__int128 real, __int128 imag);

#endif //CONVERSION_H
