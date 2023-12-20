transform = nil
animator = nil
collider = nil
script = nil
test2 = nil
test = Entity.new()

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
    content = Utils.readFile("../assets/map_test.tmj", true)
    json = require "json"
    table = json.decode('[1,2,3,{"x":10}]')
    print(table[4]["x"])
    system.pushEntity(test)
    content = Utils.readFile("../assets/map_test.tmj", false)
    script:call("move", 10, 10)
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