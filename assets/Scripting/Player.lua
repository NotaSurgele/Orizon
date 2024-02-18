local Card = require 'assets.Scripting.Card'
local Utils = require 'assets.Scripting.Utils'

-- Camera
local hud = nil
local camera = nil
local cameraCenter = nil

-- Cards
cards = {}

local save = {}

function Start()
    transform = _self:getComponentTransform2D()
    animator = _self:getComponentAnimator()
    hud = System.getEntity("Hud"):getComponentCanvas()
    camera = System.getEntity("Camera"):getComponentView()

    cards_init()
    print(animator, transform)
end

function cards_init()
    cameraCenter = camera:getCenter()
    position = Vector2f.new(-200, 300)
    scale = Vector2f.new(1, 1)
    angle = -5

    for i=1, 1 do
        card = Card.new(hud, position, scale, camera)

        card:rotate(angle)
        card:setCallback(function() print(hud) end)
        table.insert(cards, card)
        position.x = position.x + 120
        angle = angle + 4
    end
end

function handleAnimation()
    animator:playAnimation("idle", true)
end

function Update()
    handleAnimation()

    for v, card in ipairs(cards) do
        card:update()
    end
end

function Destroy()
    for _, card in ipairs(cards) do
        hud:removeObject(card)
    end
end
