#include "Scenario.hpp"
#include <string>

Character Scenario::getCharByName(std::string name) {
    for (int i = 0; i < characters.getSize(); ++i)
        if (characters.peek(i).getName().compare(name) == 0)
            return characters.peek(i);
    throw;
}

std::string Scenario::truncCommand(std::string command) { //   For /commands
    return command.substr(command.find('/')+1);
}
std::string Scenario::truncComment(std::string text){   //  For text with //comm
    return text.substr(0, text.find("//"));
}


void loadSprite(std::string charname, std::string spritename) {}

Scenario::Scenario(std::string path)
        : filepath(path) {

    filestr.open(path + title_suffix + postfix);
    std::getline(filestr, title);
    filestr.close();
    
    std::string line;
    filestr.open(path + script_suffix + postfix);
        std::cout<<path + script_suffix + postfix<<std::endl;
    
    while(filestr.is_open() && getline(filestr, line)){ //  Lags guaranteed.
        lines.add(line);
    }
    //  Closed automatically.
    lineindex = 0;
}

int Scenario::getCurrentIndex() { return lineindex; }
std::string Scenario::getCurrentLine() { return lines.peek(lineindex); }
std::string Scenario::getCurrentLineTrunc() { return truncComment(lines.peek(lineindex)); }
bool Scenario::toNextLine() {
    if(lineindex < lines.getSize()-1){
        ++lineindex;
        return true;
    }
    return false;
}
bool Scenario::toPrevLine() {
    if (lineindex) {
        --lineindex;
        return true;
    }
    return false;
}

void Scenario::processCommand(std::string command) {
    // char(Cat).loadSprite(cat_closedeyes.png);
    // background.loadSprite(room.png);
    // playSound(nya.mp3);
    std::string com = truncCommand(truncComment(command));
    std::cout<<"Processing command: "<< com <<std::endl;
}

void Scenario::playSound(std::string filename) {
    Sound(filename).play(); //  I wonder if it shall break if I just never store it.
}
void Scenario::loopSound(std::string filename) {
    int ind = loopingSounds.add(Sound(filename, true));
    loopingSounds.peek(ind).play();
}
void Scenario::stopSound(int index) {
    if (index < 0 || index >= loopingSounds.getSize())
        throw;
    loopingSounds.pop(index).stop();
}

bool Scenario::isCurrLineCommand() {
    return Scenario::getCurrentLine().length() > 2 
        && Scenario::getCurrentLine().at(0) == '/' 
        && Scenario::getCurrentLine().at(1) != '/';
}
bool Scenario::isCurrLineComment() {
    return Scenario::getCurrentLine().length() > 2 
        && Scenario::getCurrentLine().at(0) == '/' 
        && Scenario::getCurrentLine().at(1) == '/';
}

