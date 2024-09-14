local json = require "assets.Scripting.json"

local TiledMap = {}
TiledMap.__index = TiledMap

local tileMap = {}
local mapJson = nil

local saveEntity = {}
local saveMap = {}

local tileSize = {
    x=0,
    y=0
}

function displayTable(tbl)
    for key, value in pairs(tbl) do
        print(key, value)
    end
end

function TiledMap.new()
    local self = setmetatable({}, TiledMap)
    return self
end

function TiledMap.load(mapPath)
    for k,v in pairs(tileMap) do
        v:destroy()
    end

    mapContent = Utils.readFile(mapPath, true)
    if mapContent == nil or mapContent:len() <= 0 then
        print("Error: Map not found")
        return false
    end
    mapJson = json.decode(mapContent)
    TiledMap.loadTileSet()
    TiledMap.loadTileMap()
    return true
end

function TiledMap.loadTileSet()
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
                ResourceManager.addTile(tostring(id), imagePath, x, y, tileSize.x, tileSize.y)
                id = id + 1
            end
        end
    end
    return true
end

function TiledMap.loadTileMap()
    local layers = mapJson["layers"]

    layerIndex = 0
    for x, layer in pairs(layers) do
        height = layer["height"]
        width = layer["width"]
        x = layer["x"]
        y = layer["y"]
        cells = layer["data"]

        tilemap = TileMap.new(x, y, width * tileSize.x,
                height * tileSize.y, tileSize.x, tileSize.y)
        index = 0
        posX = 0.0
        posY = 0.0

        for i, cell in pairs(cells) do
            cellId = cell

            if index >= height then
                index = 0
                posY = 0
                posX = posX + tileSize.y
            end
            if cellId > 0 then
                texture = ResourceManager.getTexture(tostring(cellId))
                e = Entity.new()

                e:getLayer():set(layerIndex)
                transform = e:addTransform2D(posY, posX)
                e:addSprite(texture)
                if layerIndex >= 1 then
                    e:addBoxCollider(transform.position,
                            Vector2f.new(tileSize.x, tileSize.y)):setType(BoxColliderType.DYNAMIC)
                end
                tilemap:emplaceEntity(e)
                table.insert(saveEntity, e)
            end
            posY = posY + tileSize.x
            index = index + 1
        end
        table.insert(saveMap, tilemap)
        tilemap:render()
        layerIndex = layerIndex + 3
    end
end

function TiledMap.hide()
    for k, v in pairs(tileMap) do
        v:hide()
    end
end

function TiledMap.render()
    for k, v in pairs(tileMap) do
        v:render()
    end
end

function TiledMap.drawBox()
    for k, e in pairs(saveEntity) do
        collider = e:getBoxCollider()
        if collider ~= nil then
            Core:draw(collider)
        end
    end
end

function TiledMap.destroy()
    for k,v in pairs(saveMap) do
        v:destroy()
    end

    for k,v in pairs(saveEntity) do
        v:destroy()
    end
end

return TiledMap