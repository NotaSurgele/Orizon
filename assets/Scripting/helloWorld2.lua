toto = "titi"

function displayTable(tbl)
    for key, value in pairs(tbl) do
        print(key, value)
    end
end

function printHello(entity)
    print(entity:getComponentTransform2D())
end


function getPosition(entity)
    print(_self)
end

function Start()
end

function Update()
end
