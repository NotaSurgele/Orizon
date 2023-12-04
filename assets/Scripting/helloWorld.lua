transform = self:getComponentTransform2D()

function Start()
    print("est cer que la c'est mieux ou pas ?r")
end

function Update()
    if (Input.isActionPressed("MoveUp")) then
        transform.position.y = transform.position.y - (200 * deltaTime);
    end

    if (Input.isActionPressed("MoveDown")) then
        transform.position.y = transform.position.y + (200 * deltaTime);
    end

    if (Input.isActionPressed("MoveLeft")) then
        transform.position.x = transform.position.x - (200 * deltaTime);
    end

    if (Input.isActionPressed("MoveRight")) then
        transform.position.x = transform.position.x + (200 * deltaTime);
    end
end