#include "../include/param_helper/params.hpp"

class RectangleParameters : public Parameters
{
public:
    explicit RectangleParameters(const json params) : Parameters(params),
        length(get_value_by_key<double>("length", 1.0)),
        width(get_value_by_key<double>("width", 1.0))
    {}

    explicit RectangleParameters(const double length_=1.0, const double width_=1.0) : RectangleParameters(
            json {
                    {"length", length_},
                    {"width", width_}
            }
            )
    {}

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

    // Use case

    RectangleParameters rp(2.0);
    rp.write_to_file("test", "rectangle_parameters", true);

    auto params2 = Parameters::create_by_file("test", "rectangle_parameters");
    RectangleParameters rp2(params2.get_json());

    RectangleParameters rp3(Parameters::read_parameter_file("test", "rectangle_parameters", true));

    Rectangle rectangle2(rp3);

    std::cout << "Rectangle area: " << rectangle2.get_area() << std::endl;

    return 0;
}