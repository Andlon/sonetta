#include "navigationnativeeventfilter.h"

#ifdef Q_OS_WIN32
#define NOMINMAX
#include <Windows.h>
#endif

#include "navigation.h"

NavigationNativeEventFilter::NavigationNativeEventFilter() :
    QAbstractNativeEventFilter()
{
#ifdef Q_OS_WIN32
    // Register for raw input from Microsoft eHome Infrared Receiver

    //    RAWINPUTDEVICE device;
    //    device.usUsagePage = 0x01;
    //    //    device.

    //    PRAWINPUTDEVICELIST list;
    //    UINT nDevices = NULL;
    //    PRAWINPUTDEVICELIST pRawInputDeviceList;

    //    UINT success = GetRawInputDeviceList(NULL, &nDevices, sizeof(RAWINPUTDEVICELIST));

    //    if (success == 0)
    //    {
    //        pRawInputDeviceList = (PRAWINPUTDEVICELIST) malloc(sizeof(RAWINPUTDEVICELIST) * nDevices);
    //        if (GetRawInputDeviceList(pRawInputDeviceList, &nDevices, sizeof(RAWINPUTDEVICELIST)) != -1)
    //        {
    //            for (int i = 0; i < nDevices; ++i)
    //            {
    //                RAWINPUTDEVICELIST device = *(pRawInputDeviceList + i);
    //                if (device.dwType == RIM_TYPEHID)
    //                {
    //                    qDebug() << "Hid device.";
    //                    char data[25];
    //                    for (int i = 0; i < 25; ++i)
    //                        data[i] = 0;
    //                    UINT nChars;
    //                    GetRawInputDeviceInfo(device.hDevice, RIDI_DEVICENAME, (LPVOID) &data, &nChars);
    //                    qDebug() << data;
    //                }
    //            }
    //            free(pRawInputDeviceList);
    //        }
    //    }

#endif
}

bool NavigationNativeEventFilter::nativeEventFilter(const QByteArray &eventType, void *message, long * /*result*/)
{
#ifdef Q_OS_WIN32
    if (eventType == "windows_generic_MSG")
    {
        MSG * msg = static_cast<MSG *>(message);

        if (msg->hwnd == NULL)
            return false;

        //        if (msg->message == WM_KEYDOWN)
        //        {
        //            WPARAM keyCode = msg->wParam;
        //            Navigation::Key key = Navigation::Undefined;

        //            switch (keyCode)
        //            {
        //            case VK_RETURN:
        //                key = Navigation::OK;
        //                break;
        //            case VK_BACK:
        //                key = Navigation::Back;
        //                break;
        //                //            case VK_BROWSER_BACK:
        //                //                key = Navigation::Back;
        //                //                break;
        //                //            case VK_MEDIA_PLAY_PAUSE:
        //                //                key = Navigation::PlayPause;
        //                //                break;
        //                //            case VK_MEDIA_STOP:
        //                //                key = Navigation::Stop;
        //                //                break;
        //                //            case VK_MEDIA_NEXT_TRACK:
        //                //                key = Navigation::Next;
        //                //                break;
        //                //            case VK_MEDIA_PREV_TRACK:
        //                //                key = Navigation::Previous;
        //                //                break;
        //            case VK_LEFT:
        //                key = Navigation::Left;
        //                break;
        //            case VK_RIGHT:
        //                key = Navigation::Right;
        //                break;
        //            case VK_UP:
        //                key = Navigation::Up;
        //                break;
        //            case VK_DOWN:
        //                key = Navigation::Down;
        //                break;

        //            default:
        //                return false;
        //            }

        //            NavEvent event(key);
        //            Navigation::dispatchNavigationEvent(&event);
        //            if (event.isAccepted())
        //                return true;
        //        }
        if (msg->message == WM_APPCOMMAND)
        {
            LPARAM cmd  = GET_APPCOMMAND_LPARAM(msg->lParam);
            //            LPARAM uDevice = GET_DEVICE_LPARAM(msg->lParam);
            //            LPARAM dwKeys = GET_KEYSTATE_LPARAM(msg->lParam);

            Navigation::Key key = Navigation::Undefined;

            switch (cmd)
            {
            case APPCOMMAND_MEDIA_PLAY:
                key = Navigation::Play;
                break;
            case APPCOMMAND_MEDIA_PAUSE:
                key = Navigation::Pause;
                break;
            case APPCOMMAND_MEDIA_PLAY_PAUSE:
                key = Navigation::PlayPause;
                break;
            case APPCOMMAND_MEDIA_STOP:
                key = Navigation::Stop;
                break;
            case APPCOMMAND_MEDIA_FAST_FORWARD:
                key = Navigation::FastForward;
                break;
            case APPCOMMAND_MEDIA_REWIND:
                key = Navigation::Rewind;
                break;
                //            case APPCOMMAND_MEDIA_NEXTTRACK:
                //                key = Navigation::Next;
                //                break;
                //            case APPCOMMAND_MEDIA_PREVIOUSTRACK:
                //                key = Navigation::Previous;
                //                break;
            case APPCOMMAND_MEDIA_RECORD:
                key = Navigation::Record;
                break;
            case APPCOMMAND_MEDIA_CHANNEL_DOWN:
                key = Navigation::ChannelDown;
                break;
            case APPCOMMAND_MEDIA_CHANNEL_UP:
                key = Navigation::ChannelUp;
                break;
            case APPCOMMAND_VOLUME_DOWN:
                key = Navigation::VolumeDown;
                break;
            case APPCOMMAND_VOLUME_UP:
                key = Navigation::VolumeUp;
                break;
            case APPCOMMAND_VOLUME_MUTE:
                key = Navigation::Mute;
                break;
            case APPCOMMAND_BROWSER_BACKWARD: // Is this the correct one to use?
                key = Navigation::Back;
                break;

            default:
                return false;
            }

            NavEvent event(key);
            Navigation::dispatchNavigationEvent(&event);
            if (event.isAccepted())
                return true;
        }
    }
#endif

    return false;
}
