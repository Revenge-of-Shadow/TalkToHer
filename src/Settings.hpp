#ifndef SETTINGS_HPP
#define SETTINGS_HPP
#include "defaults.hpp"

enum class Setting{Framerate, Fontsize, Fontname, Windowsize};
class Settings{
public:
    Shortlist<sf::Vector2u> presetsizes;
    int sizeindex;
    Shortlist<std::string> fontnames;
    int fontindex;
    int framerate;
    int fontsize;
    std::string fontname;
    sf::Vector2u windowsize;
    Setting current;
    bool chosen;

    Settings();
    bool save();
    bool load();
    void next();
    void prev();
    void turnUp();
    void turnDown();
};
Settings::Settings(){
    for(const auto &entry: 
            fsys::directory_iterator(font_foldername)){
        std::string path = entry.path();
        if(path.substr(path.length()-4) == ".ttf")
            fontnames.add(
                    path.substr(path.find_first_of(kPathSepartor)+1)
                );
    }
    presetsizes.add(sf::Vector2u(1920, 1080));
    presetsizes.add(sf::Vector2u(1600, 900));
    presetsizes.add(sf::Vector2u(800, 600));
    current = Setting::Framerate;
    chosen = false;
}
bool Settings::save(){
    std::ofstream fstr(
        storage_foldername+kPathSepartor+settings_filename);
    if(!fstr.is_open()) return false;

    fstr<<framerate<<std::endl;
    fstr<<fontsize<<std::endl;
    fstr<<fontname<<std::endl;
    fstr<<windowsize.x<<std::endl;
    fstr<<windowsize.y<<std::endl;
    fstr.close();
    return true;
}
bool Settings::load(){
    std::ifstream fstr(
        storage_foldername+kPathSepartor+settings_filename);
    bool result = !fstr.is_open();
    if(!fstr.is_open()) {
        framerate = 60;
        fontsize = 24;
        fontname = "dm-serif-text-latin-400-normal.ttf";
        windowsize = sf::Vector2u(1600, 900);
    }
    else{
        fstr>>framerate;
        fstr>>fontsize;
        fstr>>fontname;
        fstr>>windowsize.x;
        fstr>>windowsize.y;
    }
    fstr.close();

    fontindex = fontnames.find(fontname);
    sizeindex = presetsizes.find(windowsize);

    return result;
}
void Settings::prev(){
    switch(current){
        case Setting::Framerate:
            current = Setting::Windowsize;
        break;
        case Setting::Fontsize:
            current = Setting::Framerate;
        break;
        case Setting::Fontname:
            current = Setting::Fontsize;
        break;
        case Setting::Windowsize:
            current = Setting::Fontname;
        break;
    }
}
void Settings::next(){
    switch(current){
        case Setting::Framerate:
            current = Setting::Fontsize;
        break;
        case Setting::Fontsize:
            current = Setting::Fontname;
        break;
        case Setting::Fontname:
            current = Setting::Windowsize;
        break;
        case Setting::Windowsize:
            current = Setting::Framerate;
        break;
    }
}
void Settings::turnUp(){
    switch(current){
        case Setting::Framerate:
            ++framerate;
        break;
        case Setting::Fontsize:
            ++fontsize;
        break;
        case Setting::Fontname:
            fontindex == 0?
                fontindex = fontnames.getSize()-1 : --fontindex;
            fontname = fontnames[fontindex];
        break;
        case Setting::Windowsize:
            sizeindex == 0?
                sizeindex = presetsizes.getSize()-1 : --sizeindex;
            windowsize = presetsizes[sizeindex];
        break;
    }
}
void Settings::turnDown(){
    switch(current){
        case Setting::Framerate:
            --framerate;
        break;
        case Setting::Fontsize:
            --fontsize;
        break;
        case Setting::Fontname:
            fontindex == fontnames.getSize()-1?
                fontindex = 0 : ++fontindex;
            fontname = fontnames[fontindex];
        break;
        case Setting::Windowsize:
            sizeindex == presetsizes.getSize()-1? 
                sizeindex = 0 : ++sizeindex;
            windowsize = presetsizes[sizeindex];
        break;
    }
}
#endif
