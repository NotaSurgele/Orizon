local Enemy = require "assets.Scripting.Enemy"

local EnemyManager = {
    enemies = {}
}

EnemyManager.__index = EnemyManager

local myTurn = false
local index = 1

--824 472

function EnemyManager.new()
    local self = setmetatable({}, EnemyManager)
    self.enemies = {}
    return self
end

function EnemyManager:createEnemies()
    local x = 824
    local y = 472

    for i=0, 3 do
        local enemy = {
            instance = nil,
            entity = nil
        }
        local entity = Entity.new()
        local texture = ResourceManager.getResource("cross")

        enemy.transform = entity:addTransform2D(x, y)
        enemy.sprite = entity:addSprite(texture)

        local bounds = enemy.sprite:getGlobalBounds()

        enemy.mainScript = entity:addScript("./assets/Scripting/Enemy.lua")
        enemy.entity = entity
        enemy.mainScript:call("setHitboxData", x, y, bounds.width, bounds.height)
        System.pushEntity(entity)
        table.insert(enemies, enemy)
        x = x + 150
    end
end

function EnemyManager:contain(bounds)
    for _, enemy in ipairs(enemies) do
        box = enemy.mainScript:call("getHitbox")

        if box:intersects(bounds) == true then
            return enemy.mainScript
        end
    end
    return nil
end

function EnemyManager:Start()
    self:createEnemies()
end

function EnemyManager:Update()
    for k,enemy in pairs(self.enemies) do
        enemy:Update()
    end
end

function EnemyManager:Destroy()
    for pos, enemy in pairs(self.enemies) do
        enemy:Destroy()
    end

    for pos, enemy in ipairs(enemies) do
        table.remove(enemies, pos)
    end
    print(#enemies)
end