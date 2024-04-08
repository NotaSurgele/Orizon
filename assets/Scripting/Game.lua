local Player = require 'assets.Scripting.Player'
local EnemyManager = require 'assets.Scripting.EnemyManager'
local GameManager = require 'assets.Scripting.GameManager'
GlobalVariable = require 'assets.Scripting.Global'

local player = Player.new()
local enemyManager = EnemyManager.new()
local gameManager = GameManager.new()

function Start()
    enemyManager:Start()
    player:Start(enemyManager)
    gameManager:Start(player, enemyManager)
end

function Update()
    player:Update()
    enemyManager:Update()
end

function Destroy()
    player:Destroy()
    enemyManager:Destroy()
end
