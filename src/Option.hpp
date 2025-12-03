
#ifndef OPTION_H
#define OPTION_H
#include "libs.hpp"

class Option{
public:
    std::string text;
    std::string val;

    Option(){}
    Option(std::string val, std::string text):text(text), val(val){}
    // Option& operator=(const Option& other){ 
    //     if(this != &other){
    //         val = other.val;
    //         text = other.text;
    //     }
    //     return *this;
    // }
};
#endif
