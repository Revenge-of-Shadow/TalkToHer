#ifndef CHARACTER_H
#define CHARACTER_H
#include "libs.hpp"
#include "defaults.h"


class Character: public sf::Drawable{

    //  Character sprite must have origin at the bottom center.
    std::string name;
    sf::Vector2f position;
    sf::Sprite sprite;    
    sf::Texture texture;
    bool loadSpriteFromPath(std::string filepath){
        if(!texture.loadFromFile(filepath)) return false;
        sprite.setTexture(texture);
        sprite.setOrigin(texture.getSize().x/2.f, texture.getSize().y);

        return true;
    };
public:
    Character(){};
    Character(std::string name)
        :name(name){
    };
    Character(std::string name, std::string sprite_name)
        :name(name){
        loadSprite(sprite_name);
    };

    void setName(std::string name){ name=name; };
    void setPosition(sf::Vector2f pos){ position = pos; sprite.setPosition(pos);};
    bool loadSprite(std::string sprite_name){
        return loadSpriteFromPath(
                     sprite_folder_suffix + kPathSepartor 
                   + name + kPathSepartor 
                   + sprite_name);
    };

    std::string getName(){return name;};
    sf::Sprite getSprite(){return sprite;};
    sf::Vector2f getPosition(){return position;};

    void move(sf::Vector2f mov){ position+=mov; };
    void brightness(float val){};

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const{
        target.draw(sprite);
    };

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
