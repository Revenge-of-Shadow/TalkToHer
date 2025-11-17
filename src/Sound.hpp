#ifndef SOUND_H
#define SOUND_H
#include "libs.hpp"

class Sound{
    std::string filename;
    bool isLooping;

    void loadFromFile(){};
public:
    Sound(bool isLooping = false):isLooping(isLooping){
        loadFromFile();
    };
    Sound(std::string filename, bool isLooping = false)
        :filename(filename), isLooping(isLooping){
        loadFromFile();
    };
    ~Sound(){
        //  If is loopimg
        //      stop
        //      kill
        //  If is playing
        //      wait
        //  else
        //      kill    
    }
    void play(){
        if(isLooping) return;
        else return;
    };
    void stop(){
    };
};
#endif
