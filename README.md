ParamHelper: Track your simulation
==================================

<!--- [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![GitHub Workflow Status](https://img.shields.io/github/workflow/status/statphysandml/ParamHelper/CI)](https://github.com/statphysandml/ParamHelper/actions?query=workflow%3ACI))
([![Documentation Status](https://readthedocs.org/projects/ParamHelper/badge/)](https://ParamHelper.readthedocs.io/)
[![codecov](https://codecov.io/gh/statphysandml/ParamHelper/branch/main/graph/badge.svg)](https://codecov.io/gh/statphysandml/ParamHelper)
[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=statphysandml_ParamHelper&metric=alert_status)](https://sonarcloud.io/dashboard?id=statphysandml_ParamHelper) -->

ParamHelper is a C++ library that allows easy tracking of all parameters of a simulation. The parameters can be written to file and loaded for a potential rerun of the simulation. The core of the library uses [JSON for Modern C++](https://github.com/nlohmann/json).

Prerequisites
------------

Building ParamHelper requires:

* A C++17-compliant compiler (e.g., g++ >= 9)
* CMake `>= 3.18`
* Doxygen (optional, for documentation)
* Python with Sphinx and Breathe (optional, for advanced documentation)

Building ParamHelper
--------------------

The recommended way to build ParamHelper is with an out-of-source build:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

You can customize the build with these CMake variables (add as `-D<var>=ON|OFF`):

* `BUILD_TESTING`: Enable building of the test suite (default: `ON`)
* `BUILD_DOCS`: Enable building the documentation (default: `ON`)

> **Note:** All dependencies (including Catch2 for testing and nlohmann_json for JSON support) are automatically downloaded using CMake's FetchContent.  
> You do **not** need to initialize or update any git submodules.

Installation
------------

To install the library locally:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=~/ParamHelper/install
cmake --build build --target install
```

If you install to a custom location, set `CMAKE_PREFIX_PATH` when using ParamHelper in other projects:

```sh
cmake -S . -B build -DCMAKE_PREFIX_PATH=~/ParamHelper/install
```

Testing
-------

To build and run the test suite:

```sh
cmake -S . -B build -DBUILD_TESTING=ON
cmake --build build
cd build
ctest
```

Documentation
-------------

To build the documentation (requires Doxygen and Sphinx):

```sh
cmake -S . -B build -DBUILD_DOCS=ON
cmake --build build --target paramhelper_doxygen
cmake --build build --target paramhelper-sphinx-doc
```

Examples
--------

Example use cases can be found in the `examples/` directory. To build the examples:

```sh
cmake -S examples -B examples/build -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=~/ParamHelper/install
cmake --build examples/build
```

Run the example with:

```sh
./ParamHelperExamples
```

The example is explained in the following in more detail.

Use case: We want to manage the parameters of a class Rectangle that can be used to compute the area of a rectangle.


The class RectangleParameter contains all parameters that are supossed to be tracked:
```c++
#include "param_helper/params.hpp"
using namespace param_helper::params;

class RectangleParameters : public Parameters
{
public:
    explicit RectangleParameters(const json params) : Parameters(params),
        length(get_entry<double>("length", 1.0)),
        width(get_entry<double>("width", 1.0))
    {}
    
    explicit RectangleParameters(const double length_=1.0, const double width_=1.0) :
        RectangleParameters(json {{"length", length_}, {"width", width_}})
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
```

The actual class Rectangle has been defined as a friend to the class RectangleParameter. The class implements all functionalities:
```c++
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
```
Access on the parameters is provided by the member variable rp. The class Reactangle can be used as follows:
```c++
// The project root can be adapted by params_helper::proj::set_relative_path_to_project_root_dir
std::cout << "Current directory: " << param_helper::proj::project_root() << std::endl;

// Generate parameters and write them to file
RectangleParameters rp(2.0, 3.0);
// Generates the directory parameters/ relative to the project_root
param_helper::fs::generate_directory_if_not_present("parameters", true);
rp.write_to_file("parameters", "rectangle_parameters", true);

// Optional
param_helper::fs::check_if_parameter_file_exists("parameters", "rectangular_parameters");

// Reload the parameters
auto params2 = Parameters::create_by_file("parameters", "rectangle_parameters");
RectangleParameters rp2(params2.get_json());

// Alternative way
RectangleParameters rp3(
    param_helper::fs::read_parameter_file("parameters", "rectangle_parameters", true)
);

// Generate object with respective parameters
Rectangle rectangle2(rp3);

std::cout << "\nCompute rectangle area: " << rectangle2.get_area() << "\n" << std::endl;
```

In the example, a json file "rectangle_parameters.json" has been stored in the directory "parameters":
```json
{
  "length": 2.0,
  "width": 3.0
}

```

Usage
-----

**With g++:**
```sh
g++ param_helper_examples.cpp -I ~/ParamHelper/install/include/ ~/ParamHelper/install/lib/libparamhelper.a -o main
```

**With CMake (CMakeLists.txt):**
```cmake
find_package(ParamHelper CONFIG REQUIRED)
target_link_libraries(your_target PRIVATE paramhelper::paramhelper)
```

If installed locally, set `CMAKE_PREFIX_PATH` when calling CMake:

```sh
cmake -S . -B build -DCMAKE_PREFIX_PATH=~/ParamHelper/install
```

Further Examples
----------------

### Use case with multiple parameters

The library also provides an easy way to combine several parameter files
```c++
Parameters project_params = Parameters::create_by_params(
    json {{"name", "project_a"}, {"details", "rectangle_analysis"}}
);

RectangleParameters rp_analysis(2.0, 3.0);
project_params.append_parameters(rp_analysis);

param_helper::fs::generate_directory_if_not_present("project", true);
project_params.write_to_file("project", "rectangle_analysis", true);

// Add additional_project parameters to the already existing project file based on the given path
Parameters additional_project_params = Parameters::create_by_params(
    json {{"type", "type_b"}, {"n", "100"}}
);
additional_project_params.merge_to_file("project", "rectangle_analysis", true);

// Reload updated parameters
auto updated_project_params = Parameters::create_by_file("project", "rectangle_analysis");
std::cout << "Updated project params " << updated_project_params << std::endl;
```
A folder "project" has been created that contains a "rectangle_analysis.json" file, that contains also the rectangle parameters:
```json
{
  "Rectangle": {
    "length": 2.0,
    "width": 3.0
  },
  "details": "rectangle_analysis",
  "n": "100",
  "name": "project_a",
  "type": "type_b"
}
```

### The Parameter class

Examples for using the base class Parameter. The class Parameter is a wrapper to manage access and further functions on a nohlmann::json object.


```c++
#include "param_helper/params.hpp"
using namespace param_helper::params;

Parameters params = Parameters::create_by_params(
    json {{"a", 0}, {"vec", std::vector<double> {0.0, 1.0}}}
);
params.add_entry("c", "c");

std::cout << "Parameters: " << params << std::endl;

auto a = params.get_entry<double>("a");
auto b = params.get_entry<double>("b", 1.0, true);
std::cout << "Extracted parameters: a " << a << " " << "b " << b << "\n" << std::endl;

std::cout << "Looking for parameter b: " << params.haskey("b") << std::endl;

// Delete entry with "b"
params.delete_entry("b");

// Get raw nohlmann json file
json params_json = params.get_json();
std::cout << "\nParams json object: " << params_json << "\n" << std::endl;
```

### Manipulating json objects

Manipulation of json objects (merge and subtract)

```c++
json additional_parameters = {{"e", 0.0}, {"f", std::complex<double> {1.0, 1.0}}};
params_json = merge(params_json, additional_parameters);
params_json = subtract(params_json, additional_parameters);
```

Support and Development
----------------------

The project was generated with the help of the [cookiecutter-cpp-project](https://github.com/ssciwr/cookiecutter-cpp-project) of the [Scientific Software Center, IWR, Heidelberg University](https://ssc.iwr.uni-heidelberg.de/).

For bug reports/suggestions/complaints please file an issue on GitHub or start a discussion on our mailing list: statphysandml@thphys.uni-heidelberg.de.