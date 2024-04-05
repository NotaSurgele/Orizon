local Enemy = require "assets.Scripting.Enemy"

local EnemyManager = {
    enemies = {}
}

EnemyManager.__index = EnemyManager

--824 472

function EnemyManager.new()
    local self = setmetatable({}, EnemyManager)
    return self
end

function EnemyManager:createEnemies()
    local x = 824
    local y = 472

    for i=0, 3 do
        local enemy = Enemy.new()
        local texture = ResourceManager.getResource("cross")
        local bounds = nil

        enemy.entity:addSprite(texture)
        bounds = enemy.entity:getSprite():getGlobalBounds()
        enemy.entity:addTransform2D(x, y)
        enemy.entity:addTag("Enemy " .. tostring(i))
        enemy:setHitboxData(x, y, bounds.width, bounds.height)
        System.pushEntity(enemy.entity)
        table.insert(self.enemies, enemy)
        x = x + 150
    end
end

function EnemyManager:contain(bounds)
    for _, enemy in ipairs(self.enemies) do
        local box = enemy:getHitbox()

        if box:intersects(bounds) == true then
            return enemy
        end
    end
    return nil
end

function EnemyManager:Start()
    self:createEnemies()
end

function EnemyManager:Update()
    for k, enemy in pairs(self.enemies) do
        enemy:Update()
    end
end

function EnemyManager:Destroy()
    for pos, enemy in pairs(self.enemies) do
        enemy:Destroy()
    end

    for pos, enemy in pairs(self.enemies) do
        enemy = nil
        table.remove(self.enemies, pos)
    end
    print(#self.enemies)
end

return EnemyManager