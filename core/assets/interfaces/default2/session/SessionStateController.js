var states = [ initialState() ]

function initialState() {
    return create("playlists", {})
}

function create(name, parameters) {
    return { name: name, parameters: parameters }
}

function push(state) {
    states.push(state)
    root.statePushed(state.name, state.parameters)
}

function pop() {
    if (states.length > 1) {
        states.pop()
        var state = states[states.length - 1]
        root.statePopped(state.name, state.parameters)
    }
}

function reset(state) {
    states = []
    states.push(state)
    root.stateReset(state.name, state.parameters)
}

function update(parameters) {
    if (states.length > 0) {
        var state = states.pop()
        state.parameters = parameters
        states.push()
    }
}
