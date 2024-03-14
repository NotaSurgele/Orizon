Utils = {}

function Utils.lerp(start, target, dt, epsilon)
    epsilon = epsilon or 0.0001

    local lerpedValue = start + (target - start) * dt
    if math.abs(lerpedValue - target) < epsilon then
        lerpedValue = target
    end
    return lerpedValue
end

function Utils.lerp2(start, target, dt)
    return Vector2f.new(
            self:lerp(start.x, target.y, dt),
            self:lerp(start.y, target.y, dt))
end

function Utils.arraySize(array)
    local counter = 0

    for _, _ in pairs(array) do
        counter = counter + 1
    end
    return counter
end

return Utils