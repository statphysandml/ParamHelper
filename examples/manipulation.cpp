#include <param_helper/params.hpp>


using namespace param_helper::params;


// Execute with: g++ manipulation.cpp -I ../include/ ../build/src/libparamhelper.a -o manipulation


int main() {
    std::cout << "\n### Manipulating json objects ###\n" << std::endl;

    Parameters params = Parameters::create_by_params(json {{"a", 0}, {"vec", std::vector<double> {0.0, 1.0}}});
    // Get raw nohlmann json file
    json params_json = params.get_json();

    json additional_parameters = {{"e", 0.0}, {"f", std::complex<double> {1.0, 1.0}}};
    std::cout << "Additional parameters " << additional_parameters << std::endl;

    params_json = merge(params_json, additional_parameters);
    std::cout << "Params json after merging" << params_json << std::endl;

    params_json = subtract(params_json, additional_parameters);
    std::cout << "Params json after subtraction" << params_json << "\n" << std::endl;
}