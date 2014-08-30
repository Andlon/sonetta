.pragma library

function formatTrackDuration(msecs)
{
    var totalSecs = Math.round(msecs / 1000);
    var mins = Math.floor(totalSecs / 60);
    var secs = totalSecs % 60;

    var padding = mins < 10 ? " " : ""

    return padding + mins + ":" + (secs < 10 ? "0" + secs : secs)
}

function formatTotalCollectionDuration(ms) {
    var totalSecs = Math.round(ms / 1000)
    var hours = Math.floor(totalSecs / 3600)
    var mins = Math.round((totalSecs - hours * 3600) / 60)

    var hourNoun = "h"
    var minuteNoun = "min"

    var composed = ""

    if (hours > 0) {
        composed += hours + " " + hourNoun
    }

    if (mins > 0) {
        if (hours > 0)
            composed += ", "

        composed += mins + " " + minuteNoun
    }

    return composed
}
