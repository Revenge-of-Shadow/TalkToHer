#include "Sound.hpp"
#include "defaults.hpp"

Sound::Sound(){music.stop();}
Sound::Sound(const Sound& other){
    filename = other.filename;

    music.stop();
    music.openFromFile(filename);
    music.setLoop(other.music.getLoop());
    music.setPlayingOffset(other.music.getPlayingOffset());
    if(other.music.Playing) music.play();
}
Sound::Sound(std::string filename, bool isLooping)
:filename(filename){
    this->filename = 
        sound_foldername+kPathSepartor+forceSeparator(filename);
    music.openFromFile(this->filename);
    music.setLoop(isLooping);
};
Sound::~Sound(){ 
    stop(); 
}
void Sound::play(){
    music.play();
};
void Sound::stop(){
    music.stop();
};
Sound& Sound::operator=(const Sound& other){
    if(&other != this){
        filename = other.filename;

        music.stop();
        music.openFromFile(filename);
        music.setLoop(other.music.getLoop());
        music.setPlayingOffset(other.music.getPlayingOffset());
        if(other.music.getStatus()==sf::Music::Playing) music.play();
    }
    return *this;
}
bool Sound::operator==(Sound other){
    return filename == other.filename;
}
