#include "defaults.h"

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
