#ifndef STEPPER_H
#define STEPPER_H

#include <stdint.h>

/// Stepper motor position interpolator.
/** Handles bresenham algorithms for a single stepper motor.
 *  Time samples are considered the dominant axis.
 *  Decides wether to take a step or nor.
 *  Assumes always fullspeed, does not handle acceleration.
 *  Does not handle direction.
 */
class Stepper {
    uint32_t current, end;
    uint32_t rate_up, rate_dn;
    int32_t acc;
public:
    Stepper() {
        prepare(0,0);
    }

    /// Prepare for a movement.
    /** \param D  specifies the total steps count to generate.
     *  \param T  specifies how many time samples should this
     *            movement take at full speed.
     */
    void prepare(
        uint32_t D,
        uint32_t T
    ) {
        current = 0;
        end = D;
        rate_up = D;
        rate_dn = T;
        acc = -int32_t(T)/2;
    }

    /// Decides wether to take a step.
    /** This function should be called every sample
     *  to update the stepper position.
     *  \return true if a step should be taken.
     */
    bool step() {
        acc += rate_up;
        if (acc>0) {
            acc -= rate_dn;
            if (++current == end) {
                prepare (0,0);
            }
            return true;
        }
        return false;
    }

    /// Detects if the last movement has completed.
    /** \returns true if the movement is completed, or
     *           false if it is still in progress.
     */
    bool done() const {
        return current == end;
    }
};

#endif // STEPPER_H
