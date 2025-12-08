#include "DrawableObject.hpp"
#include "Option.hpp"
#include "defaults.hpp"
#ifndef EXTRAS_HPP
#define EXTRAS_HPP

enum class Extra{Extras, Sprites, Sprite};
class Extras{
    DrawableObject obj;
public:
    Shortlist<Option> options;
    int index;
    std::string path;
    Extra state;

    Extras();
    void list();
    void prev();
    void next();
    void open();
    void back();

    void wrap();
    DrawableObject getObj();
};

#endif
