local StateMachine = require 'assets.Scripting.StateMachine'

GameManager = {
    player = nil,
    enemyManager = nil,
    button = nil,
    turn = true,
}
GameManager.__index = GameManager

function GameManager.new()
    local self = setmetatable({}, GameManager)

    self.player = nil
    self.enemyManager = nil
    self.turn = true
    self.button = nil
    return self
end

function GameManager:hasPlayed()
    self.turn = true
end

function GameManager:Start(player, enemyManager)
    local canvas = System.getEntity("Hud"):getCanvas()
    self.player = player
    self.enemyManager = enemyManager

    -- setup endTour button
    local texture = ResourceManager.getTexture("end_tour")

    self.button = canvas:addButton(Vector2f.new(300, 300), Vector2f.new(1, 1), texture)
    self.button:setCallback(function ()
        if self.turn == false then
        	return
        end
    	self.turn = false
    end)
end

function GameManager:buttonColor()
    -- if its not player turn
    if self.turn == false then
    	self.button:setColor(Color.new(114, 116, 116, 255))
    	return
    end

    -- if player turn
    if self.button:isHovered() then
        self.button:setColor(Color.Red)
        return
    end
    self.button:setColor(Color.White)
end

function GameManager:Update()
    self:buttonColor()

--  Play State
    self.player:Play(self.turn)
    self.enemyManager:Play(not self.turn)

--  Update state
    self.player:Update()
    self.enemyManager:Update(self.player)
end

function GameManager:Destroy()
end

return GameManager
