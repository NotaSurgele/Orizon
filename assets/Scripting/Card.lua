local Utils = require 'assets.Scripting.Utils'
local StateMachine = require 'assets.Scripting.StateMachine'

local Card = {}
Card.__index = Card

-- Private methods
local function dragAndDrop(self)
    mouse = System.getLocalMousePosition()
    mouse.x = (mouse.x - self.camera:getCenter().x) - self.button:getBasePosition().x
    mouse.y = (mouse.y - self.camera:getCenter().y) - self.button:getBasePosition().y

    if Input.isButtonPressed("Left") then
        self.animation.dest.x = mouse.x
        self.animation.dest.y = mouse.y
        self.button:setOffset(mouse.x, mouse.y)
    end
end

local function initState(card)
    card.stateMachine:insert("onHover", function(sprite, animation)
        local offset = card.button:getOffset()
        local fix = Utils:lerp(offset.y, animation.offsetY, 10 * deltaTime)
        animation.offsetY = -100
        sprite:setColor(Color.new(255, 0, 0, 255))

        if Input.isButtonPressed("Left") then
            card.stateMachine:play("onDrag")
        else
            card.button:setOffset(offset.x, fix)
        end
    end)

    card.stateMachine:insert("onNothing", function(sprite, animation)
        local offset = card.button:getOffset()
        local fix = Utils:lerp(offset.y, animation.offsetY, 10 * deltaTime)
        animation.offsetY = 0

        sprite:setColor(Color.new(255, 255, 255, 255))
        card.button:setOffset(0, fix)
    end)

    card.stateMachine:insert("onDrag", function()
        local mouse = System.getLocalMousePosition()
        mouse.x = (mouse.x - card.camera:getCenter().x) - card.button:getBasePosition().x
        mouse.y = (mouse.y - card.camera:getCenter().y) - card.button:getBasePosition().y

        card.button:setOffset(mouse.x, mouse.y)
    end)
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
        offsetY = 0
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

    if button:isHovered() then
        self.stateMachine:play("onHover", sprite, self.animation)
    else
        self.stateMachine:play("onNothing", sprite, self.animation)
    end
end

return Card