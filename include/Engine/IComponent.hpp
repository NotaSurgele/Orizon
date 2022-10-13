class IComponent {
    public:
        IComponent() = default;
        ~IComponent() = default;

        virtual void update() = 0;
        virtual void render() = 0;
};