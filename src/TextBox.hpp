#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "libs.hpp"
#include "defaults.h"

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
    TextBox(sf::Vector2f size, sf::Vector2f pos, std::string str, std::string fontname, int fontSize);
    TextBox* operator*();

    bool loadFont(std::string fontname);

    bool scrollUp();
    bool scrollDown();

    void draw(sf::RenderTarget& target);


    void setString(std::string text);
    std::string getString();

    void setFillColor(sf::Color);
    sf::Color getFillColor();
    void setOutlineColor(sf::Color);
    sf::Color getOutlineColor();
    void setOutlineThickness(float);
    float getOutlineThickness();

    void setTextFillColor(sf::Color);
    sf::Color getTextFillColor();
    void setTextOutlineColor(sf::Color);
    sf::Color getTextOutlineColor();
    void setTextOutlineThickness(float);
    float getTextOutlineThickness();

    int getWidthInChars();
    int getHeightInChars();
};
#endif
