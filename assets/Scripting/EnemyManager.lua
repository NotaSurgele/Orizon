
local enemies = {}
local playerScript = nil
local myTurn = false
local index = 1

--824 472

function createEnemies()
    x = 824
    y = 472

    for i=0, 3 do
        local enemy = {
            entity = nil,
            sprite = nil,
            mainScript = nil,
            transform = nil
        }
        local entity = Entity.new()
        local texture = ResourceManager.getResource("cross")

        enemy.transform = entity:addComponentTransform2D(x, y)
        enemy.sprite = entity:addComponentSprite(texture)

        local bounds = enemy.sprite:getGlobalBounds()

        enemy.mainScript = entity:addComponentScript("./assets/Scripting/Enemy.lua")
        enemy.entity = entity
        enemy.mainScript:call("setHitboxData", x, y, bounds.width, bounds.height)
        System.pushEntity(entity)
        table.insert(enemies, enemy)
        x = x + 150
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

    playerScript = System.getEntity("player"):getComponentScript()
end

function Update()
    if myTurn == true then
        local enemy = enemies[index]

        if enemy == nil then
            enemies[index] = nil
        	index = index + 1
        	return
        end
        local endTurn = enemy.mainScript:call("getEndTurn")

        enemy.mainScript:call("setTurn", true)
        if endTurn == true then
        	index = index + 1
        end
    end
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

function setTurn(turn)
    myTurn = turn
end