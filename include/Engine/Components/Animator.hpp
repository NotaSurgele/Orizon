#pragma once

#include "Engine/Components/Sprite.hpp"
#include <vector>
#include <array>

using AnimationIndex = std::size_t;
using AnimatorRect = std::array<int, 4>;

class Animator : public IComponent {
public:
    class Animation {
        public:
            std::vector<sf::IntRect> animFrames;

            Animation(Entity *e,
                      std::size_t const& nbFrame,
                      AnimatorRect const& anim_rect,
                      float const& animationSpeed);
            ~Animation() = default;
            Animation() = default;
            const std::vector<sf::IntRect>& get_animation_frames();
            void playAnimation(bool loop);
            void reset();
        private:
            float _currentTime = 0.0f;
            AnimationIndex _index = 0;
            Sprite *_sprite = nullptr;
            float _animation_speed = 0.0f;
            std::vector<sf::IntRect> _frames;
            float _sizeWidth = 0.0f;
            float _sizeHeight = 0.0f;
            float _offset_x = 0.0f;
            float _offset_y = 0.0f;
            std::size_t _frames_nb = 0;
    };
public:
    Animator(Entity *e);
    ~Animator() = default;

    const std::vector<sf::IntRect>& get_animation_frames(std::string const& name);
    void playAnimation(std::string const& name, bool const loop);
    Animation& newAnimation( std::size_t const &nbFrames,
                             AnimatorRect const& anim_rect,
                             float const& animationSpeed,
                             std::string const& name);
    void reset(std::string const& anim);
    void update() override final {}
    void destroy() override final {}

private:
    Entity *_e;
    std::unordered_map<std::string, Animation> _animation_map;
};
