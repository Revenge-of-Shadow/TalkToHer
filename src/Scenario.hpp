#ifndef SCENARIO_H
#define SCENARIO_H
/** The class works with a directory
 *  containing title.txt, script.txt, options.txt.  
 *  Must:
 *      return the title string
 *      parse the script (execute commands and return text)
 *          (including managing character sprites and music)
 *      read the options and allow the choice.
*
*/
#include "libs.hpp"
#include "Character.hpp"
#include <string>

class Sound{
    bool isLooping;
public:
    Sound();
    void play();
    void stop();
};

class Scenario{
    std::string filepath;
    std::string title;  //  Extracted from file.
    std::string line;
    sf::RenderTarget& target;
    std::vector<Character>  characters;

    std::string truncLine(std::string command); //  Remove / or //.
    void processCommand(std::string command);
    void processText(std::string text);

    void loadSprite(std::string charname, std::string spritetitle);
 public:
    Scenario(std::string filepath, sf::RenderTarget& target):filepath(filepath), target(target){}
    // void showNextLine(TextBox& tb);
    // void showPrevLine(TextBox& tb);
    int getCurrentIndex();
    void showCurrentLine();
    void toNextLine();
    void toPrevLine();
    void playSound(std::string filename);
    void loopSound(std::string filename, int pos = 1);
    void stopSound(int pos = 0);

    //  Simple string checks.
    bool isCurrLineCommand();
    bool isCurrLineComment();
};
#endif
