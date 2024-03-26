local StateMachine = require 'assets.Scripting.StateMachine'

local hud = nil
local endTourButton = nil
local stateMachine = StateMachine.new()

local function defineState()
	stateMachine:insert("onHover", function ()
        endTourButton:getSprite():setColor(Color.new(255, 0, 0, 255))
	end)

	stateMachine:insert("onNothing", function ()
        endTourButton:getSprite():setColor(Color.new(255, 255, 255, 255))
	end)
end

function Start()
    hud = System.getEntity("Hud"):getComponentCanvas()

    local buttonTexture = ResourceManager:getResource("end_tour")
    endTourButton = hud:addButton(Vector2f.new(50, 50), Vector2f.new(1, 1), buttonTexture)
    endTourButton:setCallback(function ()
        endTourButton.clickable = not endTourButton.clickable
    end)
    defineState()
end

function Update()
    --if Input.isButtonReleased("Left") then
    --    print("coucou")
    --end
    if endTourButton.clickable == false then
        endTourButton:getSprite():setColor(Color.new(0, 0, 0, 255))
        return
    end
    if endTourButton:isHovered() then
        stateMachine:play("onHover")
    else
        stateMachine:play("onNothing")
    end
end

function Destroy()
end