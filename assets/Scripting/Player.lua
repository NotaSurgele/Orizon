local Card = require 'assets.Scripting.Card'
local Draw = require 'assets.Scripting.Draw'


local Player = {
    entity = {},
}
Player.__index = Player

function Player.new()
    local self = setmetatable({}, Player)
    self.entity = nil
    self.enemyManager = nil
    self.turn = true
    return self
end


function Player:Start()
    self.entity = System.getEntity("player")
    transform = self.entity:getTransform2D()
    animator = self.entity:getAnimator()
end

function Player:Update()
    if Input.isActionKeyPressed("MoveLeft") then
        transform.position.x = transform.position.x - (10 * deltaTime)
    end
end

function Player:Destroy()
    self.entity:destroy()
end

return Player