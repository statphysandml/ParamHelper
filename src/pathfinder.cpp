#include "../include/param_helper/pathfinder.hpp"

// ToDo: Generalize

std::string gcp() {
    char cCurrentPath[FILENAME_MAX];

    if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
        return std::to_string(errno);

    cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; // not really required

    //printf ("The current working directory is %s", cCurrentPath);
    std::string cp = std::string(cCurrentPath);

    // std::cout << cp << std::endl;
    return cp + "/..";

    /* std::size_t found = cp.find("program"); // Findet den folder "program" und gibt den entsprechenden path zurück
    if (found != std::string::npos)
        return cp.erase(found + 7);
    else {
        std::cout << "Error in pathfinder" << std::endl;
        std::exit(EXIT_FAILURE);
    } */
}