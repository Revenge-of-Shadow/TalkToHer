#ifndef CHARACTER_H
#define CHARACTER_H
#include "libs.hpp"


class Character: public sf::Drawable{
    const std::string sprite_folder_suffix = "sprites";

    //  Character sprite must have origin at the bottom center.
    std::string name;
    sf::Vector2f position;
    sf::Sprite sprite;    
public:
    Character(){};
    Character(std::string name, std::string sprite_filename)
        :name(name){
        loadSprite(sprite_folder_suffix + kPathSepartor 
                   + name + kPathSepartor 
                   + sprite_filename);
                  };

    void setName(std::string name){ name=name; };
    bool loadSprite(std::string filepath){return false;};
    void setPosition(sf::Vector2f pos){};

    std::string getName(){return name;};
    sf::Sprite getSprite(){return sprite;};
    sf::Vector2f getPosition(){return position;};

    void move(sf::Vector2f mov){ position+=mov; };
    void brightness(float val){};

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const{};

    Character& operator=(const Character& other){
        if(this != &other){
            name = other.name;
            position = other.position;
            sprite = other.sprite;
        }
        return *this;
    }
    
};
#endif
