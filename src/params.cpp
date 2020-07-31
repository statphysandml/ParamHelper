#include "../include/param_helper/params.hpp"
#include <iostream>


bool fexists(const std::string& filename) {
    std::ifstream ifile(filename.c_str());
    return (bool)ifile;
}

json merge(const json &a, const json &b)
{
    json result = a.flatten();
    json tmp = b.flatten();

    for (json::iterator it = tmp.begin(); it != tmp.end(); ++it)
    {
        result[it.key()] = it.value();
    }

    return result.unflatten();
}

json subtract(const json &a, const json &b)  // (a- b)
{
    json result = a.flatten();
    json tmp = b.flatten();

    for (json::iterator it = tmp.begin(); it != tmp.end(); ++it)
    {
        auto it_find = result.find(it.key());
        if(it_find != result.end())
            result.erase(it.key());
    }

    return result.unflatten();
}

std::ostream& operator<< (std::ostream& os, const Parameters& params) {
    os << std::setw(4) << params.get_json().dump() << '\n';
    return os;
}