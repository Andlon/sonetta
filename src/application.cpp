#include "application.h"

#include <QtQml>
#include <QScreen>
#include <QKeyEvent>
#include <QQuickView>
#include <QStandardPaths>
#include <QDebug>
#include <QDir>

#include "quicktrackinfo.h"
//#include "quickartistsynopsis.h"
#include "quicksearch.h"
#include "imageprovider.h"

#include "models/albumlistmodel.h"
#include "models/albumbrowsemodel.h"

#include "quick/enums.h"
#include "quick/models.h"

#include "../appkey.c"

namespace sp = Spotinetta;

namespace Sonetta {

Application::Application(int &argc, char **argv)
    :   QGuiApplication(argc, argv), m_view(0), m_nav(0)
{
    sp::SessionConfig config;
    config.applicationKey = sp::ApplicationKey(g_appkey, g_appkey_size);
    config.userAgent = "Sonetta";

    config.settingsLocation = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/libspotify";
    config.cacheLocation = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/libspotify";

    // Create directories if they don't exist
    QDir dir;
    dir.mkpath(config.settingsLocation);
    dir.mkpath(config.cacheLocation);

    m_session = new sp::Session(config, this);
    m_player = new Player(m_session, this);
    m_ui = new UIStateCoordinator(this);
}

Application::~Application()
{
    delete m_view;
}

int Application::run()
{
    qmlRegisterType<Navigation>("Sonetta", 0, 1, "Navigation");
    qmlRegisterType<NavigationAttached>();
    qmlRegisterUncreatableType<QuickNavEvent>("Sonetta", 0, 1, "NavEvent", "Cannot instantiate navigation event. ");

    qmlRegisterType<QuickPlaylistContainerModel>("Sonetta", 0, 1, "PlaylistContainerModel");

    qmlRegisterUncreatableType<Spotinetta::Session>("Sonetta", 0, 1, "Session", "Cannot instantiate Session.");

    // Enums
    qmlRegisterUncreatableType<AlbumEnums>("Sonetta", 0, 1, "Album", "Cannot instantiate Album.");
    qmlRegisterUncreatableType<TrackEnums>("Sonetta", 0, 1, "Track", "Cannot instantiate Track.");

    if (m_session->error() == sp::Error::Ok)
    {
        m_nav = new Navigation(this);
        m_view = new QQuickView;

        QString applicationDir = applicationDirPath();

        ImageProvider * provider = new ImageProvider(m_session, this);

        m_view->engine()->addImageProvider(QLatin1String("sp"), provider);
        m_view->engine()->rootContext()->setContextProperty("player", m_player);
        m_view->engine()->rootContext()->setContextProperty("ui", m_ui);
        m_view->engine()->rootContext()->setContextProperty("session", m_session);
        m_view->engine()->addImportPath(applicationDir + QStringLiteral("/qml/modules"));
        m_view->setSource(QUrl::fromLocalFile(applicationDir + QStringLiteral("/qml/rework2/main.qml")));
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
    else
    {
        const QByteArray msg = QByteArray("Session creation failed. Error: ") + sp::errorMessage(m_session->error()).toUtf8();
        qFatal(msg.constData());
        return 1;
    }
}

Application * Application::instance()
{
    QCoreApplication * inst = QCoreApplication::instance();
    return inst == nullptr ? nullptr : static_cast<Application *>(inst);
}

sp::Session * Application::session() const
{
    return m_session;
}

bool Application::notify(QObject *receiver, QEvent *event)
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

}
