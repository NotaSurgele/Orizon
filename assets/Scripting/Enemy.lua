local Enemy = {
    entity = nil,
    health = nil
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

function Enemy:Update()
    if self.health <= 0 then
        self.entity:destroy()
    end
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

function Enemy:getHitbox()
    return self.hitbox
end

function Enemy:Destroy()
    self.entity:destroy()
end

return Enemy