#ifndef THROTTLE_H
#define THROTTLE_H

#include <stdint.h>

/// Handles acceleration via time dilatation.
/** This class handles acceleration separately from the stepper
 *  motor controllers. Instead of changing the stepper speed
 *  directly this class helps to create a virtual, distorted
 *  time frame in which the stepper motors are always at full
 *  speed. Time itself moves slower at the start and end of
 *  every movement, as controlled by variable rates.
 *
 *  The variable rate comes from a variable numerator and a
 *  constant denominator. The user can set the denominator
 *  and the initial and final values of the numerator for each
 *  movement. The numerator is incremented, kept, or decremented
 *  once every step in oder to create the appropriate time
 *  distortion for acceleration.
 */
struct Throttle {
public:
    uint32_t n;   //!< current output samples counter
    uint32_t n1;  //!< output sample at which to start deceleration
    uint32_t N;   //!< total output samples required

    uint32_t rd;  //!< Rate denominator
    uint32_t r0;  //!< Start rate numerator
    uint32_t r1;  //!< End rate numerator
    uint32_t r;   //!< Current rate
    int32_t  acc; //!< accumulator

    bool paused;

    uint32_t min(uint32_t a, uint32_t b) {
        return (a<b) ? a : b;
//        return a + ((b-a) & (int32_t(a-b) >> 31));
    }

public:
    /// Prepare time distortion for one movement.
    /** \param nN  is the number of samples required for
     *             the steppers to complete the movement.
     *  \param nrd is the time rate denominator and is
     *             inversely proportional to the acceleration.
     *             Full speed will be reached after nrd-nr0
     *             samples.
     *  \param nr0 is the initial rate numerator. The initial
     *             speed will be nr0/nrd times the speed
     *             programmed for the stepper class.
     *  \param nr1 is the final rate numerator. The final
     *             speed will be nr1/nrd times the speed
     *             programmed for the stepper class.
     */
    void prepare(
            uint32_t nN,
            uint32_t nrd,
            uint32_t nr0,
            uint32_t nr1
    ) {
        n   = 0;
        N   = nN;
        rd  = nrd;
        r0  = nr0;
        r1  = nr1 ? nr1 : 1; // Zero could cause a deadlock
        paused = false;

        acc = -int32_t(r0); // 1 sample delay, for DIR bits.
        r   = r0;
        uint32_t dk = rd-r1;
        if (rd) n1 = N - dk + dk*1.*dk/(2.*rd);
        else    n1 = 0;
    }

    Throttle() {
        prepare(0,0,0,0);
    }

    /// Evaluates one time step.
    /** Should be called once every real time sample, and
     *  used to decide wether or not to evaluate the stepper
     *  motion interpolators.
     *  \return true if the steppers should be evaluated, or
     *          false otherwise.
     */
    bool step() {
        if (paused) {
            // On pause, decelerate gracefully to zero.
            if (r) r--;
        } else {
            // If before brake point
            if (n<n1) {
                // Then accelerate to max-speed
                if (r < rd) r++;
            } else {
                // Otherwise decelerate to end-speed
                if (r > r1) r--;

                // On resume, accelerate to end-speed
                if (r < r1) r++;
            }
        }
        acc += r;
        if (acc > 0) {
            acc -= rd;
            if (n < N) n++;
            return true;
        }
        return false;
    }

    /// Detects wether the current movement is completed.
    /** \return true if movement is completed, or
     *          false if movement is in progress.
     */
    bool done() const {
        return n == N;
    }

    /// Pauses the movement halfway with deceleration.
    void pause() {
        paused = true;
    }

    /// Resumes the movement with acceleration.
    void resume() {
        paused = false;
    }
};

#endif // THROTTLE_H

/* N = total output steps
 * n = current step
 * n1 = steps until full-speed
 *
 * */
