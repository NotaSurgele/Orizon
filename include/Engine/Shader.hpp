#pragma once
#include <SFML/Graphics.hpp>

class Shader {
public:
    Shader() = default;
    ~Shader() = default;

    static bool isAvailable();
    bool loadFromFile(const std::string& path, const sf::Shader::Type& type);
    bool loadFromFile(const std::string& vertex, const std::string& fragment);

    const sf::Shader* operator*()
    {
        return &_shader;
    }

    void setUniform(const std::string& name, float x);


    void setUniform(const std::string& name, const sf::Glsl::Vec2& vector);


    void setUniform(const std::string& name, const sf::Glsl::Vec3& vector);


    void setUniform(const std::string& name, const sf::Glsl::Vec4& vector);


    void setUniform(const std::string& name, int x);


    void setUniform(const std::string& name, const sf::Glsl::Ivec2& vector);


    void setUniform(const std::string& name, const sf::Glsl::Ivec3& vector);


    void setUniform(const std::string& name, const sf::Glsl::Ivec4& vector);


    void setUniform(const std::string& name, bool x);


    void setUniform(const std::string& name, const sf::Glsl::Bvec2& vector);


    void setUniform(const std::string& name, const sf::Glsl::Bvec3& vector);


    void setUniform(const std::string& name, const sf::Glsl::Bvec4& vector);


    void setUniform(const std::string& name, const sf::Glsl::Mat3& matrix);


    void setUniform(const std::string& name, const sf::Glsl::Mat4& matrix);


    void setUniform(const std::string& name, const sf::Texture& texture);


    void setUniform(const std::string& name, sf::Shader::CurrentTextureType& );


    void setUniformArray(const std::string& name, const float* scalarArray, std::size_t length);


    void setUniformArray(const std::string& name, const sf::Glsl::Vec2* vectorArray, std::size_t length);


    void setUniformArray(const std::string& name, const sf::Glsl::Vec3* vectorArray, std::size_t length);


    void setUniformArray(const std::string& name, const sf::Glsl::Vec4* vectorArray, std::size_t length);


    void setUniformArray(const std::string& name, const sf::Glsl::Mat3* matrixArray, std::size_t length);


    void setUniformArray(const std::string& name, const sf::Glsl::Mat4* matrixArray, std::size_t length);


public:
    static inline bool available = false;
    std::string name;

private:
    sf::Shader _shader;
};