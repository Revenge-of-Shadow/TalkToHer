#include "Scenario.hpp"
#include "Textbox.hpp"
#include "defaults.hpp"
#include "libs.hpp"
#include <algorithm>

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

    int framerate;
    int fontsize;
    std::string fontname;
    sf::Vector2u windowsize;   //  SFML is freaking weird here.
    sf::Vector2f actualCenter();// I hate it.
    void performResize();//  It gets WORSE.

    bool saveSettings();
    bool loadSettings();

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
    options.add(Option("Settings", "Settings"));
    options.add(Option("Extras", "Extras"));
    options.add(Option("Quit", "Quit"));
    listOptions();
}

//Make sure never to call that before loading.
bool Controller::saveSettings(){
    std::ofstream filestr(settings_path);
    if(!filestr.is_open()) return false;

    filestr<<framerate<<std::endl;
    filestr<<fontsize<<std::endl;
    filestr<<fontname<<std::endl;
    filestr<<windowsize.x<<std::endl;
    filestr<<windowsize.y<<std::endl;
    filestr.close();
    return true;
}
bool Controller::loadSettings(){
    std::ifstream filestr(settings_path);
    bool result = !filestr.is_open();
    if(!filestr.is_open()) {
        framerate = 60;
        fontsize = 24;
        fontname = "dm-serif-text-latin-400-normal.ttf";
        windowsize = sf::Vector2u(800, 600);
    }
    else{
        filestr>>framerate;
        filestr>>fontsize;
        filestr>>fontname;
        filestr>>windowsize.x;
        filestr>>windowsize.y;
    }

    window.setFramerateLimit(framerate);
    window.setSize(windowsize);
    performResize();   

    filestr.close();
    return result;
}


sf::Vector2f Controller::actualCenter(){
    return actualVector(sf::Vector2f(windowsize), 
                sf::Vector2f(windowsize.x/2.f, windowsize.y/2.f));
}
void Controller::performResize(){
    // Ho, I hate it.
    //  Drawable.setPosition() relies on the non-resized value.
    //  Sending it updated size misplaces the object as if it
    //  was drawn inside the previous window, cropped with the
    //  new window.
    //  Ignoring the change makes the center good, but draws 
    //  outside of the frame.
    //  Applying the change misplaces left-top, moving view.
    windowsize = window.getSize();
    sf::View view = window.getDefaultView();
    view.setSize({static_cast<float>(windowsize.x),
        static_cast<float>(windowsize.y)});
    window.setView(view);
}

void Controller::loadScenario(std::string scenario_path){
    commandQueue.erase();
    scenario = Scenario(scenario_path);
    tryCurrLine();
    textbox.setString(scenario.getCurrentLineTrunc());
}

void Controller::loadScenarioPaths(){
    Shortlist<std::string> paths;
    for (const auto &entry: std::filesystem::directory_iterator(scenario_foldername)){
        std::string path = entry.path();
        if(entry.is_directory() 
            && std::filesystem::exists(path+kPathSepartor+script_filename))
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
        sf::Vector2f pos = actualCenter();
        pos.y = pos.y-windowsize.y/2.f+fontsize*8.f
            +fontsize*4.f*
            (-float(options.getSize()/2 + options.getSize()%2)+0.5+i);

        Textbox optionbox(
            sf::Vector2f(
                windowsize.x/2.f,
                fontsize*3),
            pos,
            options.peek(i).getText(),
            fontname,
            fontsize,
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
    loadSettings();

    textbox = Textbox(
        sf::Vector2f(windowsize.x/2.f, windowsize.y/8.f), 
        actualCenter()+
            sf::Vector2f(0.0f, windowsize.y/2.f-windowsize.y/16.f), 
        "",
        fontname,
        fontsize
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
