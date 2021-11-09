//
// Created by lukas on 28.11.20.
//

#ifndef PARAMHELPER_FILESYSTEM_HPP
#define PARAMHELPER_FILESYSTEM_HPP

#ifdef WINDOWS
#include <direct.h>
    #define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#include <iostream>
#include <fstream>
#include <sys/file.h>
#include <sys/stat.h>

#include "json.hpp"

using json = nlohmann::json;

namespace param_helper
{
    namespace fs {
        struct prfs
        {
            static std::string relative_path_to_project_root_dir;

            static std::string project_root();

            static void set_relative_path_to_project_root_dir(const std::string relative_path_to_project_root_dir_);

            static std::string get_relative_path_to_project_root_dir();

            static std::string get_path_to(const std::string directory, const bool relative_path=true);
        };

        bool fexists(const std::string& path);

        bool direxists(const std::string& path);

        void makedir(const std::string& path);

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
