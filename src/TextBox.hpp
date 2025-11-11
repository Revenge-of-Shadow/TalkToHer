#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <SFML/Graphics.hpp>

class TextBox{
    std::string line;    //  I HAVE TO MAKE SURE IT HAS NO LINE BREAKS.
    std::string shownString;
    short offset;
    int fontSize;

protected:
    void setText();

public:
    sf::RectangleShape rect;
    sf::Font font;
    sf::Text text;

    TextBox();
    TextBox(sf::Vector2f size, sf::Vector2f pos, std::string str, sf::Font& font, int fontSize);
    //  Uncrolled? I suppose it can take in the whole text and scroll it later.
    //  Line length depends on font size and rectangle width.
    void setString(std::string text);
    std::string getString();

    int getWidthInChars();
    int getHeightInChars();

    bool scrollUp();
    bool scrollDown();

    

    void draw(sf::RenderTarget& target);
    
};
#endif
