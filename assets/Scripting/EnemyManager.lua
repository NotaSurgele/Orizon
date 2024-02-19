
local enemies = {}

--864.66668701172 331.33334350586

function createEnemys()
    x = 864
    y = 250

    for i=0, 3 do
        enemy = Entity.new()

        texture = ResourceManager:getResource("cross")
        enemy:addComponentTransform2D(x, y)
        enemy:addComponentSprite(texture)
        System.pushEntity(enemy)

        table.insert(enemies, enemy)
        y = y + 100
    end
end

function Start()
    createEnemys()
end

function getMe()
    return _self
end

function Update()
    local mouse = System.getGlobalMousePosition()

    ---print(mouse.x, mouse.y)
end

function Destroy()
end