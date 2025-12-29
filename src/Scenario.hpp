#ifndef SCENARIO_H
#define SCENARIO_H
/** The class works with a directory
 *  containing title.txt, script.txt, options.txt.
 *  Must:
 *      return the title string
 *      parse the script (execute commands and return text)
 *          (including managing object sprites and music)
 *      read the options and allow the choice.
 *
 */
#include "libs.hpp"
#include "defaults.hpp"
#include "DrawableObject.hpp"
#include "Option.hpp"
#include "Sound.hpp"



class Scenario {
    std::string filepath;
    std::ifstream fstream;
    std::string title; //  Extracted from file.
    int lineindex;
    Shortlist<std::string> lines;    //  This is... BAD. But, like, reliable.
    std::ifstream fstr;

    Shortlist<DrawableObject> objects;
    Shortlist<Sound> sounds;
    
    sf::Vector2f getRelativeVector(std::string);
    int getObjIndexByName(std::string name);
    DrawableObject getObjByName(std::string name);

    std::string truncCommand(std::string command);  //  Remove / from front.
    std::string truncComment(std::string text);     //  Remove // from back.


public:
    int getObjsSize();   //  For drawing.
    DrawableObject getObj(int index);   //  Read-only.
    DrawableObject* getObjPtr(int index); // For altering from inside of Scenario.
    Shortlist<Option> getOptions();
    //  "test" leads to opening "scenarios/test/script.txt"
    Scenario();
    Scenario(std::string path);

    int getCurrentIndex();
    int getLines();
    std::string getCurrentLine();
    std::string getCurrentLineTrunc();  //  Without commentary.
    bool toNextLine();
    bool toPrevLine();
    void processCommand(std::string command);
    void playSound(std::string filename, bool looping = false);
    void stopSound(std::string filename);

    //  Simple string checks.
    bool isCurrLineCommand();
    bool isCurrLineComment();
    bool isCurrLineDisplayable();

    std::string getPath();
    std::string getTitle();

};
#endif
