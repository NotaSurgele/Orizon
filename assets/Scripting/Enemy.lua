local Enemy = {
    entity = nil,
    health = nil,
    turn = false,
}

Enemy.__index = Enemy

function Enemy.new()
    local self = setmetatable({}, Enemy)
    self.entity = Entity.new()
    self.health = 100
    self.hitbox = nil
    return self
end

function Enemy:Start()
end

function Enemy:Update(player, enemyManager)
    if self.health <= 0 then
        self.entity:destroy()
        return
    end

    -- if its not is turn then do nothing
    if self.turn == false then
    	return
    end

    -- Attack logic
    player:takeDamage(10)
    enemyManager:hasPlayed()
    self.turn = false
end

function Enemy:getEntity()
    return self.entity
end

function Enemy:takeDamage(amount)
    self.health = self.health - amount
end

function Enemy:setHitboxData(x, y, w, h)
    self.hitbox = FloatRect.new(x, y, w, h)
end

function Enemy:Play(turn)
    self.turn = turn
end

function Enemy:setTurn(turn)
	self.turn = turn
end

function Enemy:getTurn()
    return self.turn
end

function Enemy:getHitbox()
    return self.hitbox
end

function Enemy:Destroy()
    self.entity:destroy()
end

return Enemy