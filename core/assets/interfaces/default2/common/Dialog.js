.pragma library

var _rootItem = null
var _contentItem = null

function setRootItem(item) { _rootItem = item }
function setContentItem(item) { _contentItem = item }

function rootItem() { return _rootItem }
function contentItem() { return _contentItem }
