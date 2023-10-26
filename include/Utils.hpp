#pragma once
#include <string>
#include <fstream>
#include <iostream>

#include "external/json.hpp"


class Utils {
public:
    Utils() = default;
    ~Utils() = default;

    static inline std::string readFile(const std::string& filename)
    {
        std::string file_content = "";
        std::fstream file(filename);
        std::string buff = "";

        if (!file.is_open())
            return "";
        while (std::getline(file, buff))
            file_content.append(buff);
        file.close();
        return file_content;
    }

    static inline nlohmann::json fileToJson(const std::string& fileContent)
    {
        return nlohmann::json::parse(fileContent);
    }
};