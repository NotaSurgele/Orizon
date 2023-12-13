transform = nil
animator = nil
collider = nil
script = nil
test2 = nil
test = Entity.new()


Import(_state, "../assets/Scripting/debug.lua")

-- Callback function to handle collisions
function handleCollisionCallback(otherCollider)
    otherCollider:entity():destroy()
end

function displayTable(tbl)
    for key, value in pairs(tbl) do
        print(key, value)
    end
end

function Start()
    transform = _self:getComponentTransform2D()
    animator = _self:getComponentAnimator()
    collider = _self:getComponentBoxCollider()
    test:addComponentTransform2D()
    test:addComponentTag("toto")
    script = test:addComponentScript("../assets/Scripting/helloWorld2.lua")
    system.pushEntity(test)
    --displayTable(table)
    script:getState(_state, "kiki")
    kiki:printHello(kiki._self)
    --printHello()
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
--    print(test2.position.x, test.position.y)
end