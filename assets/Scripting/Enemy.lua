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
    if health <= 0 then
        self.entity:destroy()
    end
end

function Enemy:getEntity()
    return self.entity
end

function Enemy:takeDamage(amount)
    health = health - amount
end

function Enemy:setHitboxData(hitbox)
    self.hitbox = hitbox
end

function Enemy:getHitbox()
    return self.hitbox
end

function Enemy:Destroy()
    self.entity:destroy()
end

return Enemy