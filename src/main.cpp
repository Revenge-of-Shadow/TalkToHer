#include "libs.hpp"
#include "defaults.h"
#include "DrawableObject.hpp"
#include "TextBox.hpp"
#include "Scenario.hpp"

enum class State {Script, Options, Menu, Settings};

void tryPrevLine(Scenario* sc){
    //  Roll back to the last text.
    while((*sc).toPrevLine() && !(*sc).isCurrLineDisplayable()){}
    //  But roll forth if it is the first line and a comment...
    while(!(*sc).isCurrLineDisplayable() && (*sc).toNextLine()){}
}
void tryNextLine(Scenario *sc, SimpleList<std::string> *commandQueue){
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
    tb.setFillColor(sf::Color(0,0,0, 0x7F));
    tb.setOutlineColor(sf::Color(0xFF, 0xFF, 0xFF, 0x7F));

    SimpleList<std::string> commandQueue;
    Scenario sc = Scenario("test");
    sf::Sprite bg;
    State state = State::Script;
    SimpleList<Option> options;
    int chosenOption = 0;
    SimpleList<TextBox> optionBoxes;

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
                            case sf::Keyboard::Right://  Process commands and show text.
                                if(sc.getCurrentIndex() == sc.getLines()-1){
                                    state = State::Options;
                                    options = sc.getOptions();
                                    for(int i = 0; i<options.getSize(); ++i){
                                        optionBoxes.add(
                                            TextBox(
                                                sf::Vector2f(
                                                    tb.rect.getSize().x*1.25f,
                                                    FONT_SIZE*2),
                                                sf::Vector2f(windowSize.x/2, 
                                                             windowSize.y/2+(FONT_SIZE*3)*
                                                             (-float(options.getSize()/2 + options.getSize()%2)+0.5+i)),
                                                options.peek(i).getText().empty()? 
                                                "..." : options.peek(i).getText(),
                                                FONTNAME,
                                                FONT_SIZE));                   
                                    }
                                }
                                else{
                                    tryNextLine(&sc, &commandQueue);
                                    tb.setString(sc.getCurrentLineTrunc());
                                }
                                break;
                        }
                        break;
                    case State::Options:
                        std::cout<<"options: "<<options.getSize()<<std::endl;
                        switch(options.getSize()){
                            case 0:
                                state = State::Menu;
                                break;
                            case 1:
                                if(options.peek(0).getText().empty()){
                                    std::cout<<"New path:"<<options.peek(0).getPath()<<std::endl;
                                    // sc=Scenario(options.peek(0).getPath());
                                    state = State::Script;
                                }
                                break;
                            default:
                                //  options controls
                                switch (event.key.code) {
                                    case sf::Keyboard::Up:
                                        chosenOption == 0? 
                                            chosenOption = options.getSize()-1
                                            : --chosenOption;
                                        break;
                                    case sf::Keyboard::Down:
                                        chosenOption == options.getSize()-1?
                                            chosenOption = 0
                                            : ++chosenOption;
                                        break;
                                    case sf::Keyboard::Enter:
                                    case sf::Keyboard::Right:
                                        std::cout<<"Wait! Do not go!"<<std::endl;
                                        sc = Scenario(options.peek(0).getPath());
                                        state = State::Script;
                                        break;
                                    case sf::Keyboard::Left:
                                        state = State::Script;
                                        break;
                                }
                                break;
                        }
                        break;
                    case State::Menu:
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
                tb.draw(w); 
                break;
            case State::Options:    //  Keep the script; draw over it.
                if(sc.isBackgroundSet()){
                    w.draw(sc.getBackground());
                }
                for(int i = 0; i<sc.getCharsSize(); ++i)
                    w.draw(sc.getChar(i));
                tb.draw(w);
                std::cout<<"-1"<<std::endl;

                //  Center the optionboxes. I refuse to elaborate.
                for(int i = 0; i < options.getSize(); ++i){
                    (*optionBoxes.getPtr(i)).draw(w);
                }
                break;
            case State::Menu:
                break;
            case State::Settings:
                break;
        }

        w.display();
    }
    
    return EXIT_SUCCESS;
}
