#include "Controller.hpp"

namespace Cycleshooter {

Context Controller::getContext() const {
    return context;
}

NodeManager *Controller::getNodeManager() const {
    return nodeManager;
}

Ogre::Root *Controller::getRoot() const {
    return oRoot;
}

Ogre::RenderWindow *Controller::getWindow() const {
    return oRoot->getAutoCreatedWindow();
}

Ogre::SceneManager *Controller::getSceneManager() const {
    return oSceneManager;
}

Ogre::OverlaySystem *Controller::getOverlaySystem() const {
    return oOverlaySystem;
}

HUD *Controller::getHud() const {
    return hud;
}

void Controller::setHud(HUD *value) {
    hud = value;
}

Controller::Controller() {
    go();
}

Controller::~Controller() {
    if(nodeManager)
        delete nodeManager;

    if(polarUpdater)
        delete polarUpdater;

    if(polar)
        delete polar;
}

void Controller::polarUpdaterFunction() {
    while(!shutdown) {
        sf::sleep(POLAR_SLEEP_TIME);

        try {
            unsigned heartRate = polar->getInstantaneousHeartRate();
            if(heartRate) {
                logicManager->setHeartRate(heartRate);
            }
            else {
                Ogre::LogManager::getSingleton().logMessage("polarUpdaterFunction: heartRate is zero!! You're either dead or far from the control board", Ogre::LML_CRITICAL);
            }
        }
        catch (...) {
            Ogre::LogManager::getSingleton().logMessage("polarUpdaterFunction: caught exception", Ogre::LML_CRITICAL);
        }
    }
}

void Controller::go() {
    createRoot();

    Ogre::LogManager::getSingleton().logMessage("--> Controller: go <--");

    createSceneManager();
    createOverlaySystem();
    setupResources();

    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

    // logicmanager

    polar = new RandomPolar();
    polarUpdater = new sf::Thread(&Controller::polarUpdaterFunction, this);
    polarUpdater->launch();

    nodeManager = new NodeManager(this);
    nodeManager->setupRunnerMode();

    // to use a material, the resource group must be initialized
    terrainManager = new TerrainManager(oSceneManager);
    terrainManager->createTerrain();

    // starting collision handler after terrain initialization
    collisionHandler = new CollisionHandler(MAIN_TEXTURE);
    collisionHandler->loadImages();
    collisionHandler->loadTensor();
}

void Controller::setupResources() {
    Ogre::LogManager::getSingleton().logMessage("--> Controller: Setting up Resources <--");

    Ogre::ConfigFile cf;
    cf.load(RESOURCES_CONFIG);

    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
    Ogre::String secName, typeName, archName;

    while (seci.hasMoreElements()) {
        secName = seci.peekNextKey();

        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;

        for (i = settings->begin(); i != settings->end(); ++i) {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
        }
    }

    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void Controller::createRoot() {
    std::cout << "--> Controller: creating Root <--" << std::endl;

    oRoot = new Ogre::Root();

    // alternatively, use ->restoreConfig() to load saved settings
    if(!oRoot->showConfigDialog()) {
        return;
    }

    // automatically create a window
    oRoot->initialise(true, RENDER_WINDOW_NAME);
}

void Controller::createSceneManager() {
    Ogre::LogManager::getSingleton().logMessage("--> Controller: Creating Scene Manager <--");

    oSceneManager = oRoot->createSceneManager(Ogre::ST_GENERIC);
}

void Controller::createOverlaySystem() {
    Ogre::LogManager::getSingleton().logMessage("--> Controller: Creating Overlay System <--");

    oOverlaySystem = new Ogre::OverlaySystem();
    oSceneManager->addRenderQueueListener(oOverlaySystem);
}

void Controller::setupRunnerMode() {
    Ogre::LogManager::getSingleton().logMessage("--> Controller: Setting up Runner Mode <--");

    context = CONTEXT_RUNNER;
    nodeManager->setupRunnerMode();
    hud->setupRunnerMode();
}

void Controller::setupShooterMode() {
    Ogre::LogManager::getSingleton().logMessage("--> Controller: Setting up Shooter Mode <--");

    context = CONTEXT_SHOOTER;
    nodeManager->setupShooterMode();
    hud->setupShooterMode();
}

void Controller::toggleMode() {
    switch(context) {
    case CONTEXT_RUNNER:
        setupShooterMode();
        break;
    case CONTEXT_SHOOTER:
        setupRunnerMode();
        break;
    }
}

void Controller::toggleMode(const Context &newContext) {
    if (newContext == CONTEXT_RUNNER && context != CONTEXT_RUNNER)
        setupRunnerMode();
    else if (newContext == CONTEXT_SHOOTER && context != CONTEXT_SHOOTER)
        setupShooterMode();
}

void Controller::setupDebugModeOn() {
    Ogre::LogManager::getSingleton().logMessage("--> Controller: Turning Debug Mode On <--");

    debug = true;
    nodeManager->setDebugOn();
    hud->setupDebugOn();
}

void Controller::setupDebugModeOff() {
    Ogre::LogManager::getSingleton().logMessage("--> Controller: Turning Debug Mode Off <--");

    debug = false;
    nodeManager->setDebugOff();
    hud->setupDebugOff();
}

void Controller::toggleDebugMode() {
    Ogre::LogManager::getSingleton().logMessage("--> Controller: Toggling Debug Mode <--");

    if(debug) {
        setupDebugModeOff();
    }
    else {
        setupDebugModeOn();
    }
}

bool Controller::isDebugModeOn() const {
    return debug;
}

}
