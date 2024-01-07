e = nil

function Start()
    e = Entity.new()

    texture = ResourceManager:R_GET_RESSOURCE("grass")
    e:getComponentLayer():set(50)
    e:addComponentTransform2D(0, 0)
    e:addComponentSprite(texture)
end


function Update()
end

function Destroy()
    e:destroy()
end