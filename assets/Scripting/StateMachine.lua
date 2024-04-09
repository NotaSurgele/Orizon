StateMachine = {}
StateMachine.__index = StateMachine

function StateMachine.new()
    local self = setmetatable({}, StateMachine)

    self.states = {}
    self.currentState = nil
    return self
end

function StateMachine:insert(name, callback)
    self.states[name] = callback
end

function StateMachine:play(name, ...)
    local state = self.states[name]

    if state then
        self.currentState = name
        state(...)
        return
    end
    print("State", name, "Does not exist !")
end

function StateMachine:getCurrentState()
    return self.currentState
end

return StateMachine