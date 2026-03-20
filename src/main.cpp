#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QCommandLineParser>
#include <QFileInfo>
#include <QUrl>
#include <QQuickWindow>
#include <QStandardPaths>
#include <QDir>
#include <QProcess>
#include <QIcon>
#include <iostream>
#include "FileHandler.h"

using namespace Qt::StringLiterals;

void setupKView() {
    QString exePath = QCoreApplication::applicationFilePath();
    QString dirPath = QFileInfo(exePath).dir().absolutePath();
    QString iconPath = QDir(dirPath).absoluteFilePath("logo.png");
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation) + "/kview.desktop";
    QString serviceMenuDir = QDir::homePath() + "/.local/share/kio/servicemenus";
    QString serviceMenuPath = serviceMenuDir + "/kview.desktop";

    QDir().mkpath(serviceMenuDir);

    QFile desktopFile(desktopPath);
    if (desktopFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&desktopFile);
        out << "[Desktop Entry]\n";
        out << "Type=Application\n";
        out << "Name=KView\n";
        out << "Comment=Fast File Previewer\n";
        out << "Exec=" << exePath << " %f\n";
        out << "Icon=" << iconPath << "\n";
        out << "Terminal=false\n";
        out << "Categories=Utility;\n";
        out << "MimeType=image/*;video/*;text/plain;application/pdf;application/zip;\n";
        desktopFile.close();
        QFile::setPermissions(desktopPath, QFile::Permission(0x777));
    }

    QFile serviceFile(serviceMenuPath);
    if (serviceFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&serviceFile);
        out << "[Desktop Entry]\n";
        out << "Type=Service\n";
        out << "MimeType=application/octet-stream;inode/directory;image/*;video/*;audio/*;text/plain;application/pdf;application/zip;application/x-tar;application/x-7z-compressed;\n";
        out << "Actions=kviewPreview;\n";
        out << "X-KDE-ServiceTypes=KonqPopupMenu/Plugin\n";
        out << "X-KDE-Priority=TopLevel\n\n";
        out << "[Desktop Action kviewPreview]\n";
        out << "Name=KView Preview\n";
        out << "Icon=" << iconPath << "\n";
        out << "Exec=" << exePath << " %f\n";
        serviceFile.close();
        QFile::setPermissions(serviceMenuPath, QFile::Permissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner | QFileDevice::ExeOwner | QFileDevice::ReadGroup | QFileDevice::ExeGroup | QFileDevice::ReadOther | QFileDevice::ExeOther));
    }

    QProcess::execute("kbuildsycoca6", {});

    std::cout << "KView Initialized!\n";
    std::cout << "------------------\n";
    std::cout << "1. Desktop file created: " << desktopPath.toStdString() << "\n";
    std::cout << "2. Service Menu created: " << serviceMenuPath.toStdString() << "\n";
    std::cout << "   (Adds 'KView Preview' to Dolphin right-click menu)\n";
    std::cout << "\nHow to use:\n";
    std::cout << "  - Escape: Close\n";
    std::cout << "  - Enter/Return: Open file with default application\n";
    std::cout << "  - Left/Right Arrows: Previous/Next file\n";
    std::cout << "  - Up/Down Arrows: Scroll\n";
    std::cout << "  - Space: Play/Pause (Video/Audio)\n";
    std::cout << "  - Q/E: Seek Backward/Forward (Video/Audio)\n";
    }int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setOrganizationName("KView");
    app.setApplicationName("KView");
    app.setWindowIcon(QIcon(QFileInfo(QCoreApplication::applicationFilePath()).dir().absoluteFilePath("logo.png")));

    QCommandLineParser parser;
    parser.setApplicationDescription("KView - A fast file previewer");
    parser.addHelpOption();
    parser.addPositionalArgument("file", "The file to preview");
    parser.process(app);

    const QStringList args = parser.positionalArguments();
    if (args.isEmpty()) {
        setupKView();
        return 0;
    }

    QString filePath = QFileInfo(args.first()).absoluteFilePath();
    FileHandler fileHandler;
    fileHandler.setFilePath(filePath);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("fileHandler", &fileHandler);
    
    const QUrl url(u"qrc:/KView/src/Main.qml"_s);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) {
            QCoreApplication::exit(-1);
        } else {
            
            if (QQuickWindow *window = qobject_cast<QQuickWindow*>(obj)) {
                window->requestActivate();
            }
        }
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
