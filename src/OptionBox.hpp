#ifndef OPTIONBOX_H
#define OPTIONBOX_H

#include "TextBox.hpp"
class OptionBox : public TextBox{
public:
    OptionBox();
    OptionBox(sf::Vector2f size, sf::Vector2f pos, std::string str, std::string fontname, int fontSize);
    void toggleChosen();
};

#endif

