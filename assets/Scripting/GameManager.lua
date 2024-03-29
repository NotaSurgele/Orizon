local StateMachine = require 'assets.Scripting.StateMachine'

local hud = nil
local endTourButton = nil
local stateMachine = StateMachine.new()
local playerTurn = true

local enemiesManager = nil
local player = nil

local function defineState()
	stateMachine:insert("onHover", function ()
        endTourButton:setColor(Color.Red)
	end)

	stateMachine:insert("onNothing", function ()
        endTourButton:setColor(Color.White)
	end)
end

function Start()
    enemiesManager = System.getEntity("EnemyManager"):getComponentScript()
    player = System.getEntity("player"):getComponentScript()
    hud = System.getEntity("Hud"):getComponentCanvas()

    local buttonTexture = ResourceManager:getResource("end_tour")
    endTourButton = hud:addButton(Vector2f.new(50, 50), Vector2f.new(1, 1), buttonTexture)

    endTourButton:setCallback(function ()
        endTourButton.clickable = not endTourButton.clickable
        playerTurn = not playerTurn
        player:call("setTurn", playerTurn)
        enemiesManager:call("setTurn", true)
    end)
    defineState()
end

function Update()
    if endTourButton:isHovered() == true then
        stateMachine:play("onHover")
    else
        stateMachine:play("onNothing")
    end
end

function Destroy()
end