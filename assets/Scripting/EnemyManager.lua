local Enemy = require "assets.Scripting.Enemy"

local EnemyManager = {
    enemies = {}
}

EnemyManager.__index = EnemyManager

local enemies = {}

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
        local enemy = Enemy.new()
        local texture = ResourceManager.getResource("cross")

        enemy.entity:addTransform2D(x, y)
        enemy.entity:addSprite(texture)
        enemy:setHitboxData(enemy.entity:getSprite():getGlobalBounds())
        System.pushEntity(enemy.entity)
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

return EnemyManager