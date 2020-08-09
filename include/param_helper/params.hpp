#ifndef PARAMETERS_LIBRARY_HPP
#define PARAMETERS_LIBRARY_HPP

#include <iostream>
#include <fstream>
#include <complex>
#include <sys/file.h>
#include <sys/stat.h>

#include "json.hpp"
#include "pathfinder.hpp"

using json = nlohmann::json;

namespace std {

    template< class T > void to_json(json &j, const std::complex< T > &p) {
        j = json {p.real(), p.imag()};
    }

    template< class T > void from_json(const json &j, std::complex< T > &p) {
        p.real(j.at(0));
        p.imag(j.at(1));
    }
}

std::complex<double> complex_to_json(const json &j);

bool fexists(const std::string& filename);

json merge(const json &a, const json &b);

json subtract(const json &a, const json &b);  // (a- b)

bool construct_parameter_path(json& j, const std::string& parameter, std::vector<std::string>& parameter_path);

// von Moriz? - vermutlich!?? -> ermöglicht auch die rekursive suche von parametern

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

class Parameters {
public:
    Parameters() = default;

    explicit Parameters(const json params_) : params(params_)
    {}

    static Parameters create_by_params(const json params_)
    {
        return Parameters(params_);
    }

    static Parameters create_by_file(const std::string& directory, const std::string& filename)
    {
        return Parameters(read_parameter_file(directory, filename));
    }

    static std::string get_absolute_path(const std::string directory, const bool relative_path=true)
    {
        if(relative_path)
            return gcp() + directory;
        else
            return directory;
    }

    static bool check_if_parameter_file_exists(const std::string directory, const std::string filename, const bool relative_path=true)
    {
        std::string path = get_absolute_path(directory, relative_path);
        std::cout << "Check for exsiting file " << filename <<".json in: " << path << std::endl;
        return fexists(path + "/" + filename + ".json");
    }

    static json read_parameter_file(const std::string directory, const std::string filename, const bool relative_path=true)
    {
        std::string path = get_absolute_path(directory, relative_path);

        int fd;
        fd = open(path.c_str(), O_RDWR | O_CREAT, 0666);
        flock(fd, LOCK_EX/* | LOCK_NB*/); // grab exclusive lock, fail if can't obtain.

        struct stat sb;
        if (stat((path + "/" + filename + ".json").c_str(), &sb) != 0)
        {
            std::cout << "File does not exist: " << path + "/" + filename + ".json" << std::endl;
            std::exit(EXIT_FAILURE);
        }
        std::ifstream i(path + "/" + filename + ".json");
        json k;
        i >> k;
        std::cout << "Simulation file loaded" << std::endl;
        close(fd);
        return k;
    }

    static void write_parameter_file(json parameters, const std::string directory, const std::string filename, const bool relative_path=true)
    {
        std::string path = get_absolute_path(directory, relative_path);
        std::cout << "Writing parameter file into:" << path << std::endl;

        std::cout << parameters << std::endl;

        std::ofstream o(path + filename + ".json");
        o << std::setw(4) << parameters << std::endl;
    }

    void write_to_file(const std::string directory, const std::string filename, const bool relative_path=true) const
    {
        write_parameter_file(params, directory, filename, relative_path);
    }

    template<typename params_T>
    void append_parameters(const params_T parameters_wrapper)
    {
        json parameters_with_identifiers;
        parameters_with_identifiers[parameters_wrapper.name()] = parameters_wrapper.get_json();
        params = merge(params, parameters_with_identifiers);
    }

    template<typename params_T>
    void append_parameters(const params_T parameters_wrapper, std::string global_identifier)
    {
        json parameters;
        parameters[global_identifier] = parameters_wrapper.get_json();
        parameters[global_identifier]["name"] = parameters_wrapper.name();
        params = merge(params, parameters);
    }

    void merge_to_file(const std::string& directory, const std::string& filename)
    {
        std::cout << "start to read file" << std::endl;
        int fd;
        fd = open(filename.c_str(), O_RDWR | O_CREAT, 0666);
        flock(fd, LOCK_EX/* | LOCK_NB*/); // grab exclusive lock, fail if can't obtain.

        std::ifstream i(filename.c_str());

        json * k = new json();
        i >> (*k);
        std::cout << "start to merge" << std::endl;
        json l = merge(*k, params);
        write_to_file(directory, filename);
        close(fd);
    }

    template<typename T>
    T get_value_by_key(const std::string key) const
    {
        auto it_find = params.find(key);
        if(it_find != params.end())
            return it_find->get<T>();
        std::cout << "ERROR: Parameter '" << key << "' not found" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // Can probably be replaced by easier function-> params.value(key, default_val)
    template<typename T>
    T get_value_by_key(const std::string key, const T default_val) const
    {
        auto it_find = params.find(key);
        if(it_find != params.end())
            return it_find->get<T>();
        else
            return default_val;
    }

    template<typename T>
    T get_value_by_key(const std::string key, const T default_val, const bool set_default=true)
    {
        auto it_find = params.find(key);
        if(it_find != params.end())
            return it_find->get<T>();
        else
            if(set_default) add_entry(key, default_val);
            return default_val;
    }

    bool haskey(const std::string key)
    {
        auto it_find = params.find(key);
        return it_find != params.end();
    }

    template<typename T>
    void add_entry(const std::string key, const T val)
    {
        params[key] = val;
    }

    void delete_entry(const std::string key)
    {
        params.erase(key);
    }

    const json get_json() const {
        return params;
    }

    json& get_json() {
        return params;
    }

    static void generate_directory_if_not_present(const std::string directory, const bool relative_path=true)
    {
        std::string path = get_absolute_path(directory, relative_path);
        std::cout << path << std::endl;

        struct stat sb;
        if (stat(path.c_str(), &sb) != 0 && !S_ISDIR(sb.st_mode))
        {
            const int dir_err = mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
            if (-1 == dir_err)
            {
                printf("Error creating directory!\n");
                exit(1);
            }
        }
    }

protected:
    json params;
};

std::ostream& operator<< (std::ostream& os, const Parameters& params);

#endif //PARAMHELPER_LIBRARY_H
