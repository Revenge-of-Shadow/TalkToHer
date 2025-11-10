#include "TextBox.hpp"


TextBox::TextBox():offset(0), str(""){}
TextBox::TextBox(sf::Vector2f size, sf::Vector2f pos, std::string str, sf::Font& font, int fontSize){
    str = str;
    offset = 0;
    fontSize = fontSize;
    font = font;
    double outline_thickness = pos.x/pos.y;
    size = sf::Vector2f(size.x - outline_thickness*2, size.y - outline_thickness*2);

    rect = sf::RectangleShape(size);
    rect.setOrigin(size.x/2, size.y/2);
    rect.setPosition(pos);
    rect.setFillColor(sf::Color::Black);
    rect.setOutlineColor(sf::Color::White);
    rect.setOutlineThickness(outline_thickness);
    
    text = sf::Text("", font, fontSize);
    setString(str);
}

void TextBox::setString(std::string str){
    str = str;
    text.setString(str);

    //  Top left corner.
    text.setOrigin(0,0);
    //  Position it at the top left of the rectangle.
    text.setPosition(sf::Vector2f(
        rect.getPosition().x-rect.getSize().x/2,
        rect.getPosition().y-rect.getSize().y/2
    ));
    text.setFillColor(sf::Color::White);
}
std::string TextBox::getString(){
    return str;
}

int TextBox::getWidthInChars(){
    return (int)rect.getSize().x/text.getCharacterSize();
}
int TextBox::getHeightInChars(){
    return (int)rect.getSize().y/text.getCharacterSize();
}

void TextBox::scrollUp(short lines){
    offset = offset-lines < 0? 0 : offset-lines;
}
void TextBox::scrollDown(short lines){
    //  Out of bounds check needed.
    offset+=lines;
}

void TextBox::draw(sf::RenderTarget &target){
    target.draw(rect);
    target.draw(text);
}
