#ifndef BUFFERED_SW_PWM_H
#define BUFFERED_SW_PWM_H

#include <stdint.h>

class BufferedSoftwarePwm {
    uint32_t d, T;

    uint32_t i;

    uint32_t bd, bT;
public:

    void set(uint32_t duty, uint32_t period) {
        // Begin crytical section
        bd = duty;
        bT = period;
        // End crytical section
    }
    void setPeriod(uint32_t period) {
        set(bd, period);
    }
    void setDuty(uint32_t duty) {
        set(duty, bT);
    }

    bool step() {
        if (++i == T) {
            d = bd;
            T = bT;
            i = 0;
        }
        return i < d;
    }

    bool period_completed() const {
        return i == 0;
    }
};

#endif // BUFFERED_SW_PWM_H

