/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  Main file to interact with user.
 *
 *        Version:  1.0
 *        Created:  25.05.2014 13:27:37
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pavel Iakovenko (Draif), draif-33@yandex.ru, iakovenkopavel@gmail.com
 *   Organization:  2nd year student, DIHT, MIPT
 *
 * =====================================================================================
 */
#include <iostream>
#include "KDTree.hpp"
#include "ImageTuple.hpp"
#include "ImageProcessing.hpp"
#include <dirent.h>

void PrintHelpInformation() {
    std::cout << "To specify directrory with images: use \"--directory=<path_to_directory>\"(no spaces) as programm argument" 
    << std::endl;
    std::cout << "List of commands" << std::endl 
    << "To add picture to search selection type: add <path to picture>" << std::endl
    << "To find picture : find <path to picture>" << std::endl;

}

const std::vector<std::string> SimpleParser(std::string s) {
    std::vector<std::string> words;
    std::string temp;
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] == ' ' || s[i] == '\n' || s[i] == '\t') {
            if (temp.empty()) {
                continue;
            }
            words.push_back(temp);
            temp.clear();
            continue;
        }
        temp += s[i];
    }
    if (!temp.empty()) {
        words.push_back(temp);
    }
    return words;
}

int IsDots(const char *name) {
    return (strcmp(name, ".") == 0 || strcmp(name, "..") == 0);
}

char *GetFullName(const char *path, const char *name) {
    int len = strlen(path) + strlen(name) + 2;
    char *fullName = (char *)calloc(len, sizeof(char));
    if (!fullName) {
        return NULL;
    }
    snprintf(fullName, len, "%s%s%s", path, "/", name);
    return fullName;
}

void loadFromDirectory(std::vector<ImageTuple>& vec, std::string path, bool showImage) {
    struct dirent *entry = NULL;
    DIR *dir = opendir(path.c_str());
    if (!dir) {
        return;
    }
    while ((entry = readdir(dir)) != NULL) {
        char *fileName = NULL;
        if (entry->d_name[0] == '.') {
            continue;
        }
        fileName = GetFullName(path.c_str(), entry->d_name);
        try { 
            vec.push_back(ConvertToTuple(std::string(fileName), showImage));
            std::cout << "add " << fileName << std::endl;
        } catch (std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
            std::cerr << "Can't process file: " << fileName << std::endl;
        }
        free(fileName);
    }
    closedir(dir);
}

void Test() {
    KDTree<ImageTuple, VectorDistance> tree(1200);
    tree.AddPoint(ConvertToTuple("love.jpg"));
    tree.AddPoint(ConvertToTuple("love1.jpg"));
    tree.AddPoint(ConvertToTuple("love2.jpg"));
    tree.GetNearestPoint(ConvertToTuple("love.jpg")).PrintInformation();
    tree.GetNearestPoint(ConvertToTuple("love1.jpg")).PrintInformation();
    tree.GetNearestPoint(ConvertToTuple("love2.jpg")).PrintInformation();
    tree.GetNearestPoint(ConvertToTuple("love3.jpg")).PrintInformation();
    tree.GetNearestPoint(ConvertToTuple("love4.jpg")).PrintInformation();
    tree.GetNearestPoint(ConvertToTuple("conference.jpg")).PrintInformation();
}

int main(int argc, char** argv) {
    bool showImage = false;
    std::vector<ImageTuple> imageVector;
    for (int i = 1; i < argc; ++i)
        if (argv[i] == "--help" || argv[i] == "-h")
            PrintHelpInformation();
        else if (argv[i] == "--show")
            showImage = true;
        else {
            std::string s(argv[i]);
            auto p = s.find("=");
            if (p != std::string::npos && s.substr(0, p) == "--directory")
                loadFromDirectory(imageVector, s.substr(p + 1), showImage);
            else {
                std::cerr << "Unknown programm command: " << argv[i] << std::endl;
                return 1;
            }
        }
    
    const size_t size = 20;
    const size_t dimension = size * size * 3;
    KDTree<ImageTuple, VectorDistance> tree(dimension);
    if (!imageVector.empty()) 
        tree.Build(imageVector);
    std::string request;
    
    std::cout << "Please, enter the command. For information type \"--help\". To exit -\"exit\"" << std::endl;
    while (std::getline(std::cin, request)) {
        auto req = SimpleParser(request);
        if (req.size() > 2) {
            std::cerr << "Too many arguments! Type --help for help information." << std::endl;
            continue;
        }
        if (req.front() ==  "add") {
            if (req.size() == 1)
                std::cerr << "Path is empty!" << std::endl;
            try {
                tree.AddPoint(ConvertToTuple(req.back(), showImage));
            } catch (std::runtime_error& e) {
                std::cerr << e.what() << std::endl;
                std::cerr << "Perhaps you type wrong path" << std::endl;
            }
            std::cout << "Image was added." << std::endl;
        } else if (req.front() ==  "find") {
            if (req.size() > 2) {
                std::cerr << "Too many arguments! Type --help for help information." << std::endl;
                continue;
            }
            try {
                tree.GetNearestPoint(ConvertToTuple(req.back())).PrintInformation();
            } catch (std::runtime_error& e) {
                std::cerr << e.what() << std::endl;
                std::cerr << "Perhaps you type wrong path" << std::endl;
            }
        } else if (req.front() == "--help")
            PrintHelpInformation();
        else if (req.front() == "print")
            tree.PrintTree();
        else if (req.front() == "exit" || req.front() == "q")
            return 0;
        else  
            std::cerr << "Unknown command - \"" << req.front() << 
                "\" For information type \"--help\"." << std::endl;
    }
    return 0;    
}
