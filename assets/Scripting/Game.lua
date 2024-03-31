local Player = require 'assets.Scripting.Player'

local player = Player.new()
local enemyManager = nil

function Start()
    player:Start()
end

function Update()
    player:Update()
end

function Destroy()
    player:Destroy()
end