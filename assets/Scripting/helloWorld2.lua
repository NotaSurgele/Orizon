i = 0

function displayTable(tbl)
    for key, value in pairs(tbl) do
        print(key, value)
    end
end

function getEntity()
    return _self
end

function printHello(x, y)
    print("Hello World", x, y)
    _self:getComponentTransform2D().position.x = x
    _self:getComponentTransform2D().position.y = y
    return nil
end


function getPosition(entity)
    print(entity)
end

function Start()
end

function Update()
end
