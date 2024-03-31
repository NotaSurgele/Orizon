local Player = require 'assets.Scripting.Player'

local player = nil

function Start()

end

function Update()
    if player == nil then
        player = Player.new()
        player:Start()
    end
    player:Update()
end

function Destroy()
    player:Destroy()
end