#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#ifndef std
#include <stdlib.h>
#endif

class TextBox:sf::RectangleShape{
    std::string text;

protected:
    void update();
public:
    TextBox();
    TextBox(sf::RectangleShape rect, std::string text):
       text(text){
    }
    //  Uncrolled? I suppose it can take in the whole text and scroll it later.
    void setText(std::string text);
    std::string getText();

    void scrollUp(int lines);
    void scrollDown(int lines);
    
};
