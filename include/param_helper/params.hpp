#ifndef PARAMETERS_LIBRARY_HPP
#define PARAMETERS_LIBRARY_HPP

#include <iostream>
#include <fstream>
#include <complex>

#include <nlohmann/json.hpp>
#include <param_helper/filesystem.hpp>

using json = nlohmann::json;

namespace param_helper {
    namespace params {

        json merge(const json &a, const json &b);

        template<typename T>
        json merge_list_like(const json&a, const json &b)
        {
            auto a_vec = a.get<std::vector<T>>();
            auto b_vec = b.get<std::vector<T>>();
            a_vec.insert(a_vec.end(), b_vec.begin(), b_vec.end());
            auto merged_json = json(a_vec);
            return merged_json;
        }

        json subtract(const json &a, const json &b);  // (a- b)

        bool construct_parameter_path(json &j, const std::string parameter, std::vector<std::string> &parameter_path);

        template<typename T>
        static T entry_by_key(const json &params, const std::string key) {
            auto it_find = params.find(key);
            if (it_find != params.end())
                return it_find->get<T>();
            std::cerr << "ERROR: Parameter '" << key
                      << "' not found. Check if parameter is set per default in corresponding class or set parameter manually."
                      << std::endl;
            std::exit(EXIT_FAILURE);
        }

        class Parameters {
        public:
            Parameters() = default;

            explicit Parameters(const json params) : params_(params) {}

            static Parameters create_by_params(const json params) {
                return Parameters(params);
            }

            static Parameters create_by_file(const std::string directory, const std::string filename) {
                return Parameters(fs::read_parameter_file(directory, filename));
            }

            void write_to_file(const std::string directory, const std::string filename,
                               const bool relative_path = true) const {
                fs::write_parameter_file(params_, directory, filename, relative_path);
            }

            void merge_to_file(const std::string directory, const std::string filename,
                               const bool relative_path = true) {
                // std::cout << "Merging to file..." << std::endl;
                json k = fs::read_parameter_file(directory, filename, relative_path);
                params_ = merge(k, params_);
                write_to_file(directory, filename, relative_path);
            }

            // Never used? -> at least not by LatticeModelImplementations
            template<typename params_T>
            void append_parameters(const params_T& parameters_wrapper) {
                json parameters;
                parameters[parameters_wrapper.name()] = parameters_wrapper.get_json();
                params_ = merge(params_, parameters);
            }

            template<typename params_T>
            void append_parameters(const params_T& parameters_wrapper, std::string global_identifier) {
                json parameters;
                parameters[global_identifier] = parameters_wrapper.get_json();
                // parameters[global_identifier]["name"] = parameters_wrapper.name();
                params_ = merge(params_, parameters);
            }

            template<typename T>
            T get_entry(const std::string key) const {
                return entry_by_key<T>(params_, key);
            }

            template<typename T>
            T get_entry(const std::string key, const T default_val) const {
                auto it_find = params_.find(key);
                if (it_find != params_.end())
                    return it_find->get<T>();
                else
                    return default_val;
            }

            template<typename T>
            T get_entry(const std::string key, const T default_val, const bool set_default = true) {
                auto it_find = params_.find(key);
                if (it_find != params_.end())
                    return it_find->get<T>();
                else {
                    if (set_default) add_entry(key, default_val);
                    return default_val;
                }
            }

            bool haskey(const std::string key) {
                auto it_find = params_.find(key);
                return it_find != params_.end();
            }

            template<typename T>
            void add_entry(const std::string key, const T val) {
                params_[key] = val;
            }

            void delete_entry(const std::string key) {
                params_.erase(key);
            }

            const json get_json() const {
                return params_;
            }

            json &get_json() {
                return params_;
            }

        protected:
            json params_;
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
            std::cerr << "Running parameter not found" << std::endl;
            std::exit(EXIT_FAILURE);
            return j.end();
        }
    }
}*/

#endif //PARAMHELPER_LIBRARY_H
