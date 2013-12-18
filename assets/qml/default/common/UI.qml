pragma singleton

import QtQuick 2.2

QtObject {
    id: root
    readonly property alias page: m.page
    readonly property alias parameters: m.parameters
    readonly property alias count: m.count

    signal enteringPage(var page, var parameters)
    signal leavingPage(var page)

    QtObject {
        id: m

        property string page: ""
        property var parameters
        property var states: { return [] }
        property int count: 0

        function updateCount() { count = m.states.length }
        function createState(page, parameters) { return { page: page, parameters: parameters } }
    }

    function push(page, parameters)
    {
        if (page !== root.page)
        {
            leavingPage(root.page)
            enteringPage(page, parameters)
        }

        var state = m.createState(page, parameters)

        m.states.push(state)
        root.page = page
        root.parameters = parameters
        m.updateCount()
    }

    function pop()
    {
        if (count > 0)
        {
            var state = m.states.pop()

            if (state.page !== page)
            {
                leavingPage(root.page, root.parameters)
                enteringPage(state.page, state.parameters)
                root.page = state.page
            }

            root.parameters = state.parameters
            m.updateCount()
        }
    }

    function reset(page, parameters)
    {
        var state = m.createState(page, parameters)
        m.states = [ state ]

        if (page !== root.page)
        {
            leavingPage(page)
            enteringPage(page, parameters)
        }

        root.page = page
        root.parameters = parameters

        m.updateCount()
    }

    function update(page, parameters)
    {
        console.assert(page === root.page)
        m.states[count - 1].parameters = parameters
        m.parameters = parameters
    }
}
