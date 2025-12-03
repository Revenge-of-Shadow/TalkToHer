#include "Settings.hpp"
#include "Scenario.hpp"
#include "Textbox.hpp"
#include "defaults.hpp"

enum class State {Menu, Script, Options, Scenarios, Settings};

class Controller{
    void initMenu();
    void initScenarios();
    void initSettings();

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

    Settings settings;

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
void Controller::initSettings(){
    options.erase();
    options.add(Option("", "Frame rate"));
    options.add(Option("", "Font size"));
    options.add(Option("", "Font name"));
    options.add(Option("", "Window resolution"));
    listOptions();
    settings.current = Setting::Framerate;  //  It it the first one.
}


//Make sure never to call that before loading.
bool Controller::saveSettings(){
    return settings.save(); 
}
bool Controller::loadSettings(){
    bool result = settings.load(); 

    window.setFramerateLimit(settings.framerate);
    window.setSize(settings.windowsize);
    performResize();   

    return result;
}


sf::Vector2f Controller::actualCenter(){
    return actualVector(sf::Vector2f(settings.windowsize), 
                sf::Vector2f(settings.windowsize.x/2.f, settings.windowsize.y/2.f));
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
    settings.windowsize = window.getSize();
    sf::View view = window.getDefaultView();
    view.setSize({static_cast<float>(settings.windowsize.x),
        static_cast<float>(settings.windowsize.y)});
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
    for(const auto &entry: 
            std::filesystem::directory_iterator(scenario_foldername)){
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
        pos.y = pos.y-settings.windowsize.y/2.f+settings.fontsize*8.f
            +settings.fontsize*4.f*
            (-float(options.getSize()/2 + options.getSize()%2)+0.5+i);

        Textbox optionbox(
            sf::Vector2f(
                settings.windowsize.x/2.f,
                settings.fontsize*3),
            pos,
            options.peek(i).text,
            settings.fontname,
            settings.fontsize,
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
        sf::Vector2f(settings.windowsize.x/2.f, settings.windowsize.y/8.f), 
        actualCenter()+
            sf::Vector2f(0.0f, settings.windowsize.y/2.f-settings.windowsize.y/16.f), 
        "",
        settings.fontname,
        settings.fontsize
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
                    if(options[optionIndex].val == "Scenarios"){
                        initScenarios();
                        state = State::Scenarios;
                    }
                    else if(options[optionIndex].val == "Settings"){
                        initSettings();
                        state = State::Settings;
                    }
                    else if(options[optionIndex].val == "Quit"){
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
                                void initSettings();
                            case 1: // Scenario provides one option.
                                loadScenario(options[0].val);
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
                    loadScenario(options[optionIndex].val);
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
                    loadScenario(options[optionIndex].val);
                    state = State::Script;
                    break;
                case sf::Keyboard::Left:
                    initMenu();
                    state = State::Menu;
                    break;
            }
            break;
        case State::Settings:
            switch (e.key.code) {
                case sf::Keyboard::Up:
                    if(settings.chosen){
                        settings.turnUp();
                    }
                    else{
                        settings.prev();
                        (*optionboxes.getPtr(optionIndex)).unchoose();
                        optionIndex == 0? 
                            optionIndex = options.getSize()-1
                            : --optionIndex;
                        (*optionboxes.getPtr(optionIndex)).choose();
                    }
                                        break;
                case sf::Keyboard::Down:
                    if(settings.chosen){
                        settings.turnDown();
                    }
                    else{
                        settings.next();
                        (*optionboxes.getPtr(optionIndex)).unchoose();
                        optionIndex == options.getSize()-1?
                            optionIndex = 0
                            : ++optionIndex;
                        (*optionboxes.getPtr(optionIndex)).choose();
                    }
                    break;
                case sf::Keyboard::Enter:
                case sf::Keyboard::Right:
                    if(settings.chosen)
                        (*optionboxes.getPtr(optionIndex)).unchoose();
                    else
                        (*optionboxes.getPtr(optionIndex)).choose();
                    settings.chosen = !settings.chosen;
                    break;
                case sf::Keyboard::Left:
                    saveSettings();
                    loadSettings();
                    initMenu();
                    state = State::Menu;
                    break;
            }
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
            for(int i = 0; i < options.getSize(); ++i){
                std::string temp = options[i].text+":    ";
                switch(i){
                    case 0:
                        temp+=std::to_string(settings.framerate);
                        break;
                    case 1:
                        temp+=std::to_string(settings.fontsize);
                        break;
                    case 2:
                        temp+=settings.fontname;
                        break;
                    case 3:
                        temp+=std::to_string(settings.windowsize.x)+
                            "x"+std::to_string(settings.windowsize.y);
                        break;
                }
                (*optionboxes.getPtr(i)).setString(temp);
                window.draw(optionboxes[i]);
            }
            break;
    }
    window.display();
}

void Controller::mainloop(){
    if(commandQueue.getSize()) 
        scenario.processCommand(commandQueue.pop(0));
    draw();
}
