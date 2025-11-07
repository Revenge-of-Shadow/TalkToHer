#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Sprite.hpp>

#define WIN_WIDTH   1600
#define WIN_HEIGHT  900
#define filename    "/home/electroheart/Pictures/Krita/Finished/Unyu.png"

int main(){
    sf::RenderWindow w(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "Unyu");

    sf::Texture t;

    if(!t.loadFromFile(filename))
        return EXIT_FAILURE;

    sf::Sprite s;
    s.setTexture(t);
    
    
    while(w.isOpen()){
        sf::Event event;
        while(w.pollEvent(event)){
            if(event.type == sf::Event::Closed)
                w.close();
            }
        
        w.clear();
        // t.update(w);
        w.draw(s);
        w.display();
    }
    
    return EXIT_SUCCESS;
}


