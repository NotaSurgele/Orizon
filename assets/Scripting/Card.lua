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
    card.stateMachine:insert("toto", function()
        print("Enfin ?")
    end)

    card.stateMachine:insert("tata", function()
        print("tata")
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
    self.stateMachine = StateMachine.new()
    self.animation = {
        play = true,
        dest = Vector2f.new(0, 0),
        baseScale = scale
    }
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
    self.stateMachine:play("toto")
--[[    local sprite = self.button:getSprite()
    local offset = self.button:getOffset()
    local fixedPositionY = Utils:lerp(offset.y, self.animation.dest.y, 10 * deltaTime)

    if self.button:isHovered() == true then
        if self.animation.dest.y == 0 then
            self.animation.dest.x = self.position.x

            self.animation.dest.y = self.animation.dest.y - 100
            self.animation.play = false
        end
        dragAndDrop(self)
        sprite:setColor(Color.new(255, 0, 0, 255))
    else
        if self.animation.play == false then
            self.animation.dest.y = self.animation.dest.y + 100
            self.animation.play = true
        end
        sprite:setColor(Color.new(255, 255, 255, 255))
    end
    self.button:setOffset(offset.x, fixedPositionY)]]
end

return Card