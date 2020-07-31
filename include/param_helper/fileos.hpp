//
// Created by lukas on 01.04.20.
//

#ifndef PROGRAM_FILEOS_HPP
#define PROGRAM_FILEOS_HPP

#include <fstream>

class Fileos {
public:
    explicit Fileos(std::string fname) : os{} {
        os.open(fname);
    }
    ~Fileos() {os.close();}

    std::ofstream *operator->() {return &os;}

    std::ofstream& get() {return os;}
private:
    std::ofstream os;
};

#endif //PROGRAM_FILEOS_HPP
