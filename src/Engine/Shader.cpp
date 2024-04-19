#include "Shader.hpp"
#include <iostream>

bool Shader::isAvailable()
{
    if (!sf::Shader::isAvailable()) {
        std::cerr << "[SHADER] Shaders are not available in your system" << std::endl;
        Shader::available = false;
        return false;
    }
    std::cout << "[SHADER] Shaders are available in your system" << std::endl;
    Shader::available = true;
    return true;
}

bool Shader::loadFromFile(const std::string &path, const sf::Shader::Type &type)
{
    if (!available) {
        std::cerr << "[SHADER] couldn't load shader because shader are not available in your system" << std::endl;
        return false;
    }
    return _shader.loadFromFile(path, type);
}

bool Shader::loadFromFile(const std::string &vertex, const std::string &fragment)
{
    if (!available) {
        std::cerr << "[SHADER] couldn't load shader because shader are not available in your system" << std::endl;
        return false;
    }
    return _shader.loadFromFile(vertex, fragment);
}

void Shader::setUniform(const std::string &name, float x)
{
    _shader.setUniform(name, x);
}

void Shader::setUniform(const std::string &name, const sf::Glsl::Vec2 &vector)
{
    _shader.setUniform(name, vector);
}

void Shader::setUniform(const std::string &name, const sf::Glsl::Vec3 &vector)
{
    _shader.setUniform(name, vector);
}

void Shader::setUniform(const std::string &name, const sf::Glsl::Vec4 &vector)
{
    _shader.setUniform(name, vector);
}

void Shader::setUniform(const std::string &name, int x)
{
    _shader.setUniform(name, x);

}

void Shader::setUniform(const std::string &name, const sf::Glsl::Ivec2 &vector)
{
    _shader.setUniform(name, vector);

}

void Shader::setUniform(const std::string &name, const sf::Glsl::Ivec3 &vector)
{
    _shader.setUniform(name, vector);

}

void Shader::setUniform(const std::string &name, const sf::Glsl::Ivec4 &vector)
{
    _shader.setUniform(name, vector);

}

void Shader::setUniform(const std::string &name, bool x)
{

    _shader.setUniform(name, x);
}

void Shader::setUniform(const std::string &name, const sf::Glsl::Bvec2 &vector)
{
    _shader.setUniform(name, vector);

}

void Shader::setUniform(const std::string &name, const sf::Glsl::Bvec3 &vector)
{

    _shader.setUniform(name, vector);
}

void Shader::setUniform(const std::string &name, const sf::Glsl::Bvec4 &vector)
{

    _shader.setUniform(name, vector);
}

void Shader::setUniform(const std::string &name, const sf::Glsl::Mat3 &matrix)
{

    _shader.setUniform(name, matrix);
}

void Shader::setUniform(const std::string &name, const sf::Glsl::Mat4 &matrix)
{

    _shader.setUniform(name, matrix);
}

void Shader::setUniform(const std::string &name, const sf::Texture &texture)
{

    _shader.setUniform(name, texture);
}

void Shader::setUniform(const std::string &name, sf::Shader::CurrentTextureType& type)
{

    _shader.setUniform(name, type);
}

void Shader::setUniformArray(const std::string &name, const float *scalarArray, std::size_t length)
{

    _shader.setUniform(name, scalarArray);
}

void Shader::setUniformArray(const std::string &name, const sf::Glsl::Vec2 *vectorArray, std::size_t length)
{

    _shader.setUniform(name, vectorArray);
}

void Shader::setUniformArray(const std::string &name, const sf::Glsl::Vec3 *vectorArray, std::size_t length)
{

    _shader.setUniform(name, vectorArray);
}

void Shader::setUniformArray(const std::string &name, const sf::Glsl::Vec4 *vectorArray, std::size_t length)
{

    _shader.setUniform(name, vectorArray);
}

void Shader::setUniformArray(const std::string &name, const sf::Glsl::Mat3 *matrixArray, std::size_t length)
{

    _shader.setUniform(name, matrixArray);
}

void Shader::setUniformArray(const std::string &name, const sf::Glsl::Mat4 *matrixArray, std::size_t length)
{

    _shader.setUniform(name, matrixArray);
}
