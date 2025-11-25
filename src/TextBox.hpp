#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "libs.hpp"
#include "defaults.h"

class TextBox: public sf::Drawable{
    std::string line;
    std::string shownString;
    short offset;
    int fontSize;

protected:
    void setText();

public:
    sf::RectangleShape rect;
    std::string fontName;
    sf::Font font;
    sf::Text text;
    bool centeredText;

    TextBox();
    TextBox(sf::Vector2f size, sf::Vector2f pos, std::string str, std::string fontName, int fontSize, bool centeredText = false);
    TextBox* operator*();

    bool loadFont(std::string fontname);

    bool scrollUp();
    bool scrollDown();

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const{
        target.draw(rect);
        target.draw(text);
    }
    TextBox& operator=(const TextBox& other);


    void setString(std::string text);
    std::string getString() const;
    void centerText();

    void setFillColor(sf::Color);
    sf::Color getFillColor() const;
    void setOutlineColor(sf::Color);
    sf::Color getOutlineColor() const;
    void setOutlineThickness(float);
    float getOutlineThickness() const;

    void setTextFillColor(sf::Color);
    sf::Color getTextFillColor() const;
    void setTextOutlineColor(sf::Color);
    sf::Color getTextOutlineColor() const;
    void setTextOutlineThickness(float);
    float getTextOutlineThickness() const;

    int getWidthInChars() const;
    int getHeightInChars() const;
};
#endif
