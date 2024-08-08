-- Import
local Player = require 'assets.Scripting.Player'
local EnemyManager = require 'assets.Scripting.EnemyManager'
local GameManager = require 'assets.Scripting.GameManager'
local TiledMap = require 'assets.Scripting.TiledMap'
GlobalVariable = require 'assets.Scripting.Global'

local player = Player.new()
local enemyManager = EnemyManager.new()
local gameManager = GameManager.new()
local tiledMap = TiledMap.new()

local transition = {
    sprite = nil,
    color = Color.Black,
    ended = false
}

transition.play = function ()
    if transition.ended == false then
        if transition.color.a <= 0 then
            transition.color.a = 255
            transition.sprite:setColor(transition.color)
            transition.ended = true
            return 0
        end
        transition.color.a = transition.color.a - 80 * deltaTime
        transition.sprite:setColor(transition.color)
        Core:draw(transition.sprite)
        return 1
    end
    return 0
end

function Start()
    local texture = ResourceManager.getTexture("black_rect")
    transition.sprite = Sprite.new(texture)
    transition.sprite:setColor(transition.color)
    transition.sprite:setScale(10, 10)
    transition.sprite:setPosition(0, 0)

    player:Start(enemyManager)
    --tiledMap.load("./assets/map_test.tmj")
end

function Update()
    transition.play()
end

function Destroy()
    player:Destroy()
    enemyManager:Destroy()
end
