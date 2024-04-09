local Card = require 'assets.Scripting.Card'
local Draw = require 'assets.Scripting.Draw'
local Utils = require 'assets.Scripting.Utils'

local Player = {
    entity = {},
    enemyManager = nil,
    turn = true
}
Player.__index = Player

-- Camera
local hud = nil
local camera = nil

-- Cards
local cards = {}
local draw = {}

local Stats = {
    health = 100,
    shield = 0,
    attack = 1
}

function Player:resetCardPosition()
    position = Vector2f.new(-200, 300)
    angle = -5

    for i, card in pairs(cards) do
        card:playState("resetCard", position, angle)

        position.x = position.x + 120
        angle = angle + 4
    end
end

function Player:createCard()
    local card = Card.new(hud, position, scale, camera)

    card:rotate(angle)
    card:initState()
    card:setCallback(function()
        local bounds = card:getBounds()
        local enemy = self.enemyManager:contain(bounds)

        GlobalVariable.selectedCard = nil
        if enemy == nil or
            self.turn == false then
            return
        end
        enemy:takeDamage(100)
        for k, v in pairs(cards) do
            if v == card then
                cards[k] = nil
                break
            end
        end
        card:destroy()
        self:resetCardPosition()
    end)
    table.insert(cards, card)
    position.x = position.x + 120
    angle = angle + 4
end

function Player:cardInit()
    position = Vector2f.new(-200, 300)
    scale = Vector2f.new(1, 1)
    angle = -5

    for i=1, 2 do
        self:createCard()
    end

    -- Draw cards
    draw = Draw.new(hud, self)
    draw:initState()
end

function Player:handleAnimation()
    animator:playAnimation("idle", true)
end

function Player.new()
    local self = setmetatable({}, Player)
    self.entity = nil
    self.enemyManager = nil
    self.turn = true
    return self
end

function Player:Play(turn)
    self.turn = turn
end

function Player:Start(enemyManager)
    self.entity = System.getEntity("player")
    self.enemyManager = enemyManager
    transform = self.entity:getTransform2D()
    animator = self.entity:getAnimator()
    hud = System.getEntity("Hud"):getCanvas()
    camera = System.getEntity("Camera"):getView()

    self:cardInit()
end

function Player:Update()
    self:handleAnimation()

    if Stats.health <= 0 then
    	print("Player is dead restart")
    	self:Destroy()
    	return
    end
    for v, card in pairs(cards) do
        card:update()
    end
end

function Player:Destroy()
    for _, card in ipairs(cards) do
        hud:removeObject(card.button)
    end
    self.entity:destroy()
end

function Player:getTurn()
    return self.turn
end

function Player:takeDamage(amount)
    print("Player is taking damage")
    Stats.health = Stats.health - amount
end

return Player