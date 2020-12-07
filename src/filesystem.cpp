//
// Created by lukas on 28.11.20.
//

#include "../include/param_helper/filesystem.hpp"

namespace param_helper {
    namespace fs {

        std::string prfs::relative_path_to_project_root_dir = "/";

        std::string prfs::project_root() {
            char cCurrentPath[FILENAME_MAX];

            if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
                return std::to_string(errno);

            cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; // not really required

            //printf ("The current working directory is %s", cCurrentPath);
            std::string cp = std::string(cCurrentPath);

            // std::cout << cp << std::endl;
            return cp + relative_path_to_project_root_dir;
        }

        void prfs::set_relative_path_to_project_root_dir(const std::string relative_path_to_project_root_dir_) {
            relative_path_to_project_root_dir = relative_path_to_project_root_dir_;
        }

        std::string prfs::get_relative_path_to_project_root_dir() {
            return relative_path_to_project_root_dir;
        }

        std::string prfs::get_path_to(const std::string directory, const bool relative_path)
        {
            if(relative_path)
                return prfs::project_root() + "/" + directory + "/";
            else
                return directory;
        }

        bool fexists(const std::string& path) {
            std::ifstream ifile(path.c_str());
            return (bool)ifile;
        }

        bool direxists(const std::string& path) {
            struct stat sb;
            if (stat(path.c_str(), &sb) != 0 && !S_ISDIR(sb.st_mode))
                return false;
            else
                return true;
        }

        void makedir(const std::string& path)
        {
            std::cout << "Generate directory: " << path << std::endl;
            const int dir_err = mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
            if (-1 == dir_err)
            {
                printf("Error creating directory!\n");
                exit(1);
            }
        }

        void generate_directory_if_not_present(const std::string directory, const bool relative_path)
        {
            // ToDo: Add recurive method to generate path

            std::string path = prfs::get_path_to(directory, relative_path);

            if(not direxists(path))
                makedir(path);
        }

        bool check_if_parameter_file_exists(const std::string directory, const std::string filename, const bool relative_path)
        {
            std::string path = prfs::get_path_to(directory, relative_path);
            std::cout << "Check for existence of " << filename << ".json in: " << path << std::endl;
            return fexists(path + "/" + filename + ".json");
        }

        json read_parameter_file(const std::string directory, const std::string filename, const bool relative_path)
        {
            std::string path = prfs::get_path_to(directory, relative_path);

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
            std::cout << "Simulation file '" << filename << "' loaded" << std::endl;
            close(fd);
            return k;
        }

        void write_parameter_file(json parameters, const std::string directory, const std::string filename, const bool relative_path)
        {
            std::string path = prfs::get_path_to(directory, relative_path);
            std::cout << "Writing parameter file into:" << path << std::endl;

            std::cout << "\t-> Parameters: " << parameters << std::endl;

            std::ofstream o(path + filename + ".json");
            o << std::setw(4) << parameters << std::endl;
        }
    }
}
