#include "Extras.hpp"
#include "DrawableObject.hpp"
#include "defaults.hpp"

Extras::Extras():index(0), path(""), state(Extra::Extras){
    obj.setPosition(sf::Vector2f(50.f, 100.f));
    list();
}
void Extras::list(){
    switch(state){
        case Extra::Extras:
            options.erase();
            options.add(Option("sprites"+kPathSepartor, "Sprites"));
        break;
        case Extra::Sprites:
            options.erase();
            for(const auto &entry: fsys::directory_iterator(path)){
                std::string entry_path = 
                    std::string(entry.path()).substr(path.length()+1);
                if(fsys::is_directory(entry.path()))
                   entry_path+=kPathSepartor;
                options.add(Option(entry.path(), entry_path));
            }
        break;
             
    }
    index = 0;
    
}
void Extras::prev(){index > 0? --index : index = options.getSize()-1;}
void Extras::next(){index < options.getSize()-1? ++index : index = 0;}
void Extras::open(){
    switch(state){
        case Extra::Extras:
            if(options[index].text == "Sprites"){
                path = sprite_foldername;
                state = Extra::Sprites;
                list();
            }
        break;
        case Extra::Sprites:
            if(fsys::is_directory(options[index].val)){
                path=options[index].val;
                list();
            }
            else if(
                options[index].val.substr(options[index].val.length()-4)
                    == ".png"){
                obj.loadSprite(options[index].val
                               .substr(sprite_foldername.length()+1));
                state = Extra::Sprite;
            }
        break;
    }
}
void Extras::back(){
    switch(state){
        case Extra::Sprites:
            if(path == sprite_foldername)
                state = Extra::Extras;
            else{
                path = path.substr(0, path.find_last_of(kPathSepartor));
            }
            list();
        break;
        case Extra::Sprite:
            state = Extra::Sprites;
            list();
        break;
    }
}
void Extras::wrap(){}

DrawableObject Extras::getObj(){return obj;}
