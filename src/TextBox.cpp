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

    text = sf::Text(str, font, fontSize);
    text.setOrigin(text.getLocalBounds().getSize().x/2,
                   text.getLocalBounds().getSize().y/2);

    text.setPosition(pos);
    text.setFillColor(sf::Color::White);
}

void TextBox::setString(std::string str){
    str = str;
}
std::string TextBox::getString(){
    return str;
}

void TextBox::scrollUp(short lines){
    offset = offset-lines < 0? 0 : offset-lines;
}
void TextBox::scrollDown(short lines){
    //  Out of bounds check needed.
    offset+=lines;
}

// void TextBox::setFont(sf::Font font){
//     font = font;
// }
//
void TextBox::draw(sf::RenderTarget &target){
    target.draw(rect);
    target.draw(text);
}
