local Card = require 'assets.Scripting.Card'

local Draw = {}
Draw.__index = Draw

function Draw.new(hud, player)
    local texture = ResourceManager:getResource('card_back')
    local self = setmetatable(Draw, Card)
    local button = hud:addButton(Vector2f.new(400, 90),
            Vector2f.new(0.8, 0.8), texture)

    self.button = button
    self.player = player
    self.stateMachine = StateMachine.new()
    self.animation = {
        basePosition = Vector2f.new(1200, 590),
        sprite = Sprite.new(texture),
        play = false
    }

    self.animation.sprite:setPosition(1200, 590)
    self.animation.sprite:setScale(0.8, 0.8)
    return self
end

function Draw:initState()
    self.button:setCallback(function()
        self.player:createCard()
    end)
end

function Draw:update()
    Core:draw(self.animation.sprite)
end

return Draw