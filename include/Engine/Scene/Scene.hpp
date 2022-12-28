#pragma once

#include "Engine/Scene/IScene.hpp"
#include "Engine/System.hpp"
#include "Engine/Core.hpp"
#include <fstream>

class Scene : public IScene {
public:
    Scene() = default;
    ~Scene() = default;

    void create() override {};
    void update() override {};
    void destroy() override {};

    template <typename T>
    static void addEntity(T *entity)
    {
        System::addEntity(entity);
    }

    template <typename T, class... Args>
    static void addEntity(Args... args)
    {
        System::addEntity(args ...);
    }

    int loadSceneFromFile(const std::string& filename)
    {
        //std::string content = readConfigFile(filename);
        //nlohmann::json json_content = nlohmann::json::parse(content);
        IComponent *c = new Transform2D();

        std::cout << SIGNATURE(c) << std::endl;
        return 0;
    }

    //template <typename T>
    //static Entity* getEntity(Signature signature)
    //{
    //    return System::getEntity<T *>(signature);
    //}

    private:
        std::string readConfigFile(std::string const& filename)
        {
            std::string file_content = "";
            try {
                std::fstream file(filename);
                std::string buff = "";

                if (file.is_open()) {
                    while (std::getline(file, buff))
                        file_content.append(buff);
                    file.close();
                };
            } catch(std::exception &e) {
                std::cerr << e.what() << std::endl;
                throw e;
            }
            return file_content;
        }

};
