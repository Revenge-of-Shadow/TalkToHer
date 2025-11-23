#include "Scenario.hpp"
#include "libs.hpp"
Character Scenario::getCharByName(std::string name) {
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

void loadSprite(std::string charname, std::string spritename) {}
void loadBackground(std::string spritename) {}

Character* Scenario::getCharPtr(int index){ return characters.getPtr(index);}

int Scenario::getCharsSize(){ return characters.getSize(); }
Character Scenario::getChar(int index){ return characters.peek(index); }


Scenario::Scenario(std::string path)
    :filepath(scenario_folder_suffix + kPathSepartor + path + kPathSepartor){

    filestr.open(filepath + title_suffix);
    std::getline(filestr, title);
    filestr.close();
    
    std::string line;
    filestr.open(filepath + script_suffix);
    
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
    if(commandAction == "loadBackground"){
            // loadBackground(commandArg);
    }
    //  char(charName)
    else if(commandAction == "char"){// Initialization only.
        characters.add(Character(commandArg));
    }
    else if(commandObject.substr(0, commandObject.find('(')) == "char"){
        std::string charName = getArgumentOut(commandObject);
        Character* ch = getCharPtr(getCharIndexByName(charName));
        if(commandAction == "loadSprite"){
            (*ch).loadSprite(commandArg);
        }
        else if(commandAction == "setPos"){
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

