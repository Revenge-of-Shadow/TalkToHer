#include "defaults.hpp"


std::string forceSeparator(std::string str){
    Shortlist<int> positions;
    int pos;
    std::string temp_str = str;
    while((pos = temp_str.find("\\")) != str.npos){
        positions.add(str.length()-temp_str.length()+pos);
        temp_str = temp_str.substr(pos+1);
    }
    temp_str = str;
    while((pos = temp_str.find("/")) != str.npos){
        positions.add(str.length()-temp_str.length()+pos);
        temp_str = temp_str.substr(pos+1);
    }
    while(positions.getSize())
        str.replace(positions.pop(0), 1, kPathSepartor);
    return str;
}

//  I have learned to hate one tiny bug of SFML deeply.
sf::Vector2f actualVector(sf::Vector2f windowsize, sf::Vector2f v){
    sf::Vector2f offset = 
        sf::Vector2f(presetsize)-sf::Vector2f(windowsize);
    return v+offset/2.f;
}


