#ifndef _ABSTRACTBICYCLE_HPP_
#define _ABSTRACTBICYCLE_HPP_

namespace Cycleshooter {

class AbstractBicycle {

    /**
     * Factor to convert from the real speed (RPM) to the logical speed (units of the game).
     */
    const double RPM_TO_LOGICAL_SPEED = 10.0;

protected:
    /**
     * Friction (also difficulty, load): resistance of the bicycle to movement.
     * Tipically in the 0--400 range.
     */
    int friction;

    /**
     * Current speed of the bicycle (RPM). Tipically in the 0--100 range.
     */
    double speed;

public:
    AbstractBicycle(){}

    /**
     * Get the current speed of the bicycle (in speed units of the game).
     */
    double getSpeed() const {
        return RPM_TO_LOGICAL_SPEED * speed;
    }

    /**
     * Update the speed of the bicycle, by reading a response from its controller circuit.
     */
    virtual void updateSpeed() = 0;

    /**
     * Get the current friction of the bicycle.
     */
    int getFriction() const {
        return friction;
    }

    /**
     * Set the friction of the bicycle, by sending a command to the controller circuit.
     */
    virtual void setFriction(const int& value) = 0;
};

}

#endif
