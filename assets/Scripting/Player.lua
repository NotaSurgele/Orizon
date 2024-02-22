local Card = require 'assets.Scripting.Card'
local Draw = require 'assets.Scripting.Draw'
local Utils = require 'assets.Scripting.Utils'

local manager = nil

-- Camera
local hud = nil
local camera = nil
local cameraCenter = nil

-- Cards
local cards = {}
local draw = {}

function createCard()
    local card = Card.new(hud, position, scale, camera)

    card:rotate(angle)
    card:initState()
    card:setCallback(function()
        local script = manager:getComponentScript()
        local bounds = card:getBounds()
        local enemy = script:call("contain", bounds)

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

    -- Draw
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

    --print("start")
    for v, card in pairs(cards) do
        print(v, card)
        card:update()
    end
    --print("end")

    draw:update()
end

function Destroy()
    for _, card in ipairs(cards) do
        hud:removeObject(card)
    end
end
