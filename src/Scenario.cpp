#include "Scenario.hpp"

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
void loadBackground(std::string spritename) {}

Scenario::Scenario(std::string path)
        : filepath(path) {

    filestr.open(path + title_suffix);
    std::getline(filestr, title);
    filestr.close();
    
    std::string line;
    filestr.open(path + script_suffix);
    
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

    //  Formatting.
    std::string commandArg = 
        com.substr(com.find_last_of('(')+1);
    commandArg = commandArg.substr(0, commandArg.find_last_of(')'));
    std::string commandBody = com.substr(0, com.find_last_of('('));
    std::string commandObject = 
        commandBody.find('.') == std::string::npos? "" 
        : commandBody.substr(0, commandBody.find('.')); // NO NESTING
    std::string commandAction = 
        commandBody.find('.') == std::string::npos? commandBody
        : commandBody.substr(commandBody.find('.')+1);
    std::cout<<"Command: "<<com<<std::endl;
    std::cout<<"Body: "<<commandBody<<"\t"<<"Arg: "<<commandArg<<std::endl;
    std::cout<<"Object: "<<commandObject<<"\t"<<"Action: "<<commandAction<<std::endl;

     if(commandArg[0] == '\"' && commandArg[commandArg.length()-1] == '\"')
        commandArg = commandArg.substr(1, commandArg.length()-1);
    //  Formatting end.

    //  Parsing.
    if(commandAction == "sleep")
        sf::sleep(sf::milliseconds(strtof(commandArg.c_str(), NULL)));
    if(commandAction.substr(commandAction.find('.')+1) == "loadSprite"){
        if(commandAction.substr(0, commandAction.find('.')) == "background"){
            // loadBackground(commandArg);
        }
/*        else if(commandAction.substr(0, commandAction.find('(')) == "char"){
            std::string charName = commandAction.substr(
                commandAction.
            try {
                Character ch = getCharByName(commandArg);
            } catch (int n) {
                ch = Character()
            }
        }
*/
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

