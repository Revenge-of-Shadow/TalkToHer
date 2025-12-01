#include "libs.hpp"
#ifndef DEFAULTS_H
#define DEFAULTS_H

#ifdef _WIN32

const std::string kPathSepartor = "\\";

#else

#define FRAMERATE   24
#define FONT_SIZE   24
#define FONTNAME "dm-serif-text-latin-400-normal.ttf"

const std::string kPathSepartor = "/";
const std::string optionsSeparator = ":";

const std::string font_folder_suffix = "fonts";
const std::string scenario_folder_suffix = "scenarios";
const std::string title_suffix = "title.txt";
const std::string script_suffix = "script.txt";
const std::string options_suffix = "options.txt";
const std::string sprite_folder_suffix = "sprites";
const std::string background_folder_suffix = "backgrounds";

const sf::Vector2f windowSize(1600, 900);



#endif
#endif
