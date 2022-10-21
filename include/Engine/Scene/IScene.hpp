#pragma once

class IScene {
    public:
        virtual void create() = 0;
        virtual void update() = 0;
        virtual void destroy() = 0;
};