#include "Scenario.hpp"
#include "DrawableObject.hpp"
#include "defaults.hpp"

sf::Vector2f Scenario::getRelativeVector(std::string commandArg){
    return sf::Vector2f(
        strtof(commandArg.substr(0, 
            commandArg.find(',')).c_str(), NULL),
        strtof(commandArg.substr(
            commandArg.find(',')+1).c_str(), NULL)
    );
}

int Scenario::getCharIndexByName(std::string name) {
    for (int i = 0; i < characters.getSize(); ++i)
        if (characters.peek(i).getName().compare(name) == 0)
            return i;
    throw;
}
DrawableObject Scenario::getCharByName(std::string name) {
    for (int i = 0; i < characters.getSize(); ++i){
        if (characters.peek(i).getName().compare(name) == 0)
            return characters.peek(i);
    }
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
Shortlist<Option> Scenario::getOptions(){ 
    Shortlist<Option> options;

    fstr.open(filepath + options_filename);
    std::string read_line;
    while(std::getline(fstr, read_line)){
        if(read_line.empty()) break;
        int found = read_line.find(optionsSeparator);
        if(found == read_line.npos){
            options.add(Option(read_line, ""));
        }
        else{
            options.add(Option(
                read_line.substr(0,found), 
                read_line.substr(found+1, read_line.length()-found)));
        }
    }
    fstr.close();

    return options;
}


Scenario::Scenario(){}
Scenario::Scenario(std::string path)
    :filepath(scenario_foldername + kPathSepartor 
              + forceSeparator(path) + kPathSepartor),
    backgroundSet(false){
    std::cout<<"Constructing scenario: "<<path<<std::endl;

    fstr.open(filepath + title_filename);
    if(!fstr.is_open()){
        title = "";
    }
    else{
        std::getline(fstr, title);
    }
    fstr.close();

    std::string line;
    fstr.open(filepath + script_filename);
    
    while(fstr.is_open() && getline(fstr, line)){ //  Lags guaranteed.
        if(!line.empty())
            lines.add(line);
        else
            lines.add("...");
    }
    fstr.close();

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

    //  Formatting end.

    //  Parsing.
    if(commandAction == "sleep")
        sf::sleep(sf::milliseconds(strtof(commandArg.c_str(), NULL)));

    if(commandObject == "background"){
        if(commandAction == "loadSprite"){
            loadBackground(commandArg);
        }
        else if(commandAction == "setPosition"){
            background.setPosition(getRelativeVector(commandArg));
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
            (*ch).setPosition(getRelativeVector(commandArg));
        }
        else if(commandAction == "move"){
            (*ch).move(getRelativeVector(commandArg));
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

std::string Scenario::getTitle(){return title;}
