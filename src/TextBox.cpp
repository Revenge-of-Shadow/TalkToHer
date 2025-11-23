#include "TextBox.hpp"

TextBox::TextBox():offset(0), line(""){}
TextBox::TextBox(sf::Vector2f size, sf::Vector2f pos, std::string str, std::string fontname, int fontSize){
    line = str;
    offset = 0;
    fontSize = fontSize;
    font.loadFromFile(font_folder_suffix + kPathSepartor + fontname);
    double outline_thickness = pos.x/pos.y;
    size = sf::Vector2f(size.x - outline_thickness*2, size.y - outline_thickness*2);

    rect = sf::RectangleShape(size);
    rect.setOrigin(size.x/2, size.y/2);
    rect.setPosition(pos);
    rect.setFillColor(sf::Color::Black);
    rect.setOutlineColor(sf::Color::White);
    rect.setOutlineThickness(outline_thickness);
    
    text = sf::Text("", font, fontSize);
    setString(line);
}

void TextBox::setText(){//  Does not change line.
    shownString = line;
    text.setString(shownString);

    float strOffLimits = text.getLocalBounds().width/rect.getLocalBounds().width;

    if(strOffLimits <= 1.f) return; //  No checks needed.

    int lineLen = shownString.length()/strOffLimits; //  Line that would fit.
    //  An algorithm that fits the given string into the rectangle.
    //  First, offset. If needed.
    if(offset){
        //  Cut off the offset part.
        if(shownString.length() > offset*lineLen)
            shownString = shownString.substr(
                offset*lineLen, //  Start from SAFE offset.
                shownString.length() - offset*lineLen
            );
        else
            shownString = "";

        strOffLimits -= offset;
    }
    //  Then check again, seeing if offset did the job or not yet.
    if(strOffLimits > 1.f){
        //  Break string into lines.
        std::string leftoverStr = shownString;
        shownString = "";

        for(int i = 0; ; ++i){
            //  When height is out, it cuts off, not finishing the shown string.
            if((text.getCharacterSize()*(1+text.getLineSpacing()))*(i) 
                > rect.getLocalBounds().height)
                break;
            if(i >= strOffLimits-1.f){
                shownString += leftoverStr;
                break;
            }

            shownString += leftoverStr.substr(0, lineLen)+'\n';
            leftoverStr = leftoverStr.substr(
                lineLen, leftoverStr.length() - lineLen);
        }
        //  Now shownString is divided by line breaks.
    }
    text.setString(shownString);
}
void TextBox::setString(std::string str){
    line = str;
 
    setText();

    //  Top left corner.
    text.setOrigin(0,0);
    //  Position it at the top left of the rectangle.
    text.setPosition(sf::Vector2f(
        rect.getPosition().x-rect.getSize().x/2,
        rect.getPosition().y-rect.getSize().y/2
    ));
    text.setFillColor(sf::Color::White);
}
std::string TextBox::getString(){   //  Give what is stored, not what is shown.
    return line;
}

int TextBox::getWidthInChars(){
    return (int)rect.getSize().x/text.getCharacterSize();
}
int TextBox::getHeightInChars(){
    return (int)rect.getSize().y/text.getCharacterSize();
}

bool TextBox::scrollUp(){
    if(offset > 0){
        --offset;
        setText();
        return true;
    }
    return false;
}
bool TextBox::scrollDown(){
    if(shownString.find('\n') != std::string::npos){
        ++offset;
        setText();
        return true;
    }
    return false;
}

void TextBox::draw(sf::RenderTarget &target){
    target.draw(rect);
    target.draw(text);
}
