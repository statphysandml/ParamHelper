#ifndef PARAMHELPER_PROJECT_HPP
#define PARAMHELPER_PROJECT_HPP

#ifdef WINDOWS
#include <direct.h>
    #define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#include <iostream>
#include <fstream>
#include <clocale>
#include <sys/file.h>
#include <sys/stat.h>

#include "json.hpp"

using json = nlohmann::json;

namespace param_helper {
    namespace proj {
        extern std::string g_relative_path_to_project_root_dir;

        std::string path_to_executable();

        std::string project_root();

        void set_relative_path_to_project_root_dir(const std::string relative_path_to_project_root_dir);

        std::string get_relative_path_to_project_root_dir();

        std::string get_path_to(const std::string directory, const bool relative_path=true);
    }
}

#endif // PARAMHELPER_PROJECT_HPP