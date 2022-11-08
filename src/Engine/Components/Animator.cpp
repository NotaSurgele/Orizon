#include "Engine/Components/Animator.hpp"
#include "Engine/Time.hpp"

Animator::Animator(Entity *e) : _e(e)
{
}

Animator::Animation::Animation(Entity *e,
                    std::size_t const& nbFrame,
                    AnimatorRect const& anim_rect,
                    float const& animationSpeed,
                    std::string const& name) :
                    _animation_speed(animationSpeed),
                    _frames_nb(nbFrame),
                    _offset_x(anim_rect[0]),
                    _offset_y(anim_rect[1]),
                    _sizeWidth(anim_rect[2]),
                    _sizeHeight(anim_rect[3]),
                    _name(name)
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

const std::vector<sf::IntRect>& Animator::Animation::get_animation_frames()
{
    return _frames;
}

const std::string& Animator::Animation::name()
{
    return _name;
}

const std::vector<sf::IntRect>& Animator::get_animation_frames(
                                        std::string const& name)
{
    return _animation_map[name].get_animation_frames();
}

void Animator::Animation::playAnimation(const bool loop)
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

    _animation_map.insert(std::pair<std::string, Animation>(name, anim));
    return _animation_map[name];
}

void Animator::Animation::reset()
{
    _index = 0;
}

void Animator::playAnimation(std::string const& anim, const bool loop)
{
    try {
        _animation_map[anim].playAnimation(loop);
        _currentAnimation = anim;
    } catch(...) {
        std::cerr << "Cannot play animation: " << anim
            << " does not exist !" << std::endl;
    }
}

Animator::Animation& Animator::currentAnimation()
{
    return _animation_map[_currentAnimation];
}

void Animator::reset(std::string const& anim)
{
    try {
        _animation_map[anim].reset();
    } catch (...) {
        std::cerr << "Cannot reset animation: " << anim
            << " does not exist !" << std::endl;
    }
}
