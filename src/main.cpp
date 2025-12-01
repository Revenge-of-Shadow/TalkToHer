#include "defaults.h"
#include "Controller.hpp"

int main(){
    sf::RenderWindow window = sf::RenderWindow(
        sf::VideoMode(windowSize.x, windowSize.y), "");


    Controller controller(window);

    while(window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed)
                window.close();
            if(event.type == sf::Event::KeyPressed){
                controller.processKey(event);
            }
        }
        
        controller.mainloop();
    }

    return EXIT_SUCCESS;
}
