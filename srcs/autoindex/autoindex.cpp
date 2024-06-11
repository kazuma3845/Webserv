#include "autoindex.hpp"

std::string AutoIndex::create(const std::string& uri) {
    std::string page;
    const char* path = uri.c_str();
    std::string dirName(path);
    DIR* dir = opendir(path);

    page = "<html>\n"
           "    <head>\n"
           "        <title>Index of " + dirName + "</title>\n"
           "        <style>\n"
           "            table { width: 100%; border-collapse: collapse; }\n"
           "            th, td { padding: 8px; text-align: left; border-bottom: 1px solid #ddd; }\n"
           "            th { background-color: #f2f2f2; }\n"
           "        </style>\n"
           "    </head>\n"
           "    <body>\n"
           "        <h1>Index of " + dirName + "</h1>\n"
           "        <table>\n"
           "            <tr><th>Name</th><th>Last modified</th><th>Size</th></tr>\n";

    if (dir == NULL) {
        std::cerr << "Error: Directory not opened: " << path << std::endl;
        return "";
    }

    for (struct dirent* newdir = readdir(dir); newdir; newdir = readdir(dir)) {
        std::string name(newdir->d_name);
        std::string filePath = dirName + "/" + name;
        page += addLine(name, filePath);
    }
    
    page += "        </table>\n"
            "    </body>\n"
            "</html>\n";
    closedir(dir);
	std::ofstream outfile("test.html");
	outfile << page;
    return page;
}

std::string AutoIndex::addLine(const std::string& name, const std::string& path) {
    struct stat fileStat;
    if (stat(path.c_str(), &fileStat) == -1) {
        perror("stat");
        return "";
    }

    std::stringstream ss;
    ss << "            <tr>"
       << "<td><a href=\"" << name << "\">" << name << "</a></td>"
       << "<td>" << formatTime(fileStat.st_mtime) << "</td>"
       << "<td>" << ((S_ISDIR(fileStat.st_mode)) ? "-" : formatSize(fileStat.st_size)) << "</td>"
       << "</tr>\n";
    
    return ss.str();
}

std::string AutoIndex::formatTime(time_t rawTime) {
    struct tm* timeinfo = localtime(&rawTime);
    char buffer[80];
    strftime(buffer, 80, "%d-%b-%Y %H:%M", timeinfo);
    return std::string(buffer);
}

std::string AutoIndex::formatSize(off_t size) {
    std::stringstream ss;
    if (size >= 1024 * 1024) {
        ss << size / (1024 * 1024) << "M";
    } else if (size >= 1024) {
        ss << size / 1024 << "K";
    } else {
        ss << size << "B";
    }
    return ss.str();
}

// int main() {
//     AutoIndex ai;
//     std::string uri = "./"; // Vous pouvez changer cela pour le répertoire que vous voulez lister
//     std::cout << ai.create(uri) << std::endl;
//     return 0;
// }