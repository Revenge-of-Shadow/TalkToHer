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
            std::filesystem::directory_iterator(font_foldername)){
        std::string path = entry.path();
        std::cout<<path<<std::endl;
        if(path.substr(path.length()-4) == ".ttf")
            fontnames.add(
                    path.substr(path.find_first_of(kPathSepartor)+1)
                );
    }
    presetsizes.add(sf::Vector2u(1600, 900));
    presetsizes.add(sf::Vector2u(800, 600));
    current = Setting::Framerate;
    chosen = false;
}
bool Settings::save(){
    std::ofstream filestr(settings_path);
    if(!filestr.is_open()) return false;

    filestr<<framerate<<std::endl;
    filestr<<fontsize<<std::endl;
    filestr<<fontname<<std::endl;
    filestr<<windowsize.x<<std::endl;
    filestr<<windowsize.y<<std::endl;
    filestr.close();
    return true;
}
bool Settings::load(){
    std::ifstream filestr(settings_path);
    bool result = !filestr.is_open();
    if(!filestr.is_open()) {
        framerate = 60;
        fontsize = 24;
        fontname = "dm-serif-text-latin-400-normal.ttf";
        windowsize = sf::Vector2u(1600, 900);
    }
    else{
        filestr>>framerate;
        filestr>>fontsize;
        filestr>>fontname;
        filestr>>windowsize.x;
        filestr>>windowsize.y;
    }
    filestr.close();

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
