#ifndef PARAMHELPER_PROJECT_HPP
#define PARAMHELPER_PROJECT_HPP

#include <iostream>
#include <fstream>
#include <clocale>
#include <filesystem>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace param_helper {
    namespace proj {
        extern std::string g_relative_path_to_project_root_dir;

        std::string project_root();

        void set_relative_path_to_project_root_dir(const std::string relative_path_to_project_root_dir);

        std::string get_relative_path_to_project_root_dir();

        std::string get_path_to(const std::string directory, const bool relative_path=true);
    }
}

#endif // PARAMHELPER_PROJECT_HPP