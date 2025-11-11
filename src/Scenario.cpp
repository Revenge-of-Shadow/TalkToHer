#include "Scenario.hpp"

bool Scenario::isCurrLineCommand(){
    return line.length() > 2
        && line.at(0) == '/' && line.at(1) != '/';
}
bool Scenario::isCurrLineComment(){
    return line.length() > 2
        && line.at(0) == '/' && line.at(1) == '/';
}
