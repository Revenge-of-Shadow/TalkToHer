#include "libs.hpp"
#include "TextBox.hpp"
#include "Scenario.hpp"

#define WIN_WIDTH   1600
#define WIN_HEIGHT  900
#define filename    "sprites/Unyu.png"
#define FONT_FILEPATH   "fonts/dm-serif-text-latin-400-normal.ttf"
#define FONT_SIZE   24

int main(){
    sf::RenderWindow w(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "Unyu");
    w.setVerticalSyncEnabled(true);

    sf::Texture t;

    if(!t.loadFromFile(filename))
        return EXIT_FAILURE;

    sf::Sprite s;
    s.setTexture(t);
    s.setOrigin(t.getSize().x/2.f, t.getSize().y);
    s.setPosition(sf::Vector2f(WIN_WIDTH/2.f, WIN_HEIGHT));


    TextBox tb = TextBox(
        sf::Vector2f(WIN_WIDTH/2.f, WIN_HEIGHT/8.f), 
        sf::Vector2f(WIN_WIDTH/2.f, WIN_HEIGHT-WIN_HEIGHT/16.f), 
        "Unyu",
        FONT_FILEPATH,
        FONT_SIZE
    );

    Scenario sc = Scenario("test");
    
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
                    case sf::Keyboard::Right:   //  Process commands and show text.
                        while(sc.toNextLine()){
                            if(sc.isCurrLineComment())  continue;
                            if(sc.isCurrLineCommand())
                                sc.processCommand(sc.getCurrentLine());
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

        
        w.clear();
        w.draw(s);
        tb.draw(w); 
        w.display();
    }
    
    return EXIT_SUCCESS;
}


