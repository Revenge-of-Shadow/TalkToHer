#include "defaults.hpp"
#ifndef SOUND_H
#define SOUND_H

class Sound{
    sf::Music music;

public:
    std::string filename;
    Sound();
    Sound(const Sound& other);
    Sound(std::string filename, bool isLooping = false);
    ~Sound();
    void play();
    void stop();
    Sound& operator=(const Sound& other);
    bool operator==(Sound other);

};  
#endif

