#ifndef LIBS_H
#define LIBS_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "SimpleList.hpp"

#ifdef _WIN32

const std::string kPathSepartor = "\\";

#else

const std::string kPathSepartor = "/";

#endif

#endif
