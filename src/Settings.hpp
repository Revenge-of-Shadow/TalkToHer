#ifndef SETTINGS_HPP
#define SETTINGS_HPP
#include "defaults.hpp"
#include "libs.hpp"

enum class Setting{Framerate, Fontsize, Fontname, Windowsize, Menumusic};
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
    std::string menumusicpath;
    Shortlist<std::string> musicnames;
    int musicindex;
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

#endif
