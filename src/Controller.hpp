#include "Scenario.hpp"
#include "TextBox.hpp"
#include "libs.hpp"

enum class State {Script, Options, Menu, Settings};

class Controller{

    public:
    sf::Window window;
    sf::Sprite background;
    TextBox textbox;
    Shortlist<std::string> commandQueue;
    Scenario scenario;
    Shortlist<Option> options;
    int option;
    Shortlist<TextBox> optionboxes;

    void loadMenu();
    void loadOptions();
    void loadScenario(std::string scenario_path);
    void listOptions();

    Controller();

    void tryPrevLine();
    void tryNextLine();
    void tryCurrLine();

    void mainloop();
};

void Controller::loadScenario(std::string scenario_path){
    scenario = Scenario(scenario_path);
    tryCurrLine();
    textbox.setString(scenario.getCurrentLineTrunc());
}
void Controller::listOptions(){
    for(int i = 0; i < options.getSize(); ++i){
        TextBox optionBox(
            sf::Vector2f(
                windowSize.x/2.f,
                FONT_SIZE*3),
            sf::Vector2f(windowSize.x/2, 
                         windowSize.y/3+(FONT_SIZE*4)*
                         (-float(options.getSize()/2 + options.getSize()%2)+0.5+i)),
            options.peek(i).getText(),
            FONTNAME,
            FONT_SIZE,
            true);                   
        optionboxes.add(optionBox);
    }

}

void Controller::tryPrevLine(){
    //  Roll back to the last text.
    while(scenario.toPrevLine() && !scenario.isCurrLineDisplayable()){}
    //  But roll forth if it is the first line and a comment...
    while(!scenario.isCurrLineDisplayable() && scenario.toNextLine()){}
}
void Controller::tryNextLine(){
    while(scenario.toNextLine()){
        if(scenario.isCurrLineComment())  continue;
        if(scenario.isCurrLineCommand()){
            commandQueue.add(scenario.getCurrentLine());
        }
        else break;
    }
    //  But roll back if it is the last line and a comment...
    while(!scenario.isCurrLineDisplayable() && scenario.toPrevLine()){}
}
void Controller::tryCurrLine(){
    if(!scenario.isCurrLineDisplayable()) tryNextLine();
}

void Controller::mainloop(){}
