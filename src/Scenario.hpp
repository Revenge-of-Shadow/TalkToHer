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
#include "DrawableObject.hpp"
#include "Sound.hpp"
#include "TextBox.hpp"
#include "libs.hpp"
#include "defaults.h"


class Scenario {
    std::string filepath;
    std::ifstream filestream;
    std::string title; //  Extracted from file.
    int lineindex;
    SimpleList<std::string> lines;    //  This is... BAD. But, like, reliable.
    std::ifstream filestr;


    sf::Texture backgroundTexture;
    DrawableObject background;
    SimpleList<DrawableObject> characters;
    SimpleList<Sound> loopingSounds;
    
    bool backgroundSet; //  For drawing check.

    int getCharIndexByName(std::string name);
    DrawableObject getCharByName(std::string name);

    std::string truncCommand(std::string command);  //  Remove / from front.
    std::string truncComment(std::string text);     //  Remove // from back.

    void loadBackground(std::string spritename);
    void toggleBackground();    //  Just disable via bool.
    DrawableObject* getCharPtr(int index); // For altering from inside of Scenario.

public:
    int getCharsSize();   //  For drawing.
    DrawableObject getChar(int index);   //  Read-only.
    bool isBackgroundSet();//  For drawing check.
    DrawableObject getBackground();// Read-only.
    //  "test" leads to opening "scenarios/test/script.txt"
    Scenario(std::string path);

    int getCurrentIndex();
    std::string getCurrentLine();
    std::string getCurrentLineTrunc();  //  Without commentary.
    bool toNextLine();
    bool toPrevLine();
    void processCommand(std::string command);
    void playSound(std::string filename);
    void loopSound(std::string filename);
    int getLoopingSoundIndexByFilename(std::string filename); //  Do I need this?
    void stopSound(int index = 0);

    //  Simple string checks.
    bool isCurrLineCommand();
    bool isCurrLineComment();
    bool isCurrLineDisplayable();
};
#endif
