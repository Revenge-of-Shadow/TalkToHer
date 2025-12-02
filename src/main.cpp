#include "defaults.hpp"
#include "Controller.hpp"

int main(){
    sf::RenderWindow window = sf::RenderWindow(
        sf::VideoMode(presetsize.x, presetsize.y), "");


    Controller controller(window);

    while(window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed)
                window.close();
            if(event.type == sf::Event::KeyPressed){
                if(controller.processKey(event)) window.close();
            }
        }

        controller.mainloop();
    }

    return EXIT_SUCCESS;
}
