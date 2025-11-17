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
    const char *title_suffix = "title";
    const char *script_suffix = "script";
    const char *options_suffix = "options";
    const char *postfix = ".txt";

    std::string filepath;
    std::ifstream filestream;
    std::string title; //  Extracted from file.
    int lineindex;
    SimpleList<std::string> lines;    //  This is... BAD. But, like, reliable.
    std::ifstream filestr;
    sf::RenderTarget &target;
    TextBox textbox;

    SimpleList<Character> characters;
    SimpleList<Sound> loopingSounds;
    Character getCharByName(std::string name);

    std::string truncLine(std::string command); //  Remove / or //.
    void processCommand(std::string command);

    void loadSprite(std::string charname, std::string spritename);

public:
      Scenario(std::string path, sf::RenderTarget &target, TextBox tb);

      int getCurrentIndex();
      std::string getCurrentLine();
      void showCurrentLine();
      bool toNextLine();
      bool toPrevLine();
      void playSound(std::string filename);
      void loopSound(std::string filename);
      int getLoopingSoundIndexByFilename(std::string filename); //  Do I need this?
      void stopSound(int index = 0);

      //  Simple string checks.
      bool isCurrLineCommand();
      bool isCurrLineComment();
};
#endif
