#ifndef TRANSPORTDELAY_H
#define TRANSPORTDELAY_H

/// Transport delay buffer for selected bits of integgers.
/** @param T   type of data in the buffer.
 *  @param N   number os samples todelay.
 *  @param M   bitmask selecting which bits will be delayed. Set=delayed.
 */
template <typename T, int N, T M> class MaskedTransportDelay {
    T buff[N]; //!< Delay buffer
    T *p;      //!< Current sample pointer

public:
    MaskedTransportDelay() : p(buff+N) {}
    T operator() (T x) {
        // Advance pointer
        if (p == buff) p += N-1;
        else p--;

        // Exchange data
        T y = *p;
        *p = x;

        return y & M | x & ~M;
    }
};

#endif // TRANSPORTDELAY_H

