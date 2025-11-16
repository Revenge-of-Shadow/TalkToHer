#include "Scenario.hpp"
#include <string>

Character Scenario::getCharByName(std::string name) {
  for (int i = 0; i < characters.getSize(); ++i)
    if (characters.peek(i).getName().compare(name) == 0)
      return characters.peek(i);
  throw;
}

std::string Scenario::truncLine(std::string command) {
  return command.substr(command.find('/'));
}
void Scenario::processCommand(std::string command) {
  // char(Cat).loadSprite(cat_closedeyes.png);
  // background.loadSprite(room.png);
  // playSound(nya.mp3);
}

void loadSprite(std::string charname, std::string spritename) {}

std::string Scenario::readCurrentLine() {
  // return filestr.getline(lineindex);
  return "";
}
int Scenario::getCurrentIndex() { return lineindex; }
std::string Scenario::getCurrentLine() { return line; }
void Scenario::showCurrentLine() { textbox.setString(line); }
bool Scenario::toNextLine() {
  // if (eof)
  //   return false else {
  //     ++lineindex;
  //     return true
  //   }
    return false;
}
bool Scenario::toPrevLine() {
    if(lineindex){
        --lineindex;
        return true;
    }
    return false;
}

void Scenario::playSound(std::string filename){
    Sound(filename).play(); //  I wonder if it shall break if I just never store it.
}
void Scenario::loopSound(std::string filename){
    int ind = loopingSounds.add(Sound(filename, true));
    loopingSounds.peek(ind).play();
}
void Scenario::stopSound(int index){
    if(index < 0 || index >= loopingSounds.getSize())
        throw;
    loopingSounds.pop(index).stop();
}

bool Scenario::isCurrLineCommand() {
  return line.length() > 2 && line.at(0) == '/' && line.at(1) != '/';
}
bool Scenario::isCurrLineComment() {
  return line.length() > 2 && line.at(0) == '/' && line.at(1) == '/';
}
