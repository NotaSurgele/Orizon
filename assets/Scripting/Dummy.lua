Dummy = {}
Dummy.__index = Dummy

function Dummy.new()
    local self = setmetatable({}, Dummy)
    return self
end

function Dummy:Start()
end

function Dummy:Update()
end

function Dummy:Destroy()
end

return Dummy
