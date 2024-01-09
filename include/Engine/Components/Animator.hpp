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
                      float const& animationSpeed,
                      std::string  name);
            ~Animation() = default;
            Animation() = default;

            const std::vector<sf::IntRect>& getAnimationFrames();
            void playAnimation(bool loop);
            void setSprite(Sprite *sprite);
            void reset();

            void setSpeed(const float& speed)
            {
                _animation_speed = speed;
            }

            void setOffsetRect(const std::array<float, 4>& rect)
            {
                _offset_x = rect[0];
                _offset_y = rect[1];
                _sizeWidth = rect[2];
                _sizeHeight = rect[3];

                float x = _offset_x;
                float y = _offset_y;

                _frames.clear();
                for (std::size_t i = 0; i < _frames_nb; i++) {
                    sf::IntRect newFrame(x, y, _sizeWidth, _sizeHeight);

                    _frames.push_back(newFrame);
                    x += _sizeWidth;
                }
            }

            void setName(const std::string& name)
            {
                _name = name;
            }

            void setFramesNumber(const std::size_t& nb)
            {
                _frames_nb = nb;
            }

            std::size_t getFramesNumber() const { return _frames_nb; }
            std::string name();
            std::array<float, 4> getOffsetRect() const { return { _offset_x, _offset_y, _sizeWidth, _sizeHeight }; }
            float getSpeed() const { return _animation_speed; }

    private:
            std::string _name;
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
    explicit Animator(Entity *e);
    ~Animator() = default;

    const std::vector<sf::IntRect>& getAnimationFrames(std::string const& name);
    std::vector<Animation>& getAnimations() { return _animations; };

    void playAnimation(std::string const& name, bool const loop);
    Animation& newAnimation( std::size_t const &nbFrames,
                             AnimatorRect const& anim_rect,
                             float const& animationSpeed,
                             std::string const& name);
    void reset(std::string const& anim);
    void resetCurrentAnimation();
    void destroy() override final {}
    Animation& currentAnimation();

    bool empty() { return _animations.empty(); }

private:
    Entity *_e;
    std::string _currentAnimation;
    std::vector<Animation> _animations;
};
