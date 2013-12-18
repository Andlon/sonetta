pragma Singleton
import QtQuick 2.2

Item {
    id: root
    readonly property alias page: m.page
    readonly property alias parameters: m.parameters
    readonly property alias count: m.count

    signal enteringPage(var page, var parameters)
    signal leavingPage(var page)

    signal transition(var page, var parameters)

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

        transition(page, parameters)
    }

    function pop()
    {
        if (count > 1)
        {
            var state = m.history.pop()

            if (root.page !== state.page)
            {
                leavingPage(root.page, root.parameters)
                enteringPage(state.page, state.parameters)
                m.page = state.page
            }

            m.parameters = state.parameters
            m.updateCount()

            transition(page, parameters)
        }
    }

    function reset(page, parameters)
    {
        if (page !== root.page)
        {
            leavingPage(root.page)
            enteringPage(page, parameters)
        }

        m.history = [ ]

        m.page = page
        m.parameters = parameters

        m.updateCount()

        transition(page, parameters)
    }

    function update(page, parameters)
    {
        console.assert(page === root.page)
        m.parameters = parameters
    }

//    function printState(page, parameters)
//    {
//        console.log("Page: " + page)
//        console.log("Parameters: " + JSON.stringify(parameters, null, 4))
//    }

}
