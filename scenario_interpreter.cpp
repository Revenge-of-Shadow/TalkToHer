#include "TextBox.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <vector>

class Character{};

class Scenario{
    const char* filepath;
    //  fileptr to read it?
    //  some sort of index?
    //  Each line is a separate part.
    //  Scenario files can be named freely. Not just scene1.txt
    sf::RenderTarget& target;
    std::vector<Character>  characters;
    void processCommand(const char* command);
    void loadSprite(const char * charname, const char* spritetitle);
 public:
    Scenario(const char* filepath, sf::RenderTarget& target):filepath(filepath), target(target){}
    void showNextLine(TextBox& tb);
    void showPrevLine(TextBox& tb);
};
