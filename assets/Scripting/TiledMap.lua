tiledMap = {}

local tileMap = {}
local json = nil
local mapJson = nil

function displayTable(tbl)
    for key, value in pairs(tbl) do
        print(key, value)
    end
end

function tiledMap.load(mapPath)
    for k,v in pairs(tileMap) do
        v:destroy()
    end

    mapContent = Utils.readFile(mapPath, true)
    if mapContent == nil or mapContent:len() <= 0 then
        print("Error: Map not found")
        return false
    end
    json = require "json"
    mapJson = json.decode(mapContent)
    tiledMap.loadTileSet()
    return true
end

function tiledMap.loadTileSet()
    local tilesets = mapJson["tilesets"]

    for k, tileset in pairs(tilesets) do
        local source = tileset["source"]

        ResourceManager:R_ADD_TILE("hello", 1, 2, 3, 5)
    end
end

function tiledMap.render()
    for k, v in pairs(tileMap) do
        v:render()
    end
end

function tiledMap.destroy()
    for k,v in pairs(tileMap) do
        tileMap[k]:destroy()
    end
end

return tiledMap