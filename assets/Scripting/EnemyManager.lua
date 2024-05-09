local Enemy = require "assets.Scripting.Enemy"

local EnemyManager = {
    enemies = {},
    turn = false,
    gameManager = nil
}

EnemyManager.__index = EnemyManager

local index = 1

--824 472

function EnemyManager.new()
    local self = setmetatable({}, EnemyManager)
    self.turn = false
    self.gameManager = nil
    return self
end

function EnemyManager:createEnemies()
    local x = 824
    local y = 472

    for i=0, 3 do
        local enemy = Enemy.new()
        local texture = ResourceManager.getTexture("cross")
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

function EnemyManager:Start(gameManager)
    self:createEnemies()
    self.gameManager = gameManager
end

function EnemyManager:Play(turn)
	self.turn = turn
end

function EnemyManager:hasPlayed()
    self.enemies[index]:Play(false)
    index = index + 1
    if index > #self.enemies then
        index = 1
        self.turn = false
        self.gameManager:hasPlayed()
    end
end

function EnemyManager:Update(player)
    -- Set the current enemy to play
    if self.turn == true then
        print(#self.enemies, index)
        self.enemies[index]:Play(true)
    end

    for k, enemy in pairs(self.enemies) do
        enemy:Update(player, self)
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
end

return EnemyManager