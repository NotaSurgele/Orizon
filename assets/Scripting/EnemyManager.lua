
enemies = {}

--864.66668701172 331.33334350586

function createEnemies()
    x = 864
    y = 150

    for i=0, 3 do
        enemy = {
            entity = nil,
            sprite = nil,
            mainScript = nil,
            transform = nil
        }
        entity = Entity.new()

        texture = ResourceManager:getResource("cross")

        enemy.transform = entity:addComponentTransform2D(x, y)
        enemy.sprite = entity:addComponentSprite(texture)

        bounds = enemy.sprite:getGlobalBounds()

        enemy.mainScript = entity:addComponentScript("./assets/Scripting/Enemy.lua")
        enemy.entity = entity
        enemy.mainScript:call("setHitboxData", x, y, bounds.width, bounds.height)
        System.pushEntity(entity)
        table.insert(enemies, enemy)
        y = y + 200
    end
end

function contain(bounds)
    for _, enemy in ipairs(enemies) do
        box = enemy.mainScript:call("getHitbox")

        if box:intersects(bounds) == true then
            return enemy.mainScript
        end
    end
    return nil
end

function Start()
    createEnemies()
end

function getMe()
    return _self
end

function Update()
    local mouse = System.getGlobalMousePosition()

    ---print(mouse.x, mouse.y)
end

function Destroy()
    for pos, enemy in ipairs(enemies) do
        if enemy ~= nil then
            enemy.entity:destroy()
        end
    end

    for pos, enemy in ipairs(enemies) do
        table.remove(enemies, pos)
    end
    print(#enemies)
end