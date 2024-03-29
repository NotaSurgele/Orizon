local hitbox = nil
local health = 100
local player = nil

local myTurn = false
local endTurn = false

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
    player = System.getEntity("player"):getComponentScript()
end

function Update()
    if health <= 0 then
        _self:destroy()
    end
    if myTurn == false then return end
    print("attack")
    Attack()
end

function Destroy()

end

function setTurn(turn)
    print("Enemies set turn", turn)
    myTurn = turn
end

function getEndTurn()
    return endTurn
end

function Attack()
    player:call("takeDamage", 40)
    endTurn = true
    myTurn = false
end
