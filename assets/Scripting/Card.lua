local Utils = require 'assets.Scripting.Utils'
local StateMachine = require 'assets.Scripting.StateMachine'

local Card = {}
Card.__index = Card

-- Private methods
local function handleAnimation(self, sprite)
    if self.button:isHovered() then
        self.stateMachine:play("onHover", sprite, self.animation)
    else
        self.stateMachine:play("onNothing", sprite, self.animation)
    end
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
        offsetY = -200,
        scale = Vector2f.new(1, 1)
    }
    self.stateMachine = StateMachine.new()
    return self
end

function Card:initState()
    self.stateMachine:insert("onHover", function(sprite, animation)
        local offset = self.button:getOffset()
        local fix = Utils.lerp(offset.y, animation.offsetY,  10 * deltaTime)

        sprite:setColor(Color.new(255, 0, 0, 255))
        animation.offsetY = -200
        if Input.isButtonPressed("Left") then
            return self.stateMachine:play("onDrag", sprite)
        end
        self.button:setOffset(0, fix)
    end)

    self.stateMachine:insert("onNothing", function(sprite, animation)
        local offset = self.button:getOffset()
        local scale = Utils.lerp(self.button:getSize().x, 1, 10 * deltaTime)
        local fix = Utils.lerp(offset.y, animation.offsetY, 10 * deltaTime)

        animation.offsetY = -100
        sprite:setColor(Color.new(255, 255, 255, 255))
        self.button:setScale(scale, scale)
        self.button:setOffset(0, fix)
    end)

    self.stateMachine:insert("onDrag", function(sprite)
        local mouse = System.getGlobalMousePosition()
        local scale = Utils.lerp(self.button:getSize().x, 0.3, 10 * deltaTime)
        local bounds = sprite:getGlobalBounds()

        --print("Bounds", bounds.x, bounds.y)
        mouse.x = ((mouse.x - self.camera:getCenter().x) - self.button:getBasePosition().x)
        mouse.y = ((mouse.y - self.camera:getCenter().y) - self.button:getBasePosition().y)
        self.button:setScale(scale, scale)
        self.button:setOffset(mouse.x - (bounds.width * .5), mouse.y - (bounds.height * .5))
    end)
end

function Card:getBounds()
    return self.button:getSprite():getGlobalBounds()
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

function Card:destroy()
    self.hud:removeObject(self.button)
end

return Card