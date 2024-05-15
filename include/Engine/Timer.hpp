#pragma once

#include "Time.hpp"

class Timer {
public:
    explicit Timer(float to) : _to(to) {};

    Timer() = default;
    ~Timer() = default;

    void update(float factor=1.0f)
    {
        if (_ended || _to <= 0.0f) {
            _ended = true;
            return;
        }
        _current += (Time::deltaTime * factor);

        _ended = _current >= _to;
    };

    void set(float to)
    {
        _to = to;
    }

    void reset()
    {
        _ended = false;
        _current = 0.0f;
    };

    bool ended() const
    {
        return _ended;
    };

    float current() const
    {
        return _current;
    };

    float to() const
    {
        return _to;
    }

private:
    float _current = 0.0f;
    float _to = 0.0f;
    bool _ended = false;
};