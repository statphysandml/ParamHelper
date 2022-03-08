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


// Execute with: g++ use_case.cpp -I ../include/ ../lib/libparamhelper.a -o use_case


int main() {
    std::cout << "\n### Actual use case with file system exchange ###\n" << std::endl;

    std::cout << "Current directory: " << param_helper::proj::project_root() << std::endl;

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
}