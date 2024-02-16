Utils = {}

function Utils:lerp(start, target, dt)
    return start + (target - start) * dt
end

function Utils:lerp2(start, target, dt)
    return Vector2f.new(
            self:lerp(start.x, target.y, dt),
            self:lerp(start.y, target.y, dt))
end

return Utils