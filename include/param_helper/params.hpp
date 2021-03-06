#ifndef PARAMETERS_LIBRARY_HPP
#define PARAMETERS_LIBRARY_HPP

#include <iostream>
#include <fstream>
#include <complex>
#include <sys/file.h>
#include <sys/stat.h>

#include "json.hpp"
#include "filesystem.hpp"

using json = nlohmann::json;

namespace param_helper {
    namespace params {
        json merge(const json &a, const json &b);

        json subtract(const json &a, const json &b);  // (a- b)

        bool construct_parameter_path(json &j, const std::string &parameter, std::vector<std::string> &parameter_path);

        template<typename T>
        static T entry_by_key(const json &params_, const std::string key) {
            auto it_find = params_.find(key);
            if (it_find != params_.end())
                return it_find->get<T>();
            std::cout << "ERROR: Parameter '" << key
                      << "' not found. Check if parameter is set per default in corresponding class or set parameter manually."
                      << std::endl;
            std::exit(EXIT_FAILURE);
        }

        class Parameters {
        public:
            Parameters() = default;

            explicit Parameters(const json params_) : params(params_) {}

            static Parameters create_by_params(const json params_) {
                return Parameters(params_);
            }

            static Parameters create_by_file(const std::string &directory, const std::string &filename) {
                return Parameters(fs::read_parameter_file(directory, filename));
            }

            void write_to_file(const std::string directory, const std::string filename,
                               const bool relative_path = true) const {
                fs::write_parameter_file(params, directory, filename, relative_path);
            }

            void merge_to_file(const std::string &directory, const std::string &filename,
                               const bool relative_path = true) {
                std::cout << "Merging to file..." << std::endl;
                json k = fs::read_parameter_file(directory, filename, relative_path);
                params = merge(k, params);
                write_to_file(directory, filename, relative_path);
            }

            // Never used? -> at least not by LatticeModelImplementations
            template<typename params_T>
            void append_parameters(const params_T parameters_wrapper) {
                json parameters_with_identifiers;
                parameters_with_identifiers[parameters_wrapper.name()] = parameters_wrapper.get_json();
                params = merge(params, parameters_with_identifiers);
            }

            template<typename params_T>
            void append_parameters(const params_T parameters_wrapper, std::string global_identifier) {
                json parameters;
                parameters[global_identifier] = parameters_wrapper.get_json();
                parameters[global_identifier]["name"] = parameters_wrapper.name();
                params = merge(params, parameters);
            }

            template<typename T>
            T get_entry(const std::string key) const {
                return entry_by_key<T>(params, key);
            }

            template<typename T>
            T get_entry(const std::string key, const T default_val) const {
                auto it_find = params.find(key);
                if (it_find != params.end())
                    return it_find->get<T>();
                else
                    return default_val;
            }

            template<typename T>
            T get_entry(const std::string key, const T default_val, const bool set_default = true) {
                auto it_find = params.find(key);
                if (it_find != params.end())
                    return it_find->get<T>();
                else {
                    if (set_default) add_entry(key, default_val);
                    return default_val;
                }
            }

            bool haskey(const std::string key) {
                auto it_find = params.find(key);
                return it_find != params.end();
            }

            template<typename T>
            void add_entry(const std::string key, const T val) {
                params[key] = val;
            }

            void delete_entry(const std::string key) {
                params.erase(key);
            }

            const json get_json() const {
                return params;
            }

            json &get_json() {
                return params;
            }

        protected:
            json params;
        };
    }
}

namespace std {

    template< class T > void to_json(json &j, const std::complex< T > &p) {
        j = json {p.real(), p.imag()};
    }

    template< class T > void from_json(const json &j, std::complex< T > &p) {
        p.real(j.at(0));
        p.imag(j.at(1));
    }
}

std::ostream &operator<<(std::ostream &os, const param_helper::params::Parameters &params);

// -> ermöglicht auch die rekursive suche von parametern (ToDo)

/*template<class UnaryFunction>
void recursive_iterate(const json& j, UnaryFunction f)
{
    for(auto it = j.begin(); it != j.end(); ++it)
    {
        if (it->is_structured())
        {
            recursive_iterate(*it, f);
        }
        else
        {
            f(it);
        }
    }
}*/

/*template<class UnaryFunction>
auto recursive_find(json& j, UnaryFunction f)
{
    for(auto it = j.begin(); it != j.end(); ++it)
    {
        if (it->is_structured())
        {
            return recursive_find(*it, f);
        }
        else
        {
            std::cout << "Running parameter not found" << std::endl;
            std::exit(EXIT_FAILURE);
            return j.end();
        }
    }
}*/

#endif //PARAMHELPER_LIBRARY_H
