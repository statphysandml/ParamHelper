#include "../include/param_helper/params.hpp"

namespace param_helper {
    namespace params {

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


        bool construct_parameter_path(json& j, const std::string& parameter, std::vector<std::string>& parameter_path)
        {
            if(j.is_structured())
            {
                /* std::cout << "Dumping " << j.dump() << std::endl;
                for(auto &elem: parameter_path)
                    std::cout << elem << " ";
                std::cout << std::endl; */

                if(j.find(parameter) != j.end()) {
                    parameter_path.insert(parameter_path.begin(), parameter);
                    return true;
                }
                else
                    for(auto it = j.begin(); it != j.end(); ++it)
                    {
                        bool actual_parameter_path = construct_parameter_path(*it, parameter, parameter_path);
                        if(actual_parameter_path)
                        {
                            parameter_path.insert(parameter_path.begin(), it.key());
                            return true;
                        }
                    }
                return false; // nothing found
            }
            else
                return false;
        }
    }
}

std::ostream& operator<< (std::ostream& os, const param_helper::params::Parameters& params) {
    os << std::setw(4) << params.get_json().dump() << '\n';
    return os;
}
