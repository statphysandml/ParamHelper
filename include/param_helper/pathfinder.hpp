#ifndef PATHFINDER_HPP
#define PATHFINDER_HPP

#ifdef WINDOWS
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
#endif

#include <string>
#include <iostream>

// file head small

std::string gcp();

#endif