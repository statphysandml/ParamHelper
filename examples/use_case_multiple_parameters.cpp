#include "param_helper/params.hpp"


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


// Execute with: g++ use_case_multiple_parameters.cpp -I ../include/ ../build/src/libparamhelper.a -o use_case_multiple_parameters


int main() {
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

}