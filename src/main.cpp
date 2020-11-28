#include "../include/param_helper/params.hpp"

int main() {
    auto params = Parameters::create_by_params(json {{"a", 0}, {"vec", std::vector<double> {0.0, 1.0}}});

    // Manipulation

    std::cout << "Parameters: " << params << std::endl;

    auto a = params.get_value_by_key<double>("a");
    auto b = params.get_value_by_key<double>("b", 1.0, true);
    std::cout << "a " << a << " " << "b " << b << std::endl;

    std::cout << "Parameters after querying for b: " << params << std::endl;

    std::cout << "Looking for parameter b: " << params.haskey("b") << std::endl;
    params.delete_entry("b");
    std::cout << "Looking for parameter b after delete: " << params.haskey("b") << std::endl;

    // For usage of params as nohlmann json file
    json params_json = params.get_json();

    // File access




    // Operations on parameter multiple parameter objects




    return 0;
}