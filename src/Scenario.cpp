#include "Scenario.hpp"
#include "DrawableObject.hpp"
#include "libs.hpp"
#include <iostream>

DrawableObject Scenario::getCharByName(std::string name) {
    for (int i = 0; i < characters.getSize(); ++i){
        if (characters.peek(i).getName().compare(name) == 0)
            return characters.peek(i);
    }
    throw;
}
int Scenario::getCharIndexByName(std::string name) {
    for (int i = 0; i < characters.getSize(); ++i)
        if (characters.peek(i).getName().compare(name) == 0)
            return i;
    throw;
}

std::string Scenario::truncCommand(std::string command) { //   For /commands
    return command.substr(command.find('/')+1);
}
std::string Scenario::truncComment(std::string text){   //  For text with //comm
    return text.substr(0, text.find("//"));
}

void Scenario::loadBackground(std::string spritename) {
    background.setName("background");
    background.loadSprite(spritename);
    backgroundSet = true;
}
void Scenario::toggleBackground() { backgroundSet = !backgroundSet; }

DrawableObject* Scenario::getCharPtr(int index){ return characters.getPtr(index);}

int Scenario::getCharsSize(){ return characters.getSize(); }
DrawableObject Scenario::getChar(int index){ return characters.peek(index); }
bool Scenario::isBackgroundSet(){ return backgroundSet; }
DrawableObject Scenario::getBackground(){ return background; }
SimpleList<Option> Scenario::getOptions(){ 
    SimpleList<Option> options;

    filestr.open(filepath + options_suffix);
    std::string read_line;
    while(std::getline(filestr, read_line)){
        if(read_line.empty()) break;
        int found = read_line.find(optionsSeparator);
        if(found == read_line.npos){
            options.add(Option(read_line, ""));
        }
        else{
                options.add(Option(
                read_line.substr(found), 
                read_line.substr(found+1, read_line.length()-found)));
        }
    }
    filestr.close();
    
    return options;
}


Scenario::Scenario(std::string path)
    :filepath(scenario_folder_suffix + kPathSepartor + path + kPathSepartor),
    backgroundSet(false){

    filestr.open(filepath + title_suffix);
    std::getline(filestr, title);
    filestr.close();
    
    std::string line;
    filestr.open(filepath + script_suffix);
    
    while(filestr.is_open() && getline(filestr, line)){ //  Lags guaranteed.
        lines.add(line);
    }
    filestr.close();
    lineindex = 0;
}

Scenario* Scenario::operator*(){
    return this;
}

int Scenario::getCurrentIndex() { return lineindex; }
int Scenario::getLines() { return lines.getSize(); }
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

std::string getArgumentOut(std::string source){
    std::string commandArg = 
        source.substr(source.find_last_of('(')+1);
    commandArg = commandArg.substr(0, commandArg.find_last_of(')'));
    if(commandArg[0] == '\"' && commandArg[commandArg.length()-1] == '\"')
        commandArg = commandArg.substr(1, commandArg.length()-1);
    return commandArg;
}

void Scenario::processCommand(std::string command) {
    // char(Cat).loadSprite(cat_closedeyes.png);
    // background.loadSprite(room.png);
    // playSound(nya.mp3);
    std::string com = truncCommand(truncComment(command));
    std::cout<<"Processing command: "<< com <<std::endl;

    //  Formatting.
    std::string commandArg = getArgumentOut(com);
    std::string commandBody = com.substr(0, com.find_last_of('('));
    std::string commandObject = 
        commandBody.find('.') == std::string::npos? "" 
        : commandBody.substr(0, commandBody.find('.')); // NO NESTING
    std::string commandAction = 
        commandBody.find('.') == std::string::npos? commandBody
        : commandBody.substr(commandBody.find('.')+1);
    std::cout<<"\tCommand: "<<com<<std::endl;
    std::cout<<"\tBody: "<<commandBody<<"\t"<<"Arg: "<<commandArg<<std::endl;
    std::cout<<"\tObject: "<<commandObject<<"\t"<<"Action: "<<commandAction<<std::endl;

    //  Formatting end.

    //  Parsing.
    if(commandAction == "sleep")
        sf::sleep(sf::milliseconds(strtof(commandArg.c_str(), NULL)));

    if(commandObject == "background"){
        if(commandAction == "loadSprite"){
            loadBackground(commandArg);
        }
        else if(commandAction == "setPosition"){
            background.setPosition(sf::Vector2f(
                strtof(commandArg.substr(0, commandArg.find(',')).c_str(), NULL),
                strtof(commandArg.substr(commandArg.find(',')+1).c_str(), NULL)
            ));
        }
        else if(commandAction == "toggle"){
            toggleBackground();
        }
    }
    ////  char(charName)
    else if(commandAction == "char"){// Initialization only.
        characters.add(DrawableObject(commandArg));
    }
    else if(commandObject.substr(0, commandObject.find('(')) == "char"){
        std::string charName = getArgumentOut(commandObject);
        DrawableObject* ch = getCharPtr(getCharIndexByName(charName));
        if(commandAction == "loadSprite"){
            (*ch).loadSprite(commandArg);
        }
        else if(commandAction == "setPosition"){
            (*ch).setPosition(sf::Vector2f(
                strtof(commandArg.substr(0, commandArg.find(',')).c_str(), NULL),
                strtof(commandArg.substr(commandArg.find(',')+1).c_str(), NULL)
            ));
        }
        else if(commandAction == "move"){
            (*ch).move(sf::Vector2f(
                strtof(commandArg.substr(0, commandArg.find(',')).c_str(), NULL),
                strtof(commandArg.substr(commandArg.find(',')+1).c_str(), NULL)
            ));
        }
        else if(commandAction == "remove"){
            characters.pop(getCharIndexByName(charName));
        }
    }
    //  Parsing end.
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
    return getCurrentLine().length() > 2 
        && getCurrentLine().at(0) == '/' 
        && getCurrentLine().at(1) != '/';
}
bool Scenario::isCurrLineComment() {
    return getCurrentLine().length() > 2 
        && getCurrentLine().at(0) == '/' 
        && getCurrentLine().at(1) == '/';
}
bool Scenario::isCurrLineDisplayable(){
    return !getCurrentLineTrunc().empty()
        && !isCurrLineCommand() 
        && !isCurrLineComment();
}

