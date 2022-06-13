#include <param_helper/project.hpp>

namespace param_helper {
    namespace proj {

        std::string g_relative_path_to_project_root_dir = "./";

        std::string project_root() {
            return std::filesystem::current_path().string() + "/" + g_relative_path_to_project_root_dir;
        }

        void set_relative_path_to_project_root_dir(const std::string relative_path_to_project_root_dir) {
            g_relative_path_to_project_root_dir = relative_path_to_project_root_dir;
            std::cout << " -- Setting relative path to project root dir to '" << relative_path_to_project_root_dir << "', resulting in '" << project_root() << "' as absolute project root dir --" << std::endl;
        }

        std::string get_relative_path_to_project_root_dir() {
            return g_relative_path_to_project_root_dir;
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