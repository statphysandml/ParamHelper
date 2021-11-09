//
// Created by lukas on 07.12.20.
//

#include "ParamHelper/params.hpp"


using namespace param_helper::params;


// Execute with: g++ parameters.cpp -I ../include/ ../lib/libparamhelper.a -o parameters


int main() {
    std::cout << "\n### The Paramter class ###\n" << std::endl;

    Parameters params = Parameters::create_by_params(json {{"a", 0}, {"vec", std::vector<double> {0.0, 1.0}}});
    params.add_entry("c", "c");

    std::cout << "Parameters: " << params << std::endl;

    auto a = params.get_entry<double>("a");
    auto b = params.get_entry<double>("b", 1.0, true);
    std::cout << "Extracted parameters: a " << a << " " << "b " << b << "\n" << std::endl;

    std::cout << "Parameters after querying for b: " << params << std::endl;

    std::cout << "Looking for parameter b: " << params.haskey("b") << std::endl;

    // Delete entry with "b"
    params.delete_entry("b");
    std::cout << "Looking for parameter b after delete: " << params.haskey("b") << std::endl;

    // Get raw nohlmann json file
    json params_json = params.get_json();
    std::cout << "\nParams json object: " << params_json << "\n" << std::endl;

    return 0;
}