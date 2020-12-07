ParamHelper: Track your simulation
==================================

ParamHelper is a C++ library that allows an easy tracking of all parameters of a simulation. The parameters can easily be written to file and be loaded for a potential rerun of the simulation. An example provides a possible usage of the library. The core of the library makes use of JSON for Modern C++ from https://github.com/nlohmann/json. A management of the parameters takes place based on json files.

Examples
--------

Possible usages of the library can be best understood through examples. All examples can also be found in the examples/ directory.

Simple example for using the Parameter class. The Parameter class is a wrapper to manage access and further functions on a nohlmann::json object.

```c++
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
```

Manipulation of the json object