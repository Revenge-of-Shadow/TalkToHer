
#ifndef OPTION_H
#define OPTION_H
#include "libs.hpp"

class Option{
    std::string text;
    std::string path;
public:
    Option(){}
    Option(std::string path, std::string text):text(text), path(path){}
    std::string getText(){ return text; }
    std::string getPath(){ return path; }
    // Option& operator=(Option& opt){ 
    //     if(this != &opt){
    //         path = opt.path;
    //         text = opt.text;
    //     }
    //     return *this;
    // }
};
#endif
