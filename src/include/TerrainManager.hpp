#ifndef _TERRAINMANAGER_HPP_
#define _TERRAINMANAGER_HPP_

#include <Ogre.h>
#include <OgreTerrain.h>
#include <OgreTerrainGroup.h>
#include <OgreSceneManager.h>
#include <OgreRay.h>

#include "CollisionHandler.hpp"
#include "ConfigManager.hpp"
#include "Logging.hpp"

namespace Cycleshooter {

class TerrainManager {
private:
    Ogre::SceneManager* const sceneManager;

    /**
     * Terrain properties
     *  
     */
    Ogre::String terrainType = "Plane";
    Ogre::Real terrainWorldSizeHeight,terrainWorldSizeWidth;
    
    /**
     * Scale between image and terrain size.
     * Scales between the terrain
     * and collision data structure.
     */
    Ogre::Real widthScale, heightScale;
    
    /**
     * Vector to help with the system coordinator in Ogre3D
     * Translation from terrain in Ogre dimensions
     * to collision dimensions.
     */
    Ogre::Vector3 terrainTranslation;
    
    /**
     *
     */
    Ogre::Real GRASS_WIDTH, GRASS_HEIGHT;
    /**
     * Terrain size
     */
    int terrainWidth, terrainHeight;
    
    /**
     * Matrix to check position of elements in terrain
     * and check for collisions.
     */
    CollisionHandler* collisionHandler = NULL;

    /**
     * Set the environment variables to create the terrain.
     */
    void defineTerrainProperties();
    
    /**
     * Method to create and populate the terrain.
     */
    void createTerrain();

    /**
     * Creating a wall to close the terrain.
     */
    void createTerrainWall();

    /**
     * Create the terrain lake
     */
    void createTerrainLake();

    /**
     * Defines translation and scale to collision coordinates
     * (maps continous rectangles in  discrete rectangles).
     */
    void setCollisionTransformation();

    /**
     *  Function to return random IDs to calculate random positions for the bullets
     * inside the terrain.
     */
    std::vector<int> randomIdOfBullets(int maxvalue, int numOfPoints);

public:
    TerrainManager(Ogre::SceneManager* sceneManager, std::string collisionTexturePath);
    virtual ~TerrainManager();
    
    /**
     * Returns the data structure coordinates to
     * a given world point.
     */
    std::pair<int,int> getCollisionCoordinates(Ogre::Vector3 point);

    /**
     * Returns the WorldCoordinates of
     * a given Collision Coordinate.
     */
    Ogre::Vector3 getWorldCoordinates(std::pair<int,int> collisionCoord);

    /**
     * Discover terrain type and bullet existence
     * by point-bounding box intersection in a given terrain point.
     */
    std::pair<int,bool> getTerrainAt(Ogre::Vector3 coord);

    /**
     * Discover terrain type and bullet existence by
     * line-bounding box intersection in a given terrain point.
     */
    std::pair<int,bool> getTerrainAt(Ogre::Vector3 coord, Ogre::Vector3 lastCoord);

    /**
     * Defines if a line segment delimeted by p1 and p2 intersects
     * the bounding box bBox.
     */
    bool calculateSLBIntersection(Ogre::Vector3 p1,Ogre::Vector3 p2, Ogre::AxisAlignedBox bBox);

    /**
     * Obtain circuits controll points in world coordinates.
     */
    std::vector<Ogre::Vector3> obtainCircuitControllPoints();

    /**
     * Check if the player catch the bullet.
     */
    std::vector<std::pair<int,int> > calculateBulletSurroundings(Ogre::Vector3 center,Ogre::AxisAlignedBox boundingBox);
    std::vector<std::pair<int,int> > calculateBulletSurroundings(Ogre::Vector3 center);

    /**
     * Randonly generates the bullets in the region of the circuit.
     */
    void generateBullets(int numOfBullets);
    
    /**
     * Render bullets in the specific position.
     */
    void renderBullets();
      /////////////////////
     // Debug functions //
    /////////////////////
    
    /**
     * Print the Collision transformation (Image -> Ogre World)
     */
    void printCollisionTransformation();
    
};
}

#endif
