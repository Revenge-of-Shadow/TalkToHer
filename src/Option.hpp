
#ifndef OPTION_H
#define OPTION_H
#include "libs.hpp"

class Option{
public:
    std::string text;
    std::string val;

    Option(){}
    Option(std::string val, std::string text):val(val), text(text){}
};
#endif
