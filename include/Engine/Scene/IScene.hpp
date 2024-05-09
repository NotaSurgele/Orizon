#pragma once

class IScene {
    public:
        virtual void start() = 0;
        virtual void update() = 0;
        virtual void destroy() = 0;
};