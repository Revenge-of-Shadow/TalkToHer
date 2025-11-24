#ifndef DRAWABLEOBJECT_H
#define DRAWABLEOBJECT_H
#include "libs.hpp"
#include "defaults.h"


class DrawableObject: public sf::Drawable{

    //  DrawableObject sprite must have origin at the bottom center.
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
    DrawableObject(){};
    DrawableObject(std::string name)
        :name(name){
    };
    DrawableObject(std::string name, std::string sprite_name)
        :name(name){
        loadSprite(sprite_name);
    };

    void setName(std::string object_name){ name=object_name; };
    void setPosition(sf::Vector2f pos){
        position = pos;  
        sprite.setPosition(position);
    };
    bool loadSprite(std::string sprite_name){
        return loadSpriteFromPath(
                     sprite_folder_suffix + kPathSepartor 
                   + name + kPathSepartor 
                   + sprite_name);
    };

    std::string getName(){return name;};
    sf::Sprite getSprite(){return sprite;};
    sf::Vector2f getPosition()const{return position;};

    void move(sf::Vector2f mov){ position+=mov; sprite.setPosition(position); };
    void brightness(float val){};

    //  Could probably add a boolean for not drawing hidden, but I see no need
    //  for it.
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const{
        target.draw(sprite);
    };

    DrawableObject& operator=(const DrawableObject& other){
        if(this != &other){
            name = other.name;
            position = other.position;
            sprite = other.sprite;
        }
        return *this;
    }
    DrawableObject* operator*(){
        return this;
    }
    
};
#endif
