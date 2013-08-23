#include "sonataapplication.h"
#include "sonataquickview.h"

#include <QtQml>
#include <QScreen>
#include <QKeyEvent>

#include "playlistcontainermodel.h"
#include "playlistmodel.h"
#include "quicktrackinfo.h"
#include "quickartistsynopsis.h"
#include "quicklinker.h"
#include "quicksearch.h"
#include "spotifyimageprovider.h"

#include "models/albumlistmodel.h"
#include "models/albumbrowsemodel.h"

SonataApplication::SonataApplication(int &argc, char **argv)
    :   QGuiApplication(argc, argv), m_view(0), m_nav(0)
{
    m_spotify = new SpotifySession(g_appkey, g_appkey_size, this);
    m_player = new SonataPlayer(m_spotify, this);
    m_ui = new UiStateCoordinator(this);
}

SonataApplication::~SonataApplication()
{
    delete m_view;
}

int SonataApplication::start()
{
    qmlRegisterUncreatableType<SonataPlayer>("sonata", 0, 1, "Sonata", "Can not instantiate Player from QML");
    qmlRegisterUncreatableType<UiStateCoordinator>("sonata", 0, 1, "UiStateManagement", "Can not instantiate UiStateManagement from QML.");
    qmlRegisterUncreatableType<SpotifySession>("sonata", 0, 1, "Spotify", "Can not instantiate Sonata Spotify from QML.");

    qmlRegisterType<Navigation>("sonata", 0, 1, "Navigation");
    qmlRegisterType<NavigationAttached>();
    qmlRegisterUncreatableType<QuickNavEvent>("sonata", 0, 1, "NavEvent", "Cannot instantiate navigation event. ");

    qmlRegisterType<PlaylistContainerModel>("sonata", 0, 1, "PlaylistContainerModel");
    qmlRegisterType<PlaylistModel>("sonata", 0, 1, "PlaylistModel");
    qmlRegisterType<AlbumListModel>("sonata", 0, 1, "AlbumListModel");
    qmlRegisterType<AlbumBrowseModel>("sonata", 0, 1, "AlbumBrowseModel");
    qmlRegisterType<QuickTrackInfo>("sonata", 0, 1, "TrackInfo");
    qmlRegisterType<QuickArtistSynopsis>("sonata", 0, 1, "ArtistSynopsis");
    qmlRegisterType<QuickSearch>("sonata", 0, 1, "SpotifySearch");

    qmlRegisterSingletonType<QuickLinker>("sonata", 0, 1, "Linker", QuickLinkerSingletonProvider);

    if (!m_spotify->createSession())
    {
        qFatal("Failed to create spotify session. Aborting...");
        return 1;
    }

    m_nav = new Navigation(this);
    m_view = new SonataQuickView;

    QString applicationDir = applicationDirPath();

    SpotifyImageProvider * provider = new SpotifyImageProvider(this);

    m_view->engine()->addImageProvider(QLatin1String("sp"), provider);
    m_view->engine()->rootContext()->setContextProperty("player", m_player);
    m_view->engine()->rootContext()->setContextProperty("spotify", m_spotify);
    m_view->engine()->rootContext()->setContextProperty("ui", m_ui);
    m_view->engine()->addImportPath(applicationDir + QStringLiteral("/qml/modules"));
    m_view->setSource(QUrl::fromLocalFile(applicationDir + QStringLiteral("/qml/rework/main.qml")));
    m_view->setResizeMode(QQuickView::SizeRootObjectToView);

    // Center view
    QScreen * screen = m_view->screen();
    QPoint screenCenter = screen->availableGeometry().center();
    QPoint windowCenter = m_view->geometry().center();
    m_view->setPosition(screenCenter - windowCenter);
    m_view->showFullScreen();

    // Start event loop
    return exec();
}

bool SonataApplication::notify(QObject *receiver, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent * keyEvent = static_cast<QKeyEvent *>(event);

        // Ignore Alt + Enter
        if (!(keyEvent->key() == Qt::Key_Return && keyEvent->modifiers() & Qt::AltModifier))
        {
            if (Navigation::dispatchKeyEvent(keyEvent))
                return true;
        }
    }

    return QGuiApplication::notify(receiver, event);
}
