i = 0

function displayTable(tbl)
    for key, value in pairs(tbl) do
        print(key, value)
    end
end

function getEntity()
    return _self
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
