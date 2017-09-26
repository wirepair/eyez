#ifndef LOADER_H
#define LOADER_H

#include <iostream>
#include <fstream>
#include <string>

std::string LoadShader(const std::string& filename)
{
    std::string content;
    std::ifstream fileStream(filename, std::ios::in);

    if(!fileStream.is_open()) 
    {
        std::cerr << "Could not read file " << filename << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    while(!fileStream.eof())
    {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

#endif // LOADER_H