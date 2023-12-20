i = 0

function displayTable(tbl)
    for key, value in pairs(tbl) do
        print(key, value)
    end
end

function getEntity()
    return _self
end

function move(x, y)
    transform = _self:getComponentTransform2D()
    transform.position.x = transform.position.x + x
    transform.position.y = transform.position.y + y
    print(transform.position.x, transform.position.y)
end

function printEntity(entity)
    if entity ~= nil then
        print(entity:getComponentTransform2D())
    end
end

function printHello()
    print("Hello")
end

function kill()
    _self:destroy()
end

function getPosition(entity)
    print(entity)
end

function Start()
end

function Update()
end
