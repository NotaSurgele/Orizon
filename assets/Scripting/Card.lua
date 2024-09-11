
local StateMachine = require 'assets.Scripting.StateMachine'
local GlobalVariable = require 'assets.Scripting.Global'

local Card = {}
Card.__index = Card

-- Private methods
local function handleAnimation(self, sprite)
    if self.stateMachine:getCurrentState() == "resetCard" then
        self.stateMachine:play("resetCard", self.target, self.targetAngle)
        return
    end
    if self.button:isHovered() and GlobalVariable.selectedCard == nil
        or GlobalVariable.selectedCard == self then
        self.stateMachine:play("onHover", sprite, self.animation)
    else
        self.stateMachine:play("onNothing", sprite, self.animation)
    end
end

-- Class methods
function Card.new(hud, position, scale, camera)
    local self = setmetatable({}, Card)
    local texture = ResourceManager.getTexture('card')

    self.soundManager = System.getEntity("SoundManager"):getSound()
    self.soundIsPlaying = false
    self.hud = hud
    self.position = position
    self.scale = scale
    self.button = hud:addButton(position, scale, texture)
    self.camera = camera
    self.target = Vector2f.new(0, 0)
    self.targetAngle = 0
    self.animation = {
        offsetY = -200,
        scale = Vector2f.new(1, 1),
        dragPosition = Vector2f.new(-1, -1),
        line = Line.new(0, 0, 0, 0, 10, Color.Yellow)
    }
    self.stateMachine = StateMachine.new()
    return self
end

function Card:initState()
    self.stateMachine:insert("onHover", function(sprite, animation)
        local z = self.button:getZ()
        local offset = self.button:getOffset()
        local fix = Utils.lerp(offset.y, animation.offsetY,  10 * deltaTime)

        if self.soundIsPlaying == false then
            self.soundIsPlaying = true
            self.soundManager:play()
        end
        if z ~= 1 then
            self.button:setZ(1)
        end
        sprite:setColor(Color.Red)
        animation.offsetY = -200
        if Input.isButtonPressed("Left") then
            return self.stateMachine:play("onDrag", sprite)
        end
        self.button:setOffset(0, fix)
    end)

    self.stateMachine:insert("onNothing", function(sprite, animation)
        local z = self.button:getZ()
        local offset = self.button:getOffset()
        local scale = Utils.lerp(self.button:getSize().x, 1, 10 * deltaTime)
        local fix = Utils.lerp(offset.y, animation.offsetY, 10 * deltaTime)

        self.soundIsPlaying = false
        if z ~= 0 then
            self.button:setZ(0)
        end
        animation.offsetY = -100
        sprite:setColor(Color.White)
        self.button:setScale(scale, scale)
        self.button:setOffset(0, fix)
    end)

    self.stateMachine:insert("onDrag", function(sprite)
        local mouse = System.getGlobalMousePosition()
        local scale = Utils.lerp(self.button:getSize().x, 0.3, 10 * deltaTime)
        local bounds = sprite:getGlobalBounds()

        GlobalVariable.selectedCard = self
        if self.animation.dragPosition.x == -1
            and self.animation.dragPosition.y == -1 then
            self.animation.dragPosition.x = bounds.x + (bounds.width / 2)
            self.animation.dragPosition.y = bounds.y + (bounds.height / 2)

            self.animation.line:setStartPoint(self.animation.dragPosition)
        end
        self.animation.line:setEndPoint(mouse)
        mouse.x = ((mouse.x - self.camera:getCenter().x) - self.button:getBasePosition().x)
        mouse.y = ((mouse.y - self.camera:getCenter().y) - self.button:getBasePosition().y)
        self.button:setScale(scale, scale)
        self.button:setOffset(mouse.x - (bounds.width * .5), mouse.y - (bounds.height * .5))
        self.animation.line:draw()
    end)

    self.stateMachine:insert("resetCard", function(position, angle)
        local sprite = self.button:getSprite()
        local fixedRotation = 360 - sprite:getRotation()
        local basePosition = self.button:getBasePosition()
        local x = Utils.lerp(basePosition.x, position.x, 50 * deltaTime)
        local y = Utils.lerp(basePosition.y, position.y, 50 * deltaTime)

        if x == position.x and y == position.y then
            self.stateMachine:play("onNothing", sprite, self.animation)
            return
        end
        self.targetAngle = angle
        self.target.x = position.x
        self.target.y = position.y
        self.position.x = x
        self.position.y = y
        self.button:setBasePosition(x, y)
        self:rotate(fixedRotation + self.targetAngle)
    end)
end

function Card:playState(stateName, ...)
    self.stateMachine:play(stateName, ...)
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