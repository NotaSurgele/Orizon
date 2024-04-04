local Player = require 'assets.Scripting.Player'
local EnemyManager = require 'assets.Scripting.EnemyManager'
GlobalVariable = require 'assets.Scripting.Global'


local player = Player.new()
GlobalVariable.enemyManager = EnemyManager.new()

function Start()
    player:Start()
    GlobalVariable.enemyManager:Start()
end

function Update()
    player:Update()
    GlobalVariable.enemyManager:Update()
end

function Destroy()
    player:Destroy()
    GlobalVariable.enemyManager:Destroy()
end
