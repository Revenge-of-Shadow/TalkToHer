#include "libs.hpp"
#include "defaults.h"
#include "DrawableObject.hpp"
#include "TextBox.hpp"
#include "Scenario.hpp"


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

    SimpleList<std::string> commandQueue;
    Scenario sc = Scenario("test");
    sf::Sprite bg;

    while(w.isOpen()){
        sf::Event event;
        while(w.pollEvent(event)){
            if(event.type == sf::Event::Closed)
                w.close();
            //  Checks for pressed keys.
            if(event.type == sf::Event::KeyPressed){
                switch (event.key.code) {
                    case sf::Keyboard::Up:
                        tb.scrollUp();
                        break;
                    case sf::Keyboard::Down:
                        tb.scrollDown();
                        break;
                    case sf::Keyboard::Left: 
                        //  Roll back to the last text.
                        while(sc.toPrevLine() && !sc.isCurrLineDisplayable())
                        {}
                        //  But roll forth if it is the first line and a comment...
                        while(!sc.isCurrLineDisplayable() && sc.toNextLine())
                        {}
                        tb.setString(sc.getCurrentLineTrunc());
                        break;
                    case sf::Keyboard::Enter:
                    case sf::Keyboard::Right:   //  Process commands and show text.
                        while(sc.toNextLine()){
                            if(sc.isCurrLineComment())  continue;
                            if(sc.isCurrLineCommand()){
                                commandQueue.add(sc.getCurrentLine());
                            }
                            else break;
                        }
                        //  But roll back if it is the last line and a comment...
                        while(!sc.isCurrLineDisplayable() && sc.toPrevLine())
                        {}
                        tb.setString(sc.getCurrentLineTrunc());
                        break;
                }
            }
            //  Checks for pressed keys end.
        }

        if(commandQueue.getSize())  sc.processCommand(commandQueue.pop(0));
        
        w.clear();
        if(sc.isBackgroundSet()){
            w.draw(sc.getBackground());
        }
        for(int i = 0; i<sc.getCharsSize(); ++i)
            w.draw(sc.getChar(i));
        tb.draw(w); 
        w.display();
    }
    
    return EXIT_SUCCESS;
}


