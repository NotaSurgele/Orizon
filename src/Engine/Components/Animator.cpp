#include "Engine/Components/Animator.hpp"
#include "Engine/Time.hpp"

Animator::Animator(Entity *e,
                    std::size_t const& nbFrame,
                    AnimatorRect const& anim_rect,
                    float const& animationSpeed) :
                    _animation_speed(animationSpeed),
                    _frames_nb(nbFrame),
                    _offset_x(anim_rect[0]),
                    _offset_y(anim_rect[1]),
                    _sizeWidth(anim_rect[2]),
                    _sizeHeight(anim_rect[3])
{
    _sprite = e->getComponent<Sprite>();
    if (_sprite == nullptr) {
        std::cerr << "Entity does not have sprite component" << std::endl;
        return;
    }

    float x = _offset_x;
    float y = _offset_y;

    for (std::size_t i = 0; i < nbFrame; i++) {
        sf::IntRect rect(x, y, _sizeWidth, _sizeHeight);

        _frames.push_back(rect);
        x += _sizeWidth;
    }
}

const std::vector<sf::IntRect>& Animator::get_animation_frames()
{
    return _frames;
}

void Animator::playAnimation(bool loop)
{
    _currentTime += Time::deltaTime;

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

void Animator::reset(void)
{
    _index = 0;
}
