tiledMap = {}

local tileMap = {}
local json = require "json"
local mapJson = nil

local tileSize = {
    x=0,
    y=0
}

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
    mapJson = json.decode(mapContent)
    tiledMap.loadTileSet()
    tiledMap.loadTileMap()
    return true
end

function tiledMap.loadTileSet()
    local tilesets = mapJson["tilesets"]

    for k, tileset in pairs(tilesets) do
        local source = tileset["source"]

        tilesetStr = Utils.readFile("./assets/" .. source)
        tilesetJson = json.decode(tilesetStr)

        imgHeight = tilesetJson["imageheight"]
        imgWidth = tilesetJson["imagewidth"]
        imagePath = tilesetJson["image"]
        tileSize.x = tilesetJson["tilewidth"]
        tileSize.y = tilesetJson["tileheight"]

        local id = 1
        for y=0, imgHeight - 1, tileSize.y do
            for x=0, imgWidth - 1, tileSize.x do
                ResourceManager:R_ADD_TILE(tostring(id), imagePath, x, y, tileSize.x, tileSize.y)
                id = id + 1
            end
        end
    end
    return true
end

function tiledMap.loadTileMap()
    local layers = mapJson["layers"]

    draw = 10

    for x, layer in pairs(layers) do
        height = layer["height"]
        width = layer["width"]
        x = layer["x"]
        y = layer["y"]
        cells = layer["data"]

        tilemap = TileMap.new(x, y, width * tileSize.x,
                height * tileSize.y, tileSize.x, tileSize.y)
        print("tilemap addre", tilemap)
        index = 0
        posX = 0.0
        posY = 0.0

        for i, cell in pairs(cells) do
            cellId = cell

            if index >= height then
                index = 0
                posY = 0
                posX = posX + tileSize.x
            end

            if cellId > 0 then
                texture = ResourceManager:R_GET_RESSOURCE(tostring(cellId))
                e = Entity.new()

                e:getComponentLayer():set(draw)
                transform = e:addComponentTransform2D(posY, posX)
                e:addComponentSprite(texture)
                e:addComponentBoxCollider(transform.position,
                        Vector2f.new(tileSize.x, tileSize.y)):setType(BoxCollider.STATIC)
                tilemap:emplaceEntity(e)
            end
            poxY = posY + tileSize.x
            index = index + 1
        end
        tilemap:render()
        draw = draw + 2
    end
end

function tiledMap.hide()
    for k, v in pairs(tileMap) do
        v:hide()
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