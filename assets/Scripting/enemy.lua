player = nil
transform = nil

playerLayer = nil
speed = 100


function Start()
    player = System.getEntity("player")

    if player ~= nil then
        transform = player:getComponentTransform2D()
        playerLayer = player:getComponentLayer()
    end
end

function Update()
	selfTransform = _self:getComponentTransform2D()
    selfLayerValue = _self:getComponentLayer():value()

    if transform ~= nil then
		y = transform.position.y
        dirY = y - selfTransform.position.y

        if dirY <= 0 then
            playerLayer:set(selfLayerValue - 1)
        elseif dirY >= 0 then
            playerLayer:set(selfLayerValue + 1)
        end


    end

end

function Destroy()

end
