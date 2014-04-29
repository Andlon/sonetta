#include "application.h"

#include <QtQml>
#include <QScreen>
#include <QKeyEvent>
#include <QQuickView>
#include <QStandardPaths>
#include <QDebug>
#include <QDir>
#include <QFontDatabase>
#include <QFile>
#include <QWeakPointer>

#include "imageprovider.h"

#include "quick/enums.h"
#include "quick/models.h"
#include "quick/quicktrackinfo.h"
#include "quick/quickmosaicgenerator.h"
#include "quick/quickfactory.h"
#include "quick/quickartistsynopsis.h"
#include "quick/quickglobalstatemachine.h"

#include "../appkey.c"

namespace sp = Spotinetta;

namespace {
// This global is unfortunately necessary for the Qt Quick models
// to retrieve the current session
QWeakPointer<Spotinetta::Session> g_session;
}

namespace Sonetta {

Application::Application(QObject * parent)
    :   QObject(parent),
      m_view(new QQuickView),
      m_ui(new UIStateCoordinator),
      m_output(new AudioOutput),
      m_settings(new Settings),
      m_exiting(false)
{
    createSession();

    m_player.reset(new Player(m_session, m_output));
    m_search.reset(new SearchEngine(m_session.constCast<const sp::Session>(), m_settings));

    connect(m_session.data(), &sp::Session::loggedOut, this, &Application::onLogout);
    connect(m_session.data(), &sp::Session::log, [] (const QString &msg) { qDebug() << msg; });

    connect(m_settings.data(), &Settings::mouseEnabledChanged, this, &Application::updateCursor);

    m_view->installEventFilter(this);
}

Application::~Application()
{
    // Manually reset view here to make sure it's deleted
    // before any of the others (Player, SearchEngine etc.)
    m_view.reset();
}

bool Application::initialize()
{
    loadFonts();
    registerQmlTypes();

    if (m_session->error() == sp::Error::Ok)
    {
        m_navigation.initialize(m_settings->lircDelay());

        setupQuickEnvironment();
        showUi();
        return true;
    }
    else
    {
        const QByteArray msg = QByteArray("Session creation failed. Error: ") + sp::errorMessage(m_session->error()).toUtf8();
        qFatal(msg.constData());
        return false;
    }
}

void Application::onExit()
{
    if (!m_exiting)
    {
        m_exiting = true;
        if (m_session->connectionState() == sp::Session::ConnectionState::LoggedIn
                || m_session->connectionState() == sp::Session::ConnectionState::Offline)
        {
            m_session->logout();
        }
        else
        {
            // Skip session logout
            onLogout();
        }
    }
}

void Application::onLogout()
{
    if (m_exiting)
    {
        QCoreApplication::quit();
    }
}

void Application::updateCursor()
{
    if (m_view.isNull())
        return;

    if (m_settings->mouseEnabled())
    {
        m_view->unsetCursor();
    }
    else
    {
        m_view->setCursor(QCursor(Qt::BlankCursor));
    }
}

ObjectSharedPointer<sp::Session> Application::session()
{
    return ObjectSharedPointer<sp::Session>(g_session.toStrongRef());
}

bool Application::eventFilter(QObject * obj, QEvent * e)
{
    Q_UNUSED(obj);
    Q_ASSERT(obj == m_view.data());

    switch (e->type())
    {
    case QEvent::Close:
        onExit();
        return true;
    case QEvent::MouseButtonDblClick:
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
    case QEvent::MouseMove:
    case QEvent::MouseTrackingChange:
        if (!m_settings->mouseEnabled())
            return true;
    default:
        return false;
    }
}

void Application::registerQmlTypes()
{
    Navigation::registerTypes();

    qmlRegisterType<QuickPlaylistContainerModel>("Sonetta", 0, 1, "PlaylistContainerModel");
    qmlRegisterType<QuickPlaylistModel>("Sonetta", 0, 1, "PlaylistModel");
    qmlRegisterType<QuickAlbumModel>("Sonetta", 0, 1, "AlbumModel");
    qmlRegisterType<QuickTrackInfo>("Sonetta", 0, 1, "TrackInfo");
    qmlRegisterType<QuickMosaicGenerator>("Sonetta", 0, 1, "MosaicGenerator");
    qmlRegisterType<QuickArtistSynopsis>("Sonetta", 0, 1, "ArtistSynopsis");

    qmlRegisterUncreatableType<Spotinetta::Session>("Sonetta", 0, 1, "Session", "Cannot instantiate Session.");
    qmlRegisterSingletonType<QuickFactory>("Sonetta", 0, 1, "Factory", &quickFactorySingletonProvider);

    // Enums
    qmlRegisterUncreatableType<AlbumEnums>("Sonetta", 0, 1, "Album", "Cannot instantiate Album.");
    qmlRegisterUncreatableType<TrackEnums>("Sonetta", 0, 1, "Track", "Cannot instantiate Track.");

    // Register the UI Singleton type. This is a temporary workaround. Consider creating
    // a loader that dynamically loads any singleton files from a certain directory for greater
    // separation between UI and logic. NOTE TO SELF: It's possible to avoid C++ altogether with modules
    qmlRegisterSingletonType(QUrl::fromLocalFile(QCoreApplication::applicationDirPath() + "/interfaces/default/common/UI.qml"), "Sonetta", 0, 1, "UI");

    // State machine
    qmlRegisterType<QuickGlobalStateTransition>("Sonetta.Utilities", 0, 1, "GlobalStateTransition");
    qmlRegisterSingletonType<QuickGlobalStateMachine>("Sonetta.Utilities", 0, 1, "GlobalStateMachine",
                             [] (QQmlEngine *, QJSEngine *) -> QObject * { return new QuickGlobalStateMachine; });
}

void Application::setupQuickEnvironment()
{
    connect(m_view->engine(), &QQmlEngine::quit, this, &Application::onExit);

    const QString applicationDir = QCoreApplication::applicationDirPath();

    ImageProvider * provider = new ImageProvider(m_session.constCast<const sp::Session>(), m_view.data());
    m_view->engine()->addImageProvider(QLatin1String("sp"), provider);

    m_view->engine()->addImportPath(applicationDir + QStringLiteral("/quick"));
    m_view->engine()->addPluginPath(applicationDir + QStringLiteral("/quick"));
    m_view->engine()->addPluginPath(applicationDir + QStringLiteral("/plugins"));

    m_view->engine()->rootContext()->setContextProperty("player", m_player.data());
    m_view->engine()->rootContext()->setContextProperty("ui", m_ui.data());
    m_view->engine()->rootContext()->setContextProperty("session", m_session.data());
    m_view->engine()->rootContext()->setContextProperty("search", m_search.data());
    m_view->engine()->addImportPath(applicationDir + QStringLiteral("/modules/"));
    m_view->setSource(QUrl::fromLocalFile(applicationDir + QStringLiteral("/interfaces/default/main.qml")));
    m_view->setResizeMode(QQuickView::SizeRootObjectToView);
}

void Application::showUi()
{
    updateCursor();

    // Center view
    QScreen * screen = m_view->screen();
    QPoint screenCenter = screen->availableGeometry().center();
    QPoint windowCenter = m_view->geometry().center();
    m_view->setPosition(screenCenter - windowCenter);
    m_view->showFullScreen();
    //m_view->showNormal();
}

void Application::loadFonts()
{
    qDebug() << "Loading supplied applications fonts...";

    QDir fontDir (QCoreApplication::applicationDirPath() + "/fonts");
    if (fontDir.exists())
    {
        QStringList paths = fontDir.entryList(QDir::Files | QDir::NoDotAndDotDot);
        for (const QString & path : paths)
        {
            QFile file(fontDir.absoluteFilePath(path));
            if (file.open(QIODevice::ReadOnly))
            {
                QByteArray data = file.readAll();
                int id = QFontDatabase::addApplicationFontFromData(data);
                qDebug() << "Loaded font families:" << QFontDatabase::applicationFontFamilies(id);
            }
            else
            {
                qDebug() << "Failed to open font file " << path;
            }
        }
    }

    qDebug() << "Font loading complete.";
}

void Application::createSession()
{
    sp::SessionConfig config;
    config.applicationKey = sp::ApplicationKey(g_appkey, g_appkey_size);
    config.userAgent = "sonetta";
    config.audioOutput = m_output;

    config.settingsLocation = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/libspotify";
    config.cacheLocation = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/libspotify";

    // Create directories if they don't exist
    QDir dir;
    dir.mkpath(config.settingsLocation);
    dir.mkpath(config.cacheLocation);

    m_session.reset(new sp::Session(config));

    // See comment above declaration for rationale
    g_session = m_session;
}

}
