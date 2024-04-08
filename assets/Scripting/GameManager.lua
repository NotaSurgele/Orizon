local StateMachine = require 'assets.Scripting.StateMachine'

GameManager = {
    player = nil,
    enemyManager = nil,
    stateMachine = StateMachine.new()
}
GameManager.__index = GameManager

function GameManager:defineState()
    self.stateMachine:insert("playerTurn", function()

    end)

    self.stateMachine:insert("enemiesTurn", function()

    end)
end

function GameManager.new()
    local self = setmetatable({}, GameManager)
    self.player = nil
    self.enemyManager = nil
    return self
end

function GameManager:Start(player, enemyManager)
    self.player = player
    self.enemyManager = enemyManager
end

function GameManager:Update()
end

function GameManager:Destroy()
end

return GameManager
