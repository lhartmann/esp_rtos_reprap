#ifndef SIGMADELTA_H
#define SIGMADELTA_H

#include <stdint.h>

class SigmaDeltaModulator {
    uint32_t d, T;
    int32_t acc;
public:
    void set(uint32_t duty, uint32_t period) {
        d = duty;
        T = period;
    }

    bool step() {
        acc += d;
        if (acc > 0) {
            acc -= T;
            return true;
        }
        return false;
    }
};

#endif // SIGMADELTA_H

