local Player = require 'assets.Scripting.Player'
local EnemyManager = require 'assets.Scripting.EnemyManager'

local player = Player.new()
local enemyManager = EnemyManager.new()

function Start()
    player:Start()
    enemyManager:Start()
end

function Update()
    player:Update()
    enemyManager:Update()
end

function Destroy()
    --player:Destroy()
    --enemyManager:Destroy()
end