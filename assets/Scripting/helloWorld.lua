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

function Start()
    transform = _self:getComponentTransform2D()
    animator = _self:getComponentAnimator()
    collider = _self:getComponentBoxCollider()
    test:addComponentTransform2D()
    test:addComponentTag("toto")
    script = test:addComponentScript("../assets/Scripting/helloWorld2.lua")
    system.pushEntity(test)
    entity2 = script:call("getEntity")

    print("Before ", entity2:getComponentTransform2D().position.x, entity2:getComponentTransform2D().position.y)
    script:call("printHello", 100, 100)
    print("After ", entity2:getComponentTransform2D().position.x, entity2:getComponentTransform2D().position.y)
    tata()
    --displayTable(table)

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