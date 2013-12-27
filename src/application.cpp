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

#include "imageprovider.h"

#include "quick/enums.h"
#include "quick/models.h"
#include "quick/quicktrackinfo.h"
#include "quick/quickmosaicgenerator.h"
#include "quick/quickfactory.h"

#include "../appkey.c"

namespace sp = Spotinetta;

namespace Sonetta {

namespace {

void deleteAudioOutputLater(AudioOutput * output) { output->deleteLater(); }
void deleteSettingsLater(Settings * settings) { settings->deleteLater(); }

}

Application::Application(int &argc, char **argv)
    :   QGuiApplication(argc, argv), m_view(new QQuickView),
      m_output(new AudioOutput, deleteAudioOutputLater), m_settings(new Settings, deleteSettingsLater),
      m_lirc(new LircClient(this)), m_exiting(false)
{
    QGuiApplication::addLibraryPath(applicationDirPath() + QStringLiteral("/plugins"));

    createSession();

    m_player = new Player(m_session, m_output.data(), this);
    m_ui = new UIStateCoordinator(this);
    m_search = new SearchEngine(m_session, m_settings, this);

    connect(m_session, &sp::Session::loggedOut, this, &Application::onLogout);
    connect(m_session, &sp::Session::log, [] (const QString &msg) { qDebug() << msg; });

    connect(m_settings.data(), &Settings::mouseEnabledChanged, this, &Application::updateCursor);

    m_lirc->attach();
}

int Application::run()
{
    loadFonts();
    registerQmlTypes();

    if (m_session->error() == sp::Error::Ok)
    {
        setupQuickEnvironment();
        showUi();

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

void Application::onExit()
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

void Application::onLogout()
{
    if (m_exiting)
    {
        quit();
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
    switch (event->type())
    {
    case QEvent::MouseButtonDblClick:
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
        if (!m_settings->mouseEnabled())
            break;
    default:
        return QGuiApplication::notify(receiver, event);
    }

    return true;
}

void Application::registerQmlTypes()
{
    Navigation::registerTypes();

    qmlRegisterType<QuickPlaylistContainerModel>("Sonetta", 0, 1, "PlaylistContainerModel");
    qmlRegisterType<QuickPlaylistModel>("Sonetta", 0, 1, "PlaylistModel");
    qmlRegisterType<QuickAlbumModel>("Sonetta", 0, 1, "AlbumModel");
    qmlRegisterType<QuickTrackInfo>("Sonetta", 0, 1, "TrackInfo");
    qmlRegisterType<QuickMosaicGenerator>("Sonetta", 0, 1, "MosaicGenerator");

    qmlRegisterUncreatableType<Spotinetta::Session>("Sonetta", 0, 1, "Session", "Cannot instantiate Session.");
    qmlRegisterSingletonType<QuickFactory>("Sonetta", 0, 1, "Factory", &quickFactorySingletonProvider);

    // Enums
    qmlRegisterUncreatableType<AlbumEnums>("Sonetta", 0, 1, "Album", "Cannot instantiate Album.");
    qmlRegisterUncreatableType<TrackEnums>("Sonetta", 0, 1, "Track", "Cannot instantiate Track.");

    // Register the UI Singleton type. This is a temporary workaround. Consider creating
    // a loader that dynamically loads any singleton files from a certain directory for greater
    // separation between UI and logic. NOTE TO SELF: It's possible to avoid C++ altogether with modules
    qmlRegisterSingletonType(QUrl::fromLocalFile(applicationDirPath() + "/qml/default/common/UI.qml"), "Sonetta", 0, 1, "UI");

}

void Application::setupQuickEnvironment()
{
    connect(m_view->engine(), &QQmlEngine::quit, this, &Application::onExit);

    QString applicationDir = applicationDirPath();

    ImageProvider * provider = new ImageProvider(m_session, m_view.data());
    m_view->engine()->addImageProvider(QLatin1String("sp"), provider);

    m_view->engine()->addImportPath(applicationDir + QStringLiteral("/quick"));
    m_view->engine()->addPluginPath(applicationDir + QStringLiteral("/quick"));
    m_view->engine()->addPluginPath(applicationDir + QStringLiteral("/plugins"));

    m_view->engine()->rootContext()->setContextProperty("player", m_player);
    m_view->engine()->rootContext()->setContextProperty("ui", m_ui);
    m_view->engine()->rootContext()->setContextProperty("session", m_session);
    m_view->engine()->rootContext()->setContextProperty("search", m_search);
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

    QDir fontDir (applicationDirPath() + "/fonts");
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
    config.userAgent = "Sonetta";
    config.audioOutput = m_output;

    config.settingsLocation = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/libspotify";
    config.cacheLocation = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/libspotify";

    // Create directories if they don't exist
    QDir dir;
    dir.mkpath(config.settingsLocation);
    dir.mkpath(config.cacheLocation);

    m_session = new sp::Session(config, this);
}

}
