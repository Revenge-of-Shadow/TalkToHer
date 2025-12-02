#ifndef DRAWABLEOBJECT_H
#define DRAWABLEOBJECT_H
#include "defaults.hpp"


class DrawableObject: public sf::Drawable{
    //  DrawableObject sprite must have origin at the bottom center.
    std::string name;
    sf::Vector2f position;
    sf::Sprite sprite;    
    sf::Texture texture;
    bool loadSpriteFromPath(std::string filepath){
        if(!texture.loadFromFile(filepath)) return false;
        sprite.setTexture(texture);
        sprite.setOrigin(
            sf::Vector2f(texture.getSize().x/2.f, texture.getSize().y));

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
                     sprite_foldername + kPathSepartor 
                   + forceSeparator(name) + kPathSepartor 
                   + forceSeparator(sprite_name));
    };

    std::string getName(){return name;};
    sf::Sprite getSprite(){return sprite;};
    sf::Vector2f getPosition()const{return position;};

    void move(sf::Vector2f mov){ position+=mov; sprite.setPosition(position); };
    void brightness(float val){};

    //  Could probably add a boolean for not drawing hidden, but I see no need
    //  for it.
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const{
        sf::Vector2f sprPos = position/100.f;
        sprPos.x *=
            static_cast<float>(target.getSize().x);
        sprPos.y *=
            static_cast<float>(target.getSize().y);
        sf::Sprite temp = sprite;
        temp.setPosition(actualVector(
            sf::Vector2f(target.getSize()), sprPos));
        float scale = std::min(
            static_cast<float>(target.getSize().x)/presetsize.x,
            static_cast<float>(target.getSize().y)/presetsize.y);
        temp.setScale(scale, scale);
        target.draw(temp);
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
