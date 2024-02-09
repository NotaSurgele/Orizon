#include <utility>

#include "Engine/Components/Animator.hpp"
#include "Engine/Time.hpp"

Animator::Animator(Entity *e) : _e(e)
{
}

Animator::Animation::Animation(Entity *e,
                    std::size_t const& nbFrame,
                    AnimatorRect const& anim_rect,
                    float const& animationSpeed,
                    std::string  name) :
                    _animation_speed(animationSpeed),
                    _frames_nb(nbFrame),
                    _offset_x(anim_rect[0]),
                    _offset_y(anim_rect[1]),
                    _sizeWidth(anim_rect[2]),
                    _sizeHeight(anim_rect[3]),
                    _name(std::move(name))
{
    _sprite = e->getComponent<Sprite>();
/*    if (_sprite == nullptr) {
        std::cerr << "[ANIMATOR] Entity does not have sprite component" << std::endl;
        return;
    }*/

    float x = _offset_x;
    float y = _offset_y;

    for (std::size_t i = 0; i < nbFrame; i++) {
        sf::IntRect rect(x, y, _sizeWidth, _sizeHeight);

        _frames.push_back(rect);
        x += _sizeWidth;
    }
}

const std::vector<sf::IntRect>& Animator::Animation::getAnimationFrames()
{
    return _frames;
}

std::string Animator::Animation::name()
{
    return _name;
}

const std::vector<sf::IntRect>& Animator::getAnimationFrames(
                                        std::string const& name)
{
    for (auto& it : _animations) {

        if (it.name().find(name) != std::string::npos) return it.getAnimationFrames();
    }
    return _animations[0].getAnimationFrames();
}

void Animator::Animation::setSprite(Sprite *sprite)
{
    _sprite = sprite;
}

void Animator::Animation::playAnimation(const bool loop)
{
    _currentTime += Time::deltaTime;

    if (!_sprite) return;
    if ((!loop && _index >= _frames_nb) ||
        (_currentTime < _animation_speed && _index > 0))
        return;
    if (_index == _frames_nb)
        reset();
    if (_index < _frames_nb) {
        sf::IntRect bounds = _frames[_index];

        _sprite->setTextureRect(bounds);
        _index++;
    }
    _currentTime = 0;
}

Animator::Animation& Animator::newAnimation(std::size_t const& nbFrame,
                            AnimatorRect const& animRect,
                            float const& speed,
                            std::string const& name)
{
    Animator::Animation anim = Animator::Animation(_e,
                                               nbFrame,
                                               animRect,
                                               speed,
                                               name);
    _animations.push_back(anim);
    return _animations.back();
}

void Animator::Animation::reset()
{
    _index = 0;
}

void Animator::playAnimation(std::string const& anim, const bool loop)
{
    for (auto& it : _animations) {
        if (it.name().find(anim) != std::string::npos) {
            it.setSprite(_e->getComponent<Sprite>());
            it.playAnimation(loop);
            _currentAnimation = it.name();
            return;
        }
    }
}

Animator::Animation& Animator::currentAnimation()
{
    for (auto& it : _animations) {
        if (it.name().find(_currentAnimation) !=
                std::string::npos) {
            return it;
        }
    }
    return _animations.back();
}

void Animator::resetCurrentAnimation()
{
    for (auto& it : _animations) {
        if (it.name().find(_currentAnimation) !=
                std::string::npos) {
            it.reset();
            return;
        }
    }
}

void Animator::reset(std::string const& anim)
{
    for (auto& it : _animations) {
        if (it.name().find(anim) != std::string::npos) {
            it.reset();
            return;
        }
    }
}
