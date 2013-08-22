#pragma once

#include "definitions.h"

namespace Spotinetta {

enum class Error {
    Ok = SP_ERROR_OK,
    BadApiVersion = SP_ERROR_BAD_API_VERSION,
    ApiInitializationFailed = SP_ERROR_API_INITIALIZATION_FAILED,
    TrackNotPlayable = SP_ERROR_TRACK_NOT_PLAYABLE,
    BadApplicationKey = SP_ERROR_BAD_APPLICATION_KEY,
    BadUsernameOrPassword = SP_ERROR_BAD_USERNAME_OR_PASSWORD,
    UserBanned = SP_ERROR_USER_BANNED,
    UnableToContactServer = SP_ERROR_UNABLE_TO_CONTACT_SERVER,
    ClientTooOld = SP_ERROR_CLIENT_TOO_OLD,
    OtherPermanent = SP_ERROR_OTHER_PERMANENT,
    BadUserAgent = SP_ERROR_BAD_USER_AGENT,
    MissingCallback = SP_ERROR_MISSING_CALLBACK,
    InvalidIndata = SP_ERROR_INVALID_INDATA,
    IndexOutOfRange = SP_ERROR_INDEX_OUT_OF_RANGE,
    UserNeedsPremium = SP_ERROR_USER_NEEDS_PREMIUM,
    OtherTransient = SP_ERROR_OTHER_TRANSIENT,
    IsLoading = SP_ERROR_IS_LOADING,
    NoStreamAvailable = SP_ERROR_NO_STREAM_AVAILABLE,
    PermissionDenied = SP_ERROR_PERMISSION_DENIED,
    InboxIsFull = SP_ERROR_INBOX_IS_FULL,
    NoCache = SP_ERROR_NO_CACHE,
    NoSuchUser = SP_ERROR_NO_SUCH_USER,
    NoCredentials = SP_ERROR_NO_CREDENTIALS,
    NetworkDisabled = SP_ERROR_NETWORK_DISABLED,
    InvalidDeviceId = SP_ERROR_INVALID_DEVICE_ID,
    CantOpenTracefile = SP_ERROR_CANT_OPEN_TRACE_FILE,
    ApplicationBanned = SP_ERROR_APPLICATION_BANNED,
    OfflineTooManyTracks = SP_ERROR_OFFLINE_TOO_MANY_TRACKS,
    OfflineDiskCache = SP_ERROR_OFFLINE_DISK_CACHE,
    OfflineExpired = SP_ERROR_OFFLINE_EXPIRED,
    OfflineNotAllowed = SP_ERROR_OFFLINE_NOT_ALLOWED,
    OfflineLicenseLost = SP_ERROR_OFFLINE_LICENSE_LOST,
    OfflineLicenseError = SP_ERROR_OFFLINE_LICENSE_ERROR,
    LastFmAuthError = SP_ERROR_LASTFM_AUTH_ERROR,
    InvalidArgument = SP_ERROR_INVALID_ARGUMENT,
    SystemFailure = SP_ERROR_SYSTEM_FAILURE
};

inline QString errorMessage(Error error)
{
    return QString::fromUtf8(sp_error_message(static_cast<sp_error>(error)));
}

}
