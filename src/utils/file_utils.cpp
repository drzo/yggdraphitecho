// File utilities implementation
#include "file_utils.h"

#include <fstream>
#include <sstream>
#include <sys/stat.h>

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#define mkdir(path, mode) _mkdir(path)
#define stat _stat
#else
#include <dirent.h>
#include <unistd.h>
#endif

namespace llmchat {

bool file_exists(const std::string& path) {
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

bool is_directory(const std::string& path) {
    struct stat buffer;
    if (stat(path.c_str(), &buffer) != 0) {
        return false;
    }
    return S_ISDIR(buffer.st_mode);
}

bool create_directories(const std::string& path) {
    if (path.empty() || file_exists(path)) {
        return true;
    }
    
    std::string parent = get_directory(path);
    if (!parent.empty() && parent != path) {
        if (!create_directories(parent)) {
            return false;
        }
    }
    
    return mkdir(path.c_str(), 0755) == 0 || file_exists(path);
}

std::string read_file(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        return "";
    }
    
    std::ostringstream oss;
    oss << file.rdbuf();
    return oss.str();
}

bool write_file(const std::string& path, const std::string& content) {
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    
    file << content;
    return true;
}

std::vector<std::string> list_directory(const std::string& path) {
    std::vector<std::string> files;
    
#ifdef _WIN32
    WIN32_FIND_DATAA find_data;
    HANDLE hFind = FindFirstFileA((path + "\\*").c_str(), &find_data);
    
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            std::string name = find_data.cFileName;
            if (name != "." && name != "..") {
                files.push_back(name);
            }
        } while (FindNextFileA(hFind, &find_data));
        
        FindClose(hFind);
    }
#else
    DIR* dir = opendir(path.c_str());
    if (dir) {
        struct dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            std::string name = entry->d_name;
            if (name != "." && name != "..") {
                files.push_back(name);
            }
        }
        closedir(dir);
    }
#endif
    
    return files;
}

std::string get_file_extension(const std::string& path) {
    size_t pos = path.find_last_of('.');
    if (pos == std::string::npos) {
        return "";
    }
    return path.substr(pos + 1);
}

std::string get_filename(const std::string& path) {
    size_t pos = path.find_last_of("/\\");
    if (pos == std::string::npos) {
        return path;
    }
    return path.substr(pos + 1);
}

std::string get_directory(const std::string& path) {
    size_t pos = path.find_last_of("/\\");
    if (pos == std::string::npos) {
        return "";
    }
    return path.substr(0, pos);
}

std::string join_paths(const std::string& a, const std::string& b) {
    if (a.empty()) return b;
    if (b.empty()) return a;
    
    char sep = '/';
#ifdef _WIN32
    sep = '\\';
#endif
    
    if (a.back() == '/' || a.back() == '\\') {
        return a + b;
    }
    
    return a + sep + b;
}

} // namespace llmchat

