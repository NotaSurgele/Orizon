-- Import
local TiledMap = require 'assets.Scripting.TiledMap'
local Player = require 'assets.Scripting.Player'

local tiledMap = TiledMap.new()
local player = Player.new()

function Start()
    tiledMap.load("./assets/map_test.tmj")
    player:Start()
end

function Update()
    player:Update()
end

function Destroy()
end
