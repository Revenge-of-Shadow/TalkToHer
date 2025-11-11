#ifndef CHARACTER_H
#define CHARACTER_H
#include "libs.hpp"

class Character:sf::Drawable{
    //  Character sprite must have origin at the bottom center.
    std::string name;
    sf::Vector2f position;
    sf::Sprite sprite;    
public:
    Character();
    Character(std::string name, sf::Vector2f pos, std::string sprite_filename);

    void setName(std::string name);
    bool loadSprite(const char* filepath);
    void setPosition(sf::Vector2f pos);

    std::string getName();
    sf::Sprite getSprite();
    sf::Vector2f getPosition();

    void move(sf::Vector2f mov);
    void brightness(float val);

    void draw(sf::RenderTarget& target);
    
};
#endif
