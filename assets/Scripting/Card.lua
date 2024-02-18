local Utils = require 'assets.Scripting.Utils'
local StateMachine = require 'assets.Scripting.StateMachine'

local Card = {}
Card.__index = Card

-- Private methods
local function initState(card)
    card.stateMachine:insert("onHover", function(sprite, animation)
        local offset = card.button:getOffset()
        local fix = Utils:lerp(offset.y, animation.offsetY,  10 * deltaTime)

        sprite:setColor(Color.new(255, 0, 0, 255))
        animation.offsetY = -100
        if Input.isButtonPressed("Left") then
            return card.stateMachine:play("onDrag")
        end
        card.button:setOffset(offset.x, fix)
    end)

    card.stateMachine:insert("onNothing", function(sprite, animation)
        local offset = card.button:getOffset()
        local scale = Utils:lerp(sprite:getScale().x, 1, 10 * deltaTime)
        local fix = Utils:lerp(offset.y, animation.offsetY, 10 * deltaTime)

        animation.offsetY = 0
        sprite:setColor(Color.new(255, 255, 255, 255))
        card.button:setScale(scale, scale)
        card.button:setOffset(10, fix)
    end)

    card.stateMachine:insert("onDrag", function()
        local mouse = System.getLocalMousePosition()
        local scale = Utils:lerp(card.button:getSize().x, 1.2, 10 * deltaTime)

        mouse.x = ((mouse.x - card.camera:getCenter().x) - card.button:getBasePosition().x)
        mouse.y = ((mouse.y - card.camera:getCenter().y) - card.button:getBasePosition().y)
        card.button:setScale(scale, scale)
        card.button:setOffset(mouse.x, mouse.y)
    end)
end

local function handleAnimation(self, sprite)
    if self.button:isHovered() then
        return self.stateMachine:play("onHover", sprite, self.animation)
    end
    self.stateMachine:play("onNothing", sprite, self.animation)
end

-- Class methods
function Card.new(hud, position, scale, camera)
    local texture = ResourceManager:getResource('card')
    local button = hud:addButton(position, scale, texture)
    local self = setmetatable({}, Card)

    self.hud = hud
    self.position = position
    self.scale = scale
    self.button = button
    self.camera = camera
    self.animation = {
        offsetY = 0,
        scale = Vector2f.new(2, 2)
    }
    self.stateMachine = StateMachine.new()
    initState(self)
    return self
end

function Card:rotate(angle)
    self.button:getSprite():rotate(angle)
end

function Card:setCallback(callback)
    self.button:setCallback(callback)
end

function Card:update()
    local button = self.button
    local sprite = button:getSprite()

    handleAnimation(self, sprite)
end

return Card