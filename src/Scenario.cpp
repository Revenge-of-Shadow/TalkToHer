#include "Scenario.hpp"
#include "DrawableObject.hpp"
#include "Sound.hpp"
#include "defaults.hpp"

sf::Vector2f Scenario::getRelativeVector(std::string commandArg){
    return sf::Vector2f(
        strtof(commandArg.substr(0, 
                                 commandArg.find(',')).c_str(), NULL),
        strtof(commandArg.substr(
            commandArg.find(',')+1).c_str(), NULL)
    );
}

int Scenario::getObjIndexByName(std::string name) {
    for (int i = 0; i < objects.getSize(); ++i)
        if (objects.peek(i).getName().compare(name) == 0)
            return i;
    return -1; 
}
DrawableObject Scenario::getObjByName(std::string name) {
    for (int i = 0; i < objects.getSize(); ++i){
        if (objects.peek(i).getName().compare(name) == 0)
            return objects.peek(i);
    }
    throw;
}

std::string Scenario::truncCommand(std::string command) { //   For /commands
    return command.substr(command.find('/')+1);
}
std::string Scenario::truncComment(std::string text){   //  For text with //comm
    return text.substr(0, text.find("//"));
}

DrawableObject* Scenario::getObjPtr(int index){ return objects.getPtr(index);}

int Scenario::getObjsSize(){ return objects.getSize(); }
DrawableObject Scenario::getObj(int index){ return objects.peek(index); }
Shortlist<Option> Scenario::getOptions(){ 
    Shortlist<Option> options;

    fstr.open(filepath + kPathSepartor + options_filename);
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
Scenario::Scenario(std::string path){     //  Stupid damned assigmnent forcing.
    filepath = (scenario_foldername + kPathSepartor 
              + forceSeparator(path));

    std::cout<<"Constructing scenario: <"<<filepath<<">"<<std::endl;

    fstr.open(filepath + kPathSepartor + title_filename);
    if(!fstr.is_open()){
        title = "";
    }
    else{
        std::getline(fstr, title);
    }
    fstr.close();

    std::string line;
    fstr.open(filepath + kPathSepartor + script_filename);

    while(fstr.is_open() && getline(fstr, line)){ //  Lags guaranteed.
        if(!line.empty())
            lines.add(line);
        else
            lines.add("...");
    }
    fstr.close();

    lineindex = 0;

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
        commandArg = commandArg.substr(1, commandArg.length()-2);
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
    if(commandAction == "play"){
        playSound(commandArg, false);
    }
    else if(commandAction == "loop"){
        playSound(commandArg, true);
    }
    else if(commandAction == "stop"){
        stopSound(commandArg);
    }
    else if(commandAction == "sleep")
        sf::sleep(sf::milliseconds(strtof(commandArg.c_str(), NULL)));

        ////  object(objectName)
    else if(commandAction == "object"){// Initialization only.
        if(getObjIndexByName(commandArg) == -1) // Only if does not exist yet.
            objects.add(DrawableObject(commandArg));
    }
    else if(commandObject.substr(0, commandObject.find('(')) == "object"){
        std::string objectName = getArgumentOut(commandObject);
        DrawableObject* obj = getObjPtr(getObjIndexByName(objectName));
        if(commandAction == "loadSprite"){
            (*obj).loadSprite(commandArg);
        }
        else if(commandAction == "setScale"){
            (*obj).setScale(getRelativeVector(commandArg));
        }
        else if(commandAction == "setPosition"){
            (*obj).setPosition(getRelativeVector(commandArg));
        }
        else if(commandAction == "move"){
            (*obj).move(getRelativeVector(commandArg));
        }
        else if(commandAction == "remove"){
            objects.pop(getObjIndexByName(objectName));
        }
    }
    //  Parsing end.
}

void Scenario::playSound(std::string filename, bool looping) {
    sounds.add(Sound(filename, looping));
    (*sounds.getPtr(sounds.getSize()-1)).play();
}
void Scenario::stopSound(std::string filename) {
    int index = sounds.find(Sound(filename));
    if (index < 0 || index >= sounds.getSize())
        throw;
    sounds.pop(index);
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

std::string Scenario::getPath(){
    //  I think it is right to return semi-relative path. Bite me!
    return filepath.substr(filepath.find_first_of(kPathSepartor)+1);
}
std::string Scenario::getTitle(){return title;}

