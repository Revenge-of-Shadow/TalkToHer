#ifndef DEFAULTS_H
#define DEFAULTS_H
#include "libs.hpp"

#ifdef _WIN32

const std::string kPathSepartor = "\\";

#else

const std::string kPathSepartor = "/";

#endif
const std::string optionsSeparator = ":";

const std::string font_foldername = "fonts";
const std::string scenario_foldername = "scenarios";
const std::string title_filename = "title.txt";
const std::string script_filename = "script.txt";
const std::string options_filename = "options.txt";
const std::string sprite_foldername = "sprites";
const std::string background_foldername = "backgrounds";
const std::string storage_foldername = "storage";
const std::string settings_filename = "settings.txt";
const std::string save_foldername = "saves";
const std::string last_filename = "lastsave.txt";
const std::string sound_foldername = "sounds";
const std::string music_foldername = "music";

const sf::Vector2u presetsize(1600, 900);
// Window loading is the first thing that the program does, sadly.

std::string forceSeparator(std::string str);
sf::Vector2f actualVector(sf::Vector2f windowsize, sf::Vector2f v);


namespace fsys = std::filesystem;
#endif
