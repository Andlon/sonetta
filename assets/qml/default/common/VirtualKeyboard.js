var rows, cols

// gridItems is a two-dimensional array mapping row, col combinations to
// items

var gridItems

function updateModelTable() {
    var count = keys.count
    var maxRow = 0
    var maxCol = 0

    for (var i = 0; i < count; ++i)
    {
        var row = keys.get(i).row
        if (row > maxRow)
            maxRow = row

        var col = keys.get(i).col
        if (col > maxCol)
            maxCol = col
    }

    rows = maxRow + 1
    cols = maxCol + 1

    // Create two-dimensional array with proper dimensions
    gridItems = new Array(rows)
    for (var j = 0; j < rows; ++j)
        gridItems[j] = new Array(cols)

    // Loop through items again and assign items to gridItems
    for (i = 0; i < count; ++i)
    {
        var element = keys.get(i)
        row = element.row
        col = element.col
        var rowspan = element.rowspan ? element.rowspan : 1
        var colspan = element.colspan ? element.colspan : 1

        for (var r = row; r < row + rowspan; ++r)
        {
            for (var c = col; c < col + colspan; ++c)
            {
                gridItems[r][c] = gridRepeater.itemAt(i)
            }
        }
    }
}

function moveUp()
{
    // Note: Return values indicate whether or not we reacted, i.e.
    // if no action was taken, we return false
    var newRow = currentRow

    // Items may have a rowspan of more than 1, find
    // next item going upwards
    var currentItem = gridItems[currentRow][currentColumn]

    do {
        --newRow;
        // Take wrapping into account
        if (newRow < 0)
        {
            if (!wrapNavigationTop)
            {
                currentRow = 0
                return false;
            }

            newRow = rows - 1;
        }
        var item = gridItems[newRow][currentColumn]
    } while (item === currentItem && newRow != currentRow)

    currentRow = newRow
    return true
}

function moveDown()
{
    var newRow = currentRow
    var currentItem = gridItems[currentRow][currentColumn]

    do {
        ++newRow;
        // Take wrapping into account
        if (newRow >= rows)
        {
            if (!wrapNavigationBottom)
            {
                currentRow = rows - 1
                return false;
            }

            newRow = 0;
        }
        var item = gridItems[newRow][currentColumn]
    } while (item === currentItem && newRow != currentRow)

    currentRow = newRow
    return true
}

function moveLeft()
{
    var newCol = currentColumn
    var currentItem = gridItems[currentRow][currentColumn]

    do {
        --newCol;
        // Take wrapping into account
        if (newCol < 0)
        {
            if (!wrapNavigationLeft)
            {
                currentColumn = 0
                return false;
            }

            newCol = cols - 1;
        }
        var item = gridItems[currentRow][newCol]
    } while (item === currentItem && newCol != currentColumn)

    currentColumn = newCol
    return true;
}

function moveRight()
{
    var newCol = currentColumn
    var currentItem = gridItems[currentRow][currentColumn]

    do {
        ++newCol;
        // Take wrapping into account
        if (newCol >= cols)
        {
            if (!wrapNavigationRight)
            {
                currentColumn = cols - 1
                return false;
            }

            newCol = 0;
        }
        var item = gridItems[currentRow][newCol]
    } while (item === currentItem && newCol != currentColumn)

    currentColumn = newCol
    return true
}

function updateFocusItem()
{
    var item = gridItems[currentRow][currentColumn]

    if (item)
    {
        item.focus = true
    }
}

function handleKeypress()
{
    var item = gridItems[currentRow][currentColumn]

    if (item)
    {
        // Get index in model, simplest through Repeater-ListModel correlation
        var index = findIndex(item)
        var element = keys.get(index)
        // Retrieve action
        var action = element.action

        if (action)
        {
            handleAction(action)
        }
        else
        {
            var c
            if (keystate == "upper")
                c = element.upper
            else if (keystate == "lower")
                c = element.lower
            else if (keystate == "symbol")
                c = element.symbol

            root.character(c)
        }
    }
}

function handleAction(action)
{
    if (action === "shift")
    {
        if (isLowercase)
            keystate = "upper"
        else
            keystate = "lower"
    }
    else if (action === "togglesymbols")
    {
        if (keystate === "upper" || keystate === "lower")
            keystate = "symbol"
        else
            keystate = isLowercase ? "lower" : "upper"
    }
    else if (action === "space")
        root.character(" ")
    else if (action === "return")
        root.enter()
    else if (action === "prev")
        root.prev()
    else if (action === "next")
        root.next()
    else if (action === "backspace")
        root.backspace()
}

function getSize()
{
    return Qt.size(root.cellSize.width * cols, root.cellSize.height * rows)
}

function findIndex(item)
{
    for (var i = 0; i < gridRepeater.count; ++i)
    {
        var repeaterItem = gridRepeater.itemAt(i)
        if (repeaterItem === item)
            return i
    }

    return -1
}

function getIconPath(action)
{
    if (action === undefined)
        return ""

    // Deal with the keys which have context-independent icons
    if (action === "space")
        return root.spaceImage
    else if (action === "backspace")
        return root.backspaceImage
    else if (action === "prev")
        return root.prevImage
    else if (action === "next")
        return root.nextImage
    else if (action === "return")
        return root.returnImage

    // Deal with togglesymbols and shift, which have context-dependent icons
    if (root.keystate === "upper")
    {
        if (action === "shift")
            return root.shiftDownImage
        else if (action === "togglesymbols")
            return root.symbolsImage
    }
    else if (root.keystate === "lower")
    {
        if (action === "shift")
            return root.shiftUpImage
        else if (action === "togglesymbols")
            return root.symbolsImage
    }
    else if (root.keystate === "symbol")
    {
        if (action === "shift" && root.isLowercase)
            return root.shiftUpImage
        else if (action === "shift")
            return root.shiftDownImage
        else if (action === "togglesymbols")
            return root.symbolsImage
    }
}

function getKeyText(upper, lower, symbol)
{
    switch (root.keystate)
    {
    case "upper":
        return upper
    case "lower":
        return lower
    case "symbol":
        return symbol
    default:
        return ""
    }
}
