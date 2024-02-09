#pragma once
#include <string>
#include <fstream>
#include <iostream>

#include "external/json.hpp"


class Utils {
public:
    Utils() = default;
    ~Utils() = default;

    static inline std::string readFile(const std::string& filename, bool ret=false)
    {
        std::string file_content = "";
        std::fstream file(filename);
        std::string buff = "";

        if (!file.is_open())
            return "";
        while (std::getline(file, buff)) {
            file_content.append(buff);
            if (ret)
                file_content += "\n";
        }
        file.close();
        return file_content;
    }

    static inline std::vector<std::string> splitString(std::string s, const std::string& delimiter)
    {
        size_t pos = 0;
        std::string token;
        std::vector<std::string> content;
        while ((pos = s.find(delimiter)) != std::string::npos) {
            token = s.substr(0, pos);
            content.push_back(token);
            s.erase(0, pos + delimiter.length());
        }
        content.push_back(s);
        return content;
    }

    static inline bool writeFile(const std::string& filepath, const std::string& content)
    {
        try {
            std::ofstream file(filepath);

            if (!file.is_open())
                return false;
            file << content.c_str();
            file.close();
            return true;
        } catch (std::exception& error) {
            std::cerr << "Failed opening files named " << filepath << " " << error.what() << std::endl;
            return false;
        }
    }

    static inline nlohmann::json fileContentToJson(const std::string& fileContent)
    {
        return nlohmann::json::parse(fileContent);
    }

    static inline nlohmann::json readfileToJson(const std::string& filePath)
    {
        std::string fileContent = readFile(filePath);

        return fileContentToJson(fileContent);
    }
};