local tiledMap = require "assets.Scripting.TiledMap"
local save = {}

function Start()
    --tiledMap.load("../assets/map_test.tmj")
end

function Update()
    --tiledMap.drawBox()
end


function Destroy()
    tiledMap.destroy()
end
