class IComponent {
    public:
        IComponent() = default;
        ~IComponent() = default;

        virtual void update() = 0;
};