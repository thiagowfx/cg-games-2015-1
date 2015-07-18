#ifndef _AUDIOMANAGER_HPP_
#define _AUDIOMANAGER_HPP_

#include <deque>
#include <iostream>
#include <map>
#include <SFML/Audio.hpp>
#include <SFML/System/Mutex.hpp>

#include "ConfigManager.hpp"
#include "Logging.hpp"
#include "Singleton.hpp"

namespace Cycleshooter {

/**
 * Each element represents an unique sound.
 */
enum Soundname {
    /* shoot (weapon fire) sounds */
    SOUND_SHOOT01,
    SOUND_SHOOT02,
    SOUND_SHOOT03,
    SOUND_SHOOT04,

    /* dry shoot (=no ammo left) */
    SOUND_DRY_SHOOT,

    /* game events */
    SOUND_GAME_LOSS,
    SOUND_GAME_VICTORY,
    SOUND_MONSTER_HIT,

    /* heartbeat sounds */
    SOUND_HEARTBEAT01,
    SOUND_HEARTBEAT02,
    SOUND_HEARTBEAT03,
    SOUND_HEARTBEAT04,
    SOUND_HEARTBEAT05,
};

/**
 * Each element represents an unique music.
 */
enum Musicname {
    /* runner mode music */
    MUSIC_RUNNER,

    /* shooter mode music */
    MUSIC_SHOOTER,
};

/**
 * @brief A singleton class to manage all game music and sounds.
 * Inspired by: http://stackoverflow.com/a/27860198
 * Notes: sound = short duration; music = long duration; audio = generic word to refer to both of them.
 * Only one background music is allowed at any time. However, several sound effects might be played simultaneously.
 */
class AudioManager {
    SINGLETON(AudioManager)

    /**
     * @brief Location of the sounds. It must have a trailing slash.
     */
    const std::string SOUND_PATH = ConfigManager::instance().getStr("AudioManager.soundpath");

    /**
     * @brief Location of the musics. It must have a trailing slash.
     */
    const std::string MUSIC_PATH = ConfigManager::instance().getStr("AudioManager.musicpath");

    /**
     * @brief How much is the music volume lower than the sound volume?
     * E.g. 0.5 means the music is 50% lower than the sound.
     */
    const double MUSIC_VOLUME_RATIO = ConfigManager::instance().getDouble("AudioManager.music_volume_ratio");

    /**
     * Maps all soundnames to their respective soundbuffers.
     */
    std::map<Soundname, sf::SoundBuffer> soundMap;

    /**
     * Maps all musicnames to their respective Music variables.
     */
    std::map<Musicname, sf::Music> musicMap;

    /**
     * Populate the sounds map.
     */
    void populateSounds();

    /**
     * Populate the musics map.
     */
    void populateMusics();

    /**
     * Manages sounds being played currently.
     */
    std::deque<sf::Sound> playingSounds;

    /**
     * The current playing music, if any.
     */
    sf::Music* currentPlayingMusic = NULL;

    sf::Mutex currentPlayingMusicMutex;

    /**
     * Randomly plays a sound from the specified sound list.
     */
    void playRandom(const std::vector<Soundname>& sound_list);

    /**
     * Is the music muted?
     */
    bool musicMuted = false;

    /**
     * Mute the current playing music if the mute_music variable is set to true.
     */
    void doMute();

    /**
     * The heartbeat sound, occasionally played.
     */
    sf::Sound heartbeatSound;

    /**
     * Map a heartbeat level to a heartbeat sound.
     */
    Soundname getSoundnameFromHeartbeat(int level, int minimum, int maximum);

public:
    /**
     * Play the specified sound.
     */
    void playSound(const Soundname& soundname);

    /**
     * Play the music specified as argument, stopping the current one·
     * If restart is false, then it resumes the playback from the latest point where it stopped.
     */
    void playMusic(const Musicname& musicname, bool restart = false);

    /**
     * Stop the game music.
     */
    void stopMusic();

    /**
     * Toggle the mute status of the music.
     */
    void toggleMute();

    /**
     * Play a random shoot (weapon fire) sound.
     */
    void playRandomShoot();

    /**
     *  Play a heartbeat sound, according to the specified level.
     */
    void playHeartbeat(int level, int minimum, int maximum);

    /**
     * Return the duration of the given sound.
     */
    sf::Time getSoundDuration(const Soundname& soundname) const;
};

}

#endif
