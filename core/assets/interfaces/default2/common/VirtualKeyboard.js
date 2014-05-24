
// Two-dimensional array mapping to items
var gridItems

function isUndefined(variable)
{
    return (typeof(variable) === 'undefined')
}

function initialize() {
    updateGridsize();
    createKeymap();

    currentRow = 0
    currentColumn = 0
}

function updateGridsize()
{
    m.rowCount = 0
    m.columnCount = 0
    var count = keys.count

    for (var i = 0; i < count; ++i)
    {
        var element = keys.get(i)

        var rowspan = element.rowspan === 0 || isUndefined(element.rowspan)
                ? 1 : element.rowspan
        var colspan = element.colspan === 0 || isUndefined(element.colspan)
                ? 1 : element.colspan

        // Update grid sizes if necessary
        if (element.row + rowspan > m.rowCount)
            m.rowCount = element.row + rowspan

        if (element.col + colspan > m.columnCount)
            m.columnCount = element.col + colspan

    }
}

function createKeymap()
{
    // Create rowCount x columnCount two-dimensional array
    gridItems = new Array(m.rowCount)
    for (var i = 0; i < m.rowCount; ++i)
        gridItems[i] = new Array(m.columnCount)

    var count = keys.count
    for (i = 0; i < count; ++i)
    {
        var element = keys.get(i)
        console.assert(!isUndefined(element.row))
        console.assert(!isUndefined(element.col))

        var rowspan = element.rowspan === 0 || isUndefined(element.rowspan)
                ? 1 : element.rowspan
        var colspan = element.colspan === 0 || isUndefined(element.colspan)
                ? 1 : element.colspan

        for (var c = 0; c < colspan; ++c)
        {
            for (var r = 0; r < rowspan; ++r)
            {
                var row = element.row + r
                var col = element.col + c
                gridItems[row][col] = gridRepeater.itemAt(i)
            }
        }
    }
}

function updateFocus()
{
    var row = currentRow
    var col = currentColumn

    if (validatePosition(row, col))
    {
        var item = getItem(row, col);
        item.forceActiveFocus()
    }
}

function getItem(row, col)
{
    if (validatePosition(row, col))
        return gridItems ? gridItems[row][col] : null
    return null
}

function validatePosition(row, col)
{
    return row >= 0 && row < m.rowCount && col >= 0 && col < m.columnCount;
}

// TODO: moveColumn and moveRow are practically identical. Refactor to use common subset
function moveColumn(delta)
{
    var newColumn = currentColumn + delta
    if (validatePosition(currentRow, newColumn))
    {
        var initialColumn = currentColumn
        var initialItem = currentItem

        var item = getItem(currentRow, newColumn)
        while (item === initialItem && newColumn !== initialColumn
               && newColumn >= 0 && newColumn < m.columnCount)
        {
            newColumn += delta >= 0 ? 1 : -1
            item = getItem(currentRow, newColumn)
        }

        return move(currentRow, newColumn)
    }
    return false
}

function moveRow(delta)
{
    var newRow = currentRow + delta
    if (validatePosition(currentRow, newRow))
    {
        var initialRow = currentRow

        var item = getItem(newRow, currentColumn)
        while (item === currentItem && newRow !== initialRow
               && newRow >= 0 && newRow < m.rowCount)
        {
            newRow += delta >= 0 ? 1 : -1
            item = getItem(newRow, currentColumn)
        }

        return move(newRow, currentColumn)
    }
    return false
}

function move(newRow, newCol)
{
    if (validatePosition(newRow, newCol))
    {
        currentColumn = newCol
        currentRow = newRow
        updateFocus()
        return true
    }
    return false
}
