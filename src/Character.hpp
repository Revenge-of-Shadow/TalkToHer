#ifndef CHARACTER_H
#define CHARACTER_H
#include "libs.hpp"

class Character: public sf::Drawable{
    //  Character sprite must have origin at the bottom center.
    std::string name;
    sf::Vector2f position;
    sf::Sprite sprite;    
public:
    Character(){};
    Character(std::string name, sf::Vector2f pos, std::string sprite_filename)
        :name(name), position(pos){};

    void setName(std::string name){ name=name; };
    bool loadSprite(std::string filepath){return false;};
    void setPosition(sf::Vector2f pos){};

    std::string getName(){return name;};
    sf::Sprite getSprite(){return sprite;};
    sf::Vector2f getPosition(){return position;};

    void move(sf::Vector2f mov){ position+=mov; };
    void brightness(float val){};

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const{};
    
};
#endif
