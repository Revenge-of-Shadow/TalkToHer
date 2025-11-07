#include "TextBox.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <vector>

class Character{};
class Sound{
    //  start thread
};

class Scenario{
    const char* filepath;
    //  fileptr to read it?
    //  some sort of index?
    //  Each line is a separate part.
    //  Scenario files can be named freely. Not just scene1.txt
    const char* title;  //  Extracted from file.
    sf::RenderTarget& target;
    std::vector<Character>  characters;
    std::vector<Sound> sounds;

    const char* truncLine(const char* command); //  Remove / or //.
    void processCommand(const char* command);
    void processText(const char* text);

    void loadSprite(const char * charname, const char* spritetitle);
 public:
    Scenario(const char* filepath, sf::RenderTarget& target):filepath(filepath), target(target){}
    // void showNextLine(TextBox& tb);
    // void showPrevLine(TextBox& tb);
    int getCurrentIndex();
    void showCurrentLine(TextBox& tb);
    void toNextLine();
    void toPrevLine();
    void playSound(const char* filename);
    void loopSound(const char* filename, int pos = 1);
    void stopSound(int pos = 0);

    bool isCurrLineCommand();   //  str.c_str[0]=='/'
};
