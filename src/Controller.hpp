#include "Scenario.hpp"
#include "Textbox.hpp"
#include "libs.hpp"
#include <SFML/Window/Keyboard.hpp>

enum class State {Menu, Script, Options, Scenarios, Settings};

class Controller{
    void initMenu();
    void initScenarios();

public:
    sf::RenderWindow& window;
    sf::Sprite background;
    Textbox textbox;
    Shortlist<std::string> commandQueue; // FIFO filled during scenario.
    Scenario scenario;
    Shortlist<Option> options; // Filled with scenarios before showcase.
    int optionIndex;
    Shortlist<Textbox> optionboxes; // Filled just before showcase; drawn
    State state;

    void loadScenarios();
    void loadOptions();
    void loadScenario(std::string scenario_path);
    void loadScenarioPaths();
    void listOptions();

    Controller(sf::RenderWindow &w);

    void tryPrevLine();
    void tryNextLine();
    void tryCurrLine();

    bool processKey(sf::Event);
    void draw();
    void mainloop();
};

void Controller::initScenarios(){
    loadScenarioPaths();
    listOptions();
}
void Controller::initMenu(){
    options.erase();
    options.add(Option("Scenarios", "Scenario select"));
    options.add(Option("Quit", "Quit"));
    listOptions();
}


void Controller::loadScenario(std::string scenario_path){
    commandQueue.erase();
    scenario = Scenario(scenario_path);
    tryCurrLine();
    textbox.setString(scenario.getCurrentLineTrunc());
}

void Controller::loadScenarioPaths(){
    Shortlist<std::string> paths;
    for (const auto &entry: std::filesystem::directory_iterator(scenario_folder_suffix)){
        std::string path = entry.path();
        if(entry.is_directory() 
            && std::filesystem::exists(path+kPathSepartor+script_suffix))
                paths.add(
                    path.substr(path.find_first_of(kPathSepartor)+1)
                );
    }
    paths.sort();

    options.erase();
    for(int i = 0; i<paths.getSize(); ++i){
        options.add(Option(paths[i], paths[i]));
    }
}

void Controller::listOptions(){
    optionboxes.erase();
    for(int i = 0; i < options.getSize(); ++i){
        Textbox optionbox(
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
        optionboxes.add(optionbox);
    }
    if(options.getSize()){
        optionIndex = 0;
        (*optionboxes.getPtr(0)).choose();
    }
    else
        optionIndex = -1;
}


Controller::Controller(sf::RenderWindow &w): window(w){
    window.setFramerateLimit(FRAMERATE);

    textbox = Textbox(
        sf::Vector2f(windowSize.x/2.f, windowSize.y/8.f), 
        sf::Vector2f(windowSize.x/2.f, windowSize.y-windowSize.y/16.f), 
        "",
        FONTNAME,
        FONT_SIZE
    );

    initMenu();
    state = State::Menu;
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

bool Controller::processKey(sf::Event e){
    switch(state){
        case State::Menu:
            switch (e.key.code) {
                case sf::Keyboard::Up:
                    (*optionboxes.getPtr(optionIndex)).unchoose();
                    optionIndex == 0? 
                        optionIndex = options.getSize()-1
                        : --optionIndex;
                    (*optionboxes.getPtr(optionIndex)).choose();
                    break;
                case sf::Keyboard::Down:
                    (*optionboxes.getPtr(optionIndex)).unchoose();
                    optionIndex == options.getSize()-1?
                        optionIndex = 0
                        : ++optionIndex;
                    (*optionboxes.getPtr(optionIndex)).choose();
                    break;
                case sf::Keyboard::Enter:
                case sf::Keyboard::Right:
                    if(options[optionIndex].getPath() == "Scenarios"){
                        initScenarios();
                        state = State::Scenarios;
                    }
                    else if(options[optionIndex].getPath() == "Quit"){
                        return 1;
                    }
                    break;
            }
            break;
        case State::Script:
            switch (e.key.code) {
                case sf::Keyboard::Up:
                    textbox.scrollUp();
                    break;
                case sf::Keyboard::Down:
                    textbox.scrollDown();
                    break;
                case sf::Keyboard::Left: 
                    tryPrevLine();
                    textbox.setString(scenario.getCurrentLineTrunc());
                    break;
                case sf::Keyboard::Enter:
                case sf::Keyboard::Right:
                    if(scenario.getCurrentIndex() == scenario.getLines()-1){
                        options = scenario.getOptions();
                        switch(options.getSize()){
                            case 0: //  Scenario ends.
                                initMenu();
                                state = State::Menu;
                                break;
                            case 1: // Scenario provides one option.
                                loadScenario(options[0].getPath());
                                state = State::Script;
                                break;
                            default:
                                listOptions(); 
                                state = State::Options;
                                break;
                        }
                    }
                    else{//  Process commands and show text.
                        tryNextLine();
                        textbox.setString(scenario.getCurrentLineTrunc());
                    }
                    break;
            }
            break;
        case State::Options:
            switch (e.key.code) {
                case sf::Keyboard::Up:
                    (*optionboxes.getPtr(optionIndex)).unchoose();
                    optionIndex == 0? 
                        optionIndex = options.getSize()-1
                        : --optionIndex;
                    (*optionboxes.getPtr(optionIndex)).choose();
                    break;
                case sf::Keyboard::Down:
                    (*optionboxes.getPtr(optionIndex)).unchoose();
                    optionIndex == options.getSize()-1?
                        optionIndex = 0
                        : ++optionIndex;
                    (*optionboxes.getPtr(optionIndex)).choose();
                    break;
                case sf::Keyboard::Enter:
                case sf::Keyboard::Right:
                    loadScenario(options[optionIndex].getPath());
                    state = State::Script;
                    break;
                case sf::Keyboard::Left:
                    state = State::Script;
                    break;
            }
            break;
        case State::Scenarios:
            switch (e.key.code) {
                case sf::Keyboard::Up:
                    (*optionboxes.getPtr(optionIndex)).unchoose();
                    optionIndex == 0? 
                        optionIndex = options.getSize()-1
                        : --optionIndex;
                    (*optionboxes.getPtr(optionIndex)).choose();
                    break;
                case sf::Keyboard::Down:
                    (*optionboxes.getPtr(optionIndex)).unchoose();
                    optionIndex == options.getSize()-1?
                        optionIndex = 0
                        : ++optionIndex;
                    (*optionboxes.getPtr(optionIndex)).choose();
                    break;
                case sf::Keyboard::Enter:
                case sf::Keyboard::Right:
                    loadScenario(options[optionIndex].getPath());
                    state = State::Script;
                    break;
                case sf::Keyboard::Left:
                    initMenu();
                    state = State::Menu;
                    break;
            }
            break;
        case State::Settings:
            break;
    }
    return 0;
}

void Controller::draw(){
    window.clear();
    switch(state){
            case State::Menu:
               //   Add menu background. 
                for(int i = 0; i < options.getSize(); ++i){
                    window.draw(optionboxes[i]);
                }
                break;
            case State::Script:
                if(scenario.isBackgroundSet()){
                    window.draw(scenario.getBackground());
                }
                for(int i = 0; i<scenario.getCharsSize(); ++i)
                    window.draw(scenario.getChar(i));
                window.draw(textbox); 
                break;
            case State::Options:    //  Keep the script; draw over it.
                if(scenario.isBackgroundSet()){
                    window.draw(scenario.getBackground());
                }
                for(int i = 0; i<scenario.getCharsSize(); ++i)
                    window.draw(scenario.getChar(i));
                window.draw(textbox);

                //  Center the optionboxes. I refuse to elaborate.
                for(int i = 0; i < options.getSize(); ++i){
                    window.draw(optionboxes[i]);
                }
                break;
            case State::Scenarios:
                for(int i = 0; i < options.getSize(); ++i){
                    window.draw(optionboxes[i]);
                }
                break;
            case State::Settings:
                break;
        }
    window.display();
}

void Controller::mainloop(){
    if(commandQueue.getSize()) 
        scenario.processCommand(commandQueue.pop(0));
    draw();
}
