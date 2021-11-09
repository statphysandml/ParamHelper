//
// Created by lukas on 07.12.20.
//

#include <iostream>
#include "ParamHelper/params.hpp"


using namespace param_helper::params;


class RectangleParameters : public Parameters
{
public:
    explicit RectangleParameters(const json params) : Parameters(params),
                                                      length(get_entry<double>("length", 1.0)),
                                                      width(get_entry<double>("width", 1.0))
    {}

    explicit RectangleParameters(const double length_=1.0, const double width_=1.0) : RectangleParameters(
            json {{"length", length_}, {"width", width_}})
    {}

    static std::string name()
    {
        return "Rectangle";
    }

private:
    friend class Rectangle;

    double length;
    double width;
};

class Rectangle
{
public:

    explicit Rectangle(const RectangleParameters rp_) : rp(rp_)
    {}

    double get_area() const
    {
        return rp.length * rp.width;
    }

private:
    const RectangleParameters rp;
};


// Execute with: g++ all_examples.cpp -I ../include/ ../lib/libparamhelper.a -o all_examples


int main() {
    //[Use case

    std::cout << "\n### Actual use case with file system exchange ###\n" << std::endl;

    std::cout << "Current directory: " << param_helper::fs::prfs::project_root() << std::endl;

    // Generate parameters and write them to file
    RectangleParameters rp(2.0, 3.0);
    param_helper::fs::generate_directory_if_not_present("parameters", true);
    rp.write_to_file("parameters", "rectangle_parameters", true);

    // Reload the parameters
    param_helper::fs::check_if_parameter_file_exists("parameters", "rectangular_parameters"); // optional

    auto params2 = Parameters::create_by_file("parameters", "rectangle_parameters");
    RectangleParameters rp2(params2.get_json());

    // Alternative way
    RectangleParameters rp3(param_helper::fs::read_parameter_file("parameters", "rectangle_parameters", true));

    // Generate object with respective parameters
    Rectangle rectangle2(rp3);

    std::cout << "\nCompute rectangle area: " << rectangle2.get_area() << "\n" << std::endl;

    //]

    // Use case with multiple parameter files

    std::cout << "\n### Combination of several parameter files ###\n" << std::endl;

    Parameters project_params = Parameters::create_by_params(json {{"name", "project_a"}, {"details", "rectangle_analysis"}});

    RectangleParameters rp_analysis(2.0, 3.0);
    project_params.append_parameters(rp_analysis);

    param_helper::fs::generate_directory_if_not_present("project", true);
    project_params.write_to_file("project", "rectangle_analysis", true);

    std::cout << "Project params: " << project_params << std::endl;

    // Add additional_project parameters to the already existing project file based on the given path
    Parameters additional_project_params = Parameters::create_by_params(json {{"type", "type_b"}, {"n", "100"}});
    additional_project_params.merge_to_file("project", "rectangle_analysis", true);

    // Reload updated parameters
    auto updated_project_params = Parameters::create_by_file("project", "rectangle_analysis");
    std::cout << "Updated project params " << updated_project_params << std::endl;

    //]

    //[ Parameter class

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

    //]

    //[ Manipulation

    std::cout << "\n### Manipulating json objects ###\n" << std::endl;

    json additional_parameters = {{"e", 0.0}, {"f", std::complex<double> {1.0, 1.0}}};
    std::cout << "Additional parameters " << additional_parameters << std::endl;

    params_json = merge(params_json, additional_parameters);
    std::cout << "Params json after merging" << params_json << std::endl;

    params_json = subtract(params_json, additional_parameters);
    std::cout << "Params json after subtraction" << params_json << "\n" << std::endl;

    //]
    return 0;
}