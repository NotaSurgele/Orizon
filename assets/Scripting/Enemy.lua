local hitbox = nil
local health = 100

function setHitboxData(x, y, w, h)
    hitbox = FloatRect.new(x, y, w, h)
end

function getHitbox()
    return hitbox
end

function takeDamage(amount)
    health = health - amount
end

function Start()
end

function Update()

    if health <= 0 then
        _self:destroy()
    end

end

function Destroy()

end
