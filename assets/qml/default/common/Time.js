.pragma library

function formatTime(msecs)
{
    var totalSecs = Math.round(msecs / 1000);
    var mins = Math.floor(totalSecs / 60);
    var secs = totalSecs % 60;

    var padding = mins < 10 ? " " : ""

    return padding + mins + ":" + (secs < 10 ? "0" + secs : secs)
}
