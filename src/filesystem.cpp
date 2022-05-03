#include "../include/param_helper/filesystem.hpp"

namespace param_helper {
    namespace fs {
        bool fexists(const std::string path) {
            std::ifstream ifile(path.c_str());
            return (bool)ifile;
        }

        bool direxists(const std::string path) {
            if(std::filesystem::is_directory(path))
                return true;
            else
                return false;
        }

        void makedir(const std::string path)
        {
            // Check if parent path exists
            std::size_t found = path.find_last_of("/\\");
            if(not direxists(path.substr(0, found)))
                makedir(path.substr(0, found));

            if(not direxists(path)) {
                // std::cout << "Generate directory: " << path << std::endl;
                // Generate directory
                std::filesystem::create_directory(path);
            }
        }

        void generate_directory_if_not_present(const std::string directory, const bool relative_path)
        {
            std::string path = param_helper::proj::get_path_to(directory, relative_path);
            
            if(not direxists(path))
                makedir(path);
        }

        bool check_if_parameter_file_exists(const std::string directory, const std::string filename, const bool relative_path)
        {
            std::string path = param_helper::proj::get_path_to(directory, relative_path);
            // std::cout << "Check for existence of " << filename << ".json in: " << path << std::endl;
            return fexists(path + "/" + filename + ".json");
        }

        json read_parameter_file(const std::string directory, const std::string filename, const bool relative_path)
        {
            std::string path = param_helper::proj::get_path_to(directory, relative_path);

            if(not std::filesystem::is_regular_file(path + "/" + filename + ".json"))
            {
                std::cerr << "File does not exist: " << path + "/" + filename + ".json" << std::endl;
                std::exit(EXIT_FAILURE);
            }

            std::ifstream i(path + "/" + filename + ".json");
            json k;
            i >> k;
            return k;
        }

        void write_parameter_file(json parameters, const std::string directory, const std::string filename, const bool relative_path)
        {
            std::string path = param_helper::proj::get_path_to(directory, relative_path);
            // std::cout << "Writing parameter file into:" << path << std::endl;
            // std::cout << "\t-> Parameters: " << parameters << std::endl;
            generate_directory_if_not_present(path, false);

            std::ofstream o(path + filename + ".json");
            o << std::setw(4) << parameters << std::endl;
        }
    }
}
