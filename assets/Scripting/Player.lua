local Card = require 'assets.Scripting.Card'
local Draw = require 'assets.Scripting.Draw'
local Utils = require 'assets.Scripting.Utils'
local GlobalVariable = require 'assets.Scripting.Global'

local manager = nil

-- Junk

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

function resetCardPosition()
    position = Vector2f.new(-200, 300)
    angle = -5

    for i, card in pairs(cards) do
        card:playState("resetCard", position, angle)

        position.x = position.x + 120
        angle = angle + 4
    end
end

function createCard()
    local card = Card.new(hud, position, scale, camera)

    card:rotate(angle)
    card:initState()
    card:setCallback(function()
        local script = manager:getComponentScript()
        local bounds = card:getBounds()
        local enemy = script:call("contain", bounds)

        GlobalVariable.selectedCard = nil
        if enemy == nil then
            return
        end
        enemy:call("takeDamage", 100)
        for k, v in pairs(cards) do
            if v == card then
                cards[k] = nil
                break
            end
        end
        card:destroy()
        resetCardPosition()
    end)
    table.insert(cards, card)
    position.x = position.x + 120
    angle = angle + 4
end

function cardInit()
    position = Vector2f.new(-200, 300)
    scale = Vector2f.new(1, 1)
    angle = -5

    for i=1, 2 do
        createCard()
    end

    -- Draw cards
    draw = Draw.new(hud, _self:getComponentScript())
    draw:initState()
end

function handleAnimation()
    animator:playAnimation("idle", true)
end

function Start()
    transform = _self:getComponentTransform2D()
    animator = _self:getComponentAnimator()
    manager = System.getEntity("EnemyManager")
    hud = System.getEntity("Hud"):getComponentCanvas()
    camera = System.getEntity("Camera"):getComponentView()

    cardInit()
end

function Update()
    handleAnimation()

    if Stats.health <= 0 then
    	print("Player is dead restart")
    end
    for v, card in pairs(cards) do
        card:update()
    end
    draw:update()
end

function Destroy()
    for _, card in ipairs(cards) do
        hud:removeObject(card)
    end
end
