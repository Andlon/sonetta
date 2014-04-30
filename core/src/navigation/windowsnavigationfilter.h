#pragma once

#include <QAbstractNativeEventFilter>
#include "navigation.h"

#ifdef Q_OS_WIN32
#define NOMINMAX
#include <Windows.h>
#endif

class WindowsNavigationFilter : public QAbstractNativeEventFilter {
public:
    bool nativeEventFilter(const QByteArray &eventType, void *message, long *)
    {
        Q_UNUSED(eventType);
        Q_UNUSED(message);
#ifdef Q_OS_WIN32
        if (eventType == "windows_generic_MSG")
        {
            MSG * msg = static_cast<MSG *>(message);

            if (msg->hwnd == NULL)
                return false;

            if (msg->message == WM_APPCOMMAND)
            {
                LPARAM cmd  = GET_APPCOMMAND_LPARAM(msg->lParam);
                //            LPARAM uDevice = GET_DEVICE_LPARAM(msg->lParam);
                //            LPARAM dwKeys = GET_KEYSTATE_LPARAM(msg->lParam);

                Navigation::Button button = Navigation::Undefined;

                switch (cmd)
                {
                case APPCOMMAND_MEDIA_PLAY:
                    button = Navigation::Play;
                    break;
                case APPCOMMAND_MEDIA_PAUSE:
                    button = Navigation::Pause;
                    break;
                case APPCOMMAND_MEDIA_PLAY_PAUSE:
                    button = Navigation::PlayPause;
                    break;
                case APPCOMMAND_MEDIA_STOP:
                    button = Navigation::Stop;
                    break;
                case APPCOMMAND_MEDIA_FAST_FORWARD:
                    button = Navigation::FastForward;
                    break;
                case APPCOMMAND_MEDIA_REWIND:
                    button = Navigation::Rewind;
                    break;
                case APPCOMMAND_MEDIA_NEXTTRACK:
                    button = Navigation::Next;
                    break;
                case APPCOMMAND_MEDIA_PREVIOUSTRACK:
                    button = Navigation::Previous;
                    break;
                case APPCOMMAND_MEDIA_RECORD:
                    button = Navigation::Record;
                    break;
                case APPCOMMAND_MEDIA_CHANNEL_DOWN:
                    button = Navigation::ChannelDown;
                    break;
                case APPCOMMAND_MEDIA_CHANNEL_UP:
                    button = Navigation::ChannelUp;
                    break;
                case APPCOMMAND_VOLUME_DOWN:
                    button = Navigation::VolumeDown;
                    break;
                case APPCOMMAND_VOLUME_UP:
                    button = Navigation::VolumeUp;
                    break;
                case APPCOMMAND_VOLUME_MUTE:
                    button = Navigation::Mute;
                    break;
//                case APPCOMMAND_BROWSER_BACKWARD: // Seems to come in addition to VK_BACKSPACE, ignore for now?
//                    button = Navigation::Back;
//                    break;

                default:
                    return false;
                }

                // No autorepeat detection for WM_APPCOMMAND?
                Navigation::dispatchNavigationEvent(button, false);
                return true;
            }
        }

#endif
        return false;
    }
};
