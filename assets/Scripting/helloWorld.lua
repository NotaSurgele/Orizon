transform = nil
animator = nil
collider = nil
test = Entity.new()

Import(_state, "../assets/Scripting/helloWorld2.lua")

-- Callback function to handle collisions
function handleCollisionCallback(otherCollider)
    otherCollider:entity():destroy()
end

function Start()
    transform = _self:getComponentTransform2D()
    animator = _self:getComponentAnimator()
    collider = _self:getComponentBoxCollider()

    test:addComponentTransform2D()
    test:addComponentTag("toto")
    system.pushEntity(test)
    printHello()
    collider:onCollision(handleCollisionCallback)
    print(animator, transform, collider)
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