transform = nil
animator = nil

function Start()
<<<<<<< HEAD
    transform = self:getComponentTransform2D()
    animator = self:getComponentAnimator()
    print(animator, transform)
end

function handleAnimation()
    animator:playAnimation("idle", true)
end

function Update()
    handleAnimation()
    if (Input.isActionKeyPressed("MoveUp")) then
        transform.position.y = transform.position.y - (200 * deltaTime);
    end

    if (Input.isActionKeyPressed("MoveDown")) then
        transform.position.y = transform.position.y + (200 * deltaTime);
    end

    if (Input.isActionKeyPressed("MoveLeft")) then
        transform.position.x = transform.position.x - (200 * deltaTime);
    end

    if (Input.isActionKeyPressed("MoveRight")) then
        transform.position.x = transform.position.x + (200 * deltaTime);
    end
end