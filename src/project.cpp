#include "../include/param_helper/project.hpp"

namespace param_helper {
    namespace proj {

        std::string relative_path_to_project_root_dir = "./";

        std::string path_to_executable() {
            char cCurrentPath[FILENAME_MAX];

            if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
                return std::to_string(errno);

            cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; // not really required

            //printf ("The current working directory is %s", cCurrentPath);
            std::string cp = std::string(cCurrentPath);

            // std::cout << cp << std::endl;
            return cp;
        }

        std::string project_root() {
            return path_to_executable() + "/" + relative_path_to_project_root_dir;
        }

        void set_relative_path_to_project_root_dir(const std::string relative_path_to_project_root_dir_) {
            relative_path_to_project_root_dir = relative_path_to_project_root_dir_;
        }

        std::string get_relative_path_to_project_root_dir() {
            return relative_path_to_project_root_dir;
        }

        std::string get_path_to(const std::string directory, const bool relative_path)
        {
            if(relative_path)
                return project_root() + "/" + directory + "/";
            else
                return directory;
        }
    }
}