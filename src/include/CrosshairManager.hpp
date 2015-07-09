#ifndef _CROSSHAIRMANAGER_HPP_
#define _CROSSHAIRMANAGER_HPP_

#include <OgreImage.h>
#include <OgreOverlay.h>
#include <OgreOverlayManager.h>

namespace Cycleshooter {
// TODO: sensibility and entropy
class CrosshairManager {

    /**
     * The crosshair overlay.
     */
    Ogre::Overlay* crosshair;

    /**
     * Reduction scale of the crosshair size.
     */
    const double CROSSHAIR_SCALE_SIZE = 0.25;

public:
    CrosshairManager() {
        crosshair = Ogre::OverlayManager::getSingleton().getByName("Cycleshooter/Crosshair");
        crosshair->setScale(CROSSHAIR_SCALE_SIZE, CROSSHAIR_SCALE_SIZE);
    }

    /**
     * Move/scroll the crosshair by the amount specified. Optionally it may wrap around the screen too.
     */
    void scroll(const double& dx, const double& dy, bool wraps = false) {
        double px = crosshair->getScrollX() + dx;
        double py = crosshair->getScrollY() + dy;

        px = (px > 1.0) ? (wraps ? -1.0 : +1.0) : px;
        px = (px < -1.0) ? (wraps ? +1.0 : -1.0) : px;

        py = (py > 1.0) ? (wraps ? -1.0 : +1.0) : py;
        py = (py < -1.0) ? (wraps ? +1.0 : -1.0) : py;

        crosshair->setScroll(px, py);
    }

    /**
     * Return the (x,y) \in [-1.0, +1.0]^2 coordinates of the crosshair.
     */
    std::pair<double, double> getScrollXY() const {
        return std::make_pair(getScrollX(), getScrollY());
    }

    /**
     * Return the x \in [-1.0, +1.0] coordinate of the crosshair.
     */
    double getScrollX() const {
        return crosshair->getScrollX();
    }

    /**
     * Return the y \in [-1.0, +1.0] coordinate of the crosshair.
     */
    double getScrollY() const {
        return crosshair->getScrollY();
    }

    /**
     * Map [-1.0, +1.0]^2 from crosshair coordinates to [0, width) x [0, height) image coordinates.
     */
    std::pair<int, int> convertToImageCoordinates(const Ogre::Image& image) const {
        auto width = image.getWidth();
        auto height = image.getHeight();

        // map [-1.0, +1.0] to [0, width)
        int retx = ((getScrollX() + 1.0) * width) / 2.0;

        // map [-1.0, +1.0] to [0, height)
        // inverted in the vertical
        int rety = height - int(((getScrollY() + 1.0) * height) / 2.0);

        if(retx == image.getWidth())
            --retx;

        if(rety == image.getHeight())
            --rety;

        return std::make_pair(retx, rety);
    }

    void setupRunnerMode() {
        crosshair->hide();
    }

    void setupShooterMode() {
        crosshair->show();
    }

};
}

#endif
