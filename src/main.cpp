#include "TextBox.hpp"
#include <SFML/System/Vector2.hpp>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <ostream>
#include <string>

#define WIN_WIDTH   1600
#define WIN_HEIGHT  900
#define filename    "sprites/Unyu.png"
#define FONT_FILEPATH   "fonts/dm-serif-text-latin-400-normal.ttf"
#define FONT_SIZE   32

int main(){
    sf::RenderWindow w(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "Unyu");

    sf::Texture t;

    if(!t.loadFromFile(filename))
        return EXIT_FAILURE;

    sf::Sprite s;
    s.setTexture(t);
    s.setOrigin(t.getSize().x/2.f, t.getSize().y);
    s.setPosition(sf::Vector2f(WIN_WIDTH/2.f, WIN_HEIGHT));


    sf::Font font;
    if(!font.loadFromFile(FONT_FILEPATH)){
        std::cout<<"Font file not found."<<std::endl;
        return EXIT_FAILURE;
    }

    TextBox tb = TextBox(
        sf::Vector2f(WIN_WIDTH/2.f, WIN_HEIGHT/8.f), 
        sf::Vector2f(WIN_WIDTH/2.f, WIN_HEIGHT-WIN_HEIGHT/16.f), 
        "Unyu",
        font,
        FONT_SIZE
    );

    std::string txt = "";
    for(int i = 0; i < 120; ++i)
        txt+=('E'+i%3);
    tb.setString(txt);
    tb.scrollDown();
    tb.scrollUp();

    
    while(w.isOpen()){
        sf::Event event;
        while(w.pollEvent(event)){
            if(event.type == sf::Event::Closed)
                w.close();
            }
        
        w.clear();
        // t.update(w);
        w.draw(s);
        tb.draw(w);
        w.display();
    }
    
    return EXIT_SUCCESS;
}


