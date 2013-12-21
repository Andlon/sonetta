pragma Singleton
import QtQuick 2.2

Item {
    id: root
    readonly property alias page: m.page
    readonly property alias parameters: m.parameters
    readonly property alias count: m.count

    signal enteringPage(var page, var parameters)
    signal leavingPage(var page)

    signal transition(var prev, var current, var type)

    QtObject {
        id: m

        property string page: "test"
        property var parameters: { return {} }
        property var history: { return [] }
        property int count: 0

        function updateCount() { count = m.history.length + 1 }
        function createState(page, parameters) { return { page: page, parameters: parameters } }
    }

    function push(page, parameters)
    {
        if (page !== root.page)
        {
            leavingPage(root.page)
            enteringPage(page, parameters)
        }

        var state = m.createState(root.page, root.parameters)
        m.history.push(state)

        m.page = page
        m.parameters = parameters
        m.updateCount()

        transition(state, { page: page, parameters: parameters }, "push")
    }

    function pop()
    {
        if (count > 1)
        {
            var state = m.history.pop()
            var prev = { page: root.page, parameters: root.parameters }

            if (root.page !== state.page)
            {
                leavingPage(root.page, root.parameters)
                enteringPage(state.page, state.parameters)
                m.page = state.page
            }

            m.parameters = state.parameters
            m.updateCount()

            transition(prev, state, "pop")
        }
    }

    function reset(page, parameters)
    {
        var prev = { page: root.page, parameters: parameters }
        if (page !== root.page)
        {
            leavingPage(root.page)
            enteringPage(page, parameters)
        }

        m.history = [ ]
        m.page = page
        m.parameters = parameters

        m.updateCount()

        transition(prev, { page: page, parameters: parameters }, "reset")
    }

    function update(page, parameters)
    {
        console.assert(page === root.page)
        m.parameters = parameters
    }
}
