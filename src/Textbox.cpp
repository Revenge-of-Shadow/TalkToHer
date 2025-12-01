#include "Textbox.hpp"

Textbox::Textbox():offset(0), line(""), isChosen(false){}
Textbox::Textbox(sf::Vector2f size, sf::Vector2f pos, std::string str, std::string fontName, int fontSize, bool centeredText){
    this->line = str;
    this->offset = 0;
    this->fontName = fontName;
    this->fontSize = fontSize;
    font.loadFromFile(font_folder_suffix + kPathSepartor + fontName);
    double outline_thickness = pos.x/pos.y;
    size = sf::Vector2f(size.x - outline_thickness*2, size.y - outline_thickness*2);
    this->centeredText = centeredText;
    isChosen = false;

    rect = sf::RectangleShape(size);
    rect.setOrigin(size.x/2, size.y/2);
    rect.setPosition(pos);
    setFillColor(sf::Color(0,0,0, 0x7F));
    setOutlineColor(sf::Color(0xFF, 0xFF, 0xFF, 0x7F));
    setOutlineThickness(outline_thickness);
    
    text = sf::Text("", font, fontSize);
    setTextFillColor(sf::Color::White);
    setString(line);

}

Textbox* Textbox::operator*(){ return this; }

bool Textbox::scrollUp(){
    if(offset > 0){
        --offset;
        setText();
        return true;
    }
    return false;
}
bool Textbox::scrollDown(){
    if(shownString.find('\n') != std::string::npos){
        ++offset;
        setText();
        return true;
    }
    return false;
}

void Textbox::choose(){
    if(isChosen) return;
    rect.setSize(sf::Vector2f(rect.getSize().x*2.f, rect.getSize().y));
    isChosen = true;
}
void Textbox::unchoose(){
    if(!isChosen) return;
    rect.setSize(sf::Vector2f(rect.getSize().x/2.f, rect.getSize().y));
    isChosen = false;
}

Textbox& Textbox::operator=(const Textbox& other){
    if(this != &other){
        line = other.line;
        offset = other.offset;
        fontName = other.fontName;
        fontSize = other.fontSize;
        font.loadFromFile(font_folder_suffix + kPathSepartor + fontName);
        centeredText = other.centeredText;

        rect = other.rect;
        setFillColor(other.getFillColor());
        setOutlineColor(other.getOutlineColor());
        setOutlineThickness(other.getOutlineThickness());

        text = sf::Text("", font, fontSize);
        setTextFillColor(other.getTextFillColor());
        setTextOutlineColor(other.getOutlineColor());
        setOutlineThickness(other.getOutlineThickness());
        setString(line);
    }
    return *this;
}

void Textbox::setText(){//  Does not change line.
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
void Textbox::setString(std::string str){
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

    if(centeredText)
        centerText();
}
std::string Textbox::getString() const{   //  Give what is stored, not what is shown.
    return line;
}
void Textbox::centerText(){
    text.setPosition(sf::Vector2f(
        rect.getPosition().x - text.getLocalBounds().width/2,
        rect.getPosition().y - text.getLocalBounds().height/2
    ));
}

void Textbox::setFillColor(sf::Color color){rect.setFillColor(color);}
sf::Color Textbox::getFillColor() const{ return rect.getFillColor(); }
void Textbox::setOutlineColor(sf::Color color){rect.setOutlineColor(color);}
sf::Color Textbox::getOutlineColor() const{ return rect.getOutlineColor(); }
void Textbox::setOutlineThickness(float thickness){ rect.setOutlineThickness(thickness);}
float Textbox::getOutlineThickness() const{ return rect.getOutlineThickness(); }

void Textbox::setTextFillColor(sf::Color color){text.setFillColor(color);}
sf::Color Textbox::getTextFillColor() const{ return text.getFillColor(); }
void Textbox::setTextOutlineColor(sf::Color color){text.setOutlineColor(color);}
sf::Color Textbox::getTextOutlineColor() const{ return text.getOutlineColor(); }
void Textbox::setTextOutlineThickness(float thickness){ text.setOutlineThickness(thickness);}
float Textbox::getTextOutlineThickness() const{ return text.getOutlineThickness(); }

int Textbox::getWidthInChars() const{
    return (int)rect.getSize().x/text.getCharacterSize();
}
int Textbox::getHeightInChars() const{
    return (int)rect.getSize().y/text.getCharacterSize();
}
