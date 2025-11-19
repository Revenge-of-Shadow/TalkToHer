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
#include "Character.hpp"
#include "Sound.hpp"
#include "TextBox.hpp"
#include "libs.hpp"


class Scenario {
    const std::string title_suffix = "title.txt";
    const std::string script_suffix = "script.txt";
    const std::string options_suffix = "options.txt";

    std::string filepath;
    std::ifstream filestream;
    std::string title; //  Extracted from file.
    int lineindex;
    SimpleList<std::string> lines;    //  This is... BAD. But, like, reliable.
    std::ifstream filestr;

    SimpleList<Character> characters;
    SimpleList<Sound> loopingSounds;
    Character getCharByName(std::string name);

    std::string truncCommand(std::string command);  //  Remove / from front.
    std::string truncComment(std::string text);     //  Remove // from back.

    void loadSprite(std::string charname, std::string spritename);
    void loadBackground(std::string spritename);

public:
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
