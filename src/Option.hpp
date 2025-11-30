
#ifndef OPTION_H
#define OPTION_H
#include "libs.hpp"

class Option{
    std::string text;
    std::string path;
public:
    Option(){}
    Option(std::string path, std::string text):text(text), path(path){}
    std::string getText()const{ return text; }
    std::string getPath()const{ return path; }
    Option& operator=(const Option& other){ 
        std::cout<<path<<" := "<<other.path<<std::endl;
        if(this != &other){
            path = other.path;
            text = other.text;
        }
        return *this;
    }
};
#endif
