#ifndef PARAMHELPER_FILESYSTEM_HPP
#define PARAMHELPER_FILESYSTEM_HPP


#include <iostream>
#include <fstream>
#include <clocale>
#include <filesystem>

#include <param_helper/project.hpp>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace param_helper
{
    namespace fs {
        bool fexists(const std::string path);

        bool direxists(const std::string path);

        void makedir(const std::string path);

        void generate_directory_if_not_present(const std::string directory, const bool relative_path=true);

        bool check_if_parameter_file_exists(const std::string directory, const std::string filename, const bool relative_path=true);

        json read_parameter_file(const std::string directory, const std::string filename, const bool relative_path=true);

        void write_parameter_file(json parameters, const std::string directory, const std::string filename, const bool relative_path=true);

        class Fileos {
        public:
            explicit Fileos(std::string fname, bool append=false) : os{} {
                if (append)
                    os.open(fname, std::ios_base::app);
                else
                    os.open(fname);
            }

            ~Fileos() {os.close();}

            std::ofstream *operator->() {return &os;}

            std::ofstream& get() {return os;}
        private:
            std::ofstream os;
        };
    }
}

#endif //PARAMHELPER_FILESYSTEM_HPP
