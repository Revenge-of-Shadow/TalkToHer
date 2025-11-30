#include "libs.hpp"
#include "defaults.h"
#include "DrawableObject.hpp"
#include "Textbox.hpp"
#include "Scenario.hpp"
#include <filesystem>

enum class State {Script, Options, Menu, Settings};

void tryPrevLine(Scenario* sc){
    //  Roll back to the last text.
    while((*sc).toPrevLine() && !(*sc).isCurrLineDisplayable()){}
    //  But roll forth if it is the first line and a comment...
    while(!(*sc).isCurrLineDisplayable() && (*sc).toNextLine()){}
}
void tryNextLine(Scenario *sc, Shortlist<std::string> *commandQueue){
    while((*sc).toNextLine()){
        if((*sc).isCurrLineComment())  continue;
        if((*sc).isCurrLineCommand()){
            (*commandQueue).add((*sc).getCurrentLine());
        }
        else break;
    }
    //  But roll back if it is the last line and a comment...
    while(!(*sc).isCurrLineDisplayable() && (*sc).toPrevLine()){}
}
void tryCurrLine(Scenario *sc, Shortlist<std::string> *commandQueue){
    if(!(*sc).isCurrLineDisplayable()) tryNextLine(sc, commandQueue);
}

void listScenarios(Shortlist<Option> &options){
    Shortlist<std::string> paths;
    for (const auto &entry: std::filesystem::directory_iterator(scenario_folder_suffix)){
        std::string path = entry.path();
        paths.add(path.substr(path.find_first_of(kPathSepartor)+1));
    }
    paths.sort();
    options.erase();
    for(int i = 0; i<paths.getSize(); ++i){
        options.add(Option(paths[i], paths[i]));
    }
}

void listOptions(const Shortlist<Option> &options, Shortlist<TextBox> &boxes){
    for(int i = 0; i<options.getSize(); ++i){
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
        boxes.add(optionBox);
    }
}

void loadScenario(Scenario& sc, Shortlist<std::string> &commandQueue,
                  TextBox &tb, std::string path){
    commandQueue.erase();
    sc=Scenario(path);
    tryCurrLine(&sc, &commandQueue);
    tb.setString(sc.getCurrentLineTrunc());
}

//  Open it; Try to read... If is not readable, keep listing.
//  Scenatio(path); sc.isCurrLineDisplayable(); tryNextLine();

int main(){
    sf::RenderWindow w(sf::VideoMode(windowSize.x, windowSize.y), "Unyu");
    w.setVerticalSyncEnabled(true);

    TextBox tb = TextBox(
        sf::Vector2f(windowSize.x/2.f, windowSize.y/8.f), 
        sf::Vector2f(windowSize.x/2.f, windowSize.y-windowSize.y/16.f), 
        "",
        FONTNAME,
        FONT_SIZE
    );

    Shortlist<std::string> commandQueue;
    Scenario sc = Scenario("test");
    sf::Sprite bg;
    State state = State::Menu;
    Shortlist<Option> options;
    int chosenOption = 0;
    Shortlist<TextBox> optionBoxes;
    // tryCurrLine(&sc, &commandQueue);
    // tb.setString(sc.getCurrentLineTrunc());
    listScenarios(options);
    listOptions(options, optionBoxes);

    while(w.isOpen()){
        sf::Event event;
        while(w.pollEvent(event)){
            if(event.type == sf::Event::Closed)
                w.close();
            if(event.type == sf::Event::KeyPressed){
                switch(state){
                    case State::Script:
                        switch (event.key.code) {
                            case sf::Keyboard::Up:
                                tb.scrollUp();
                                break;
                            case sf::Keyboard::Down:
                                tb.scrollDown();
                                break;
                            case sf::Keyboard::Left: 
                                tryPrevLine(&sc);
                                tb.setString(sc.getCurrentLineTrunc());
                                break;
                            case sf::Keyboard::Enter:
                            case sf::Keyboard::Right:
                                if(sc.getCurrentIndex() == sc.getLines()-1){
                                    options = sc.getOptions();
                                    switch(options.getSize()){
                                        case 0:
                                            state = State::Menu;
                                            listScenarios(options);
                                            listOptions(options, optionBoxes);

                                            break;
                                        case 1:
                                            loadScenario(sc, commandQueue, tb,
                                                         options.last().getPath());
                                            break;
                                        default:
                                            listOptions(options, optionBoxes); 
                                            state = State::Options;
                                            break;
                                    }
                                }
                                else{//  Process commands and show text.
                                    tryNextLine(&sc, &commandQueue);
                                    tb.setString(sc.getCurrentLineTrunc());
                                }
                                break;
                        }
                        break;
                    case State::Options:
                        //  options controls
                        switch (event.key.code) {
                            case sf::Keyboard::Up:
                                optionBoxes[chosenOption].unchoose();
                                chosenOption == 0? 
                                    chosenOption = options.getSize()-1
                                    : --chosenOption;
                                optionBoxes[chosenOption].choose();
                                break;
                            case sf::Keyboard::Down:
                                optionBoxes[chosenOption].unchoose();
                                chosenOption == options.getSize()-1?
                                    chosenOption = 0
                                    : ++chosenOption;
                                optionBoxes[chosenOption].choose();
                                break;
                            case sf::Keyboard::Enter:
                            case sf::Keyboard::Right:
                                loadScenario(sc, commandQueue, tb,
                                             options[chosenOption].getPath());
                                state = State::Script;
                                break;
                            case sf::Keyboard::Left:
                                state = State::Script;
                                break;
                        }
                        break;
                    case State::Menu:
                        switch (event.key.code) {
                            case sf::Keyboard::Up:
                                optionBoxes[chosenOption].unchoose();
                                chosenOption == 0? 
                                    chosenOption = options.getSize()-1
                                    : --chosenOption;
                                optionBoxes[chosenOption].choose();
                                break;
                            case sf::Keyboard::Down:
                                optionBoxes[chosenOption].unchoose();
                                chosenOption == options.getSize()-1?
                                    chosenOption = 0
                                    : ++chosenOption;
                                optionBoxes[chosenOption].choose();
                                break;
                            case sf::Keyboard::Enter:
                            case sf::Keyboard::Right:
                                std::cout<<"hey"<<std::endl;
                                loadScenario(sc, commandQueue, tb,
                                             options[chosenOption].getPath());
                                state = State::Script;
                                break;
                            case sf::Keyboard::Left:
                                break;
                        }
                        break;
                    case State::Settings:
                        break;
                }
            }
            //  Checks for pressed keys end.
        }

        if(commandQueue.getSize())  sc.processCommand(commandQueue.pop(0));

        w.clear();

        switch(state){
            case State::Script:
                if(sc.isBackgroundSet()){
                    w.draw(sc.getBackground());
                }
                for(int i = 0; i<sc.getCharsSize(); ++i)
                    w.draw(sc.getChar(i));
                w.draw(tb); 
                break;
            case State::Options:    //  Keep the script; draw over it.
                if(sc.isBackgroundSet()){
                    w.draw(sc.getBackground());
                }
                for(int i = 0; i<sc.getCharsSize(); ++i)
                    w.draw(sc.getChar(i));
                w.draw(tb);

                //  Center the optionboxes. I refuse to elaborate.
                for(int i = 0; i < options.getSize(); ++i){
                    w.draw(optionBoxes[i]);
                }
                break;
            case State::Menu:
                for(int i = 0; i < options.getSize(); ++i){
                    w.draw(optionBoxes[i]);
                }
                break;
            case State::Settings:
                break;
        }
        w.display();
    }

    return EXIT_SUCCESS;
}
