#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "libs.hpp"

class TextBox{
    std::string line;
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
    void setString(std::string text);
    std::string getString();

    int getWidthInChars();
    int getHeightInChars();

    bool scrollUp();
    bool scrollDown();

    void draw(sf::RenderTarget& target);
};
#endif
