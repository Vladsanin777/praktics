#include <QApplication>
#include <QQmlApplicationEngine>
#include <QTranslator>
#include <QLocale>
#include <QFile>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QFile styleFile("main.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        app.setStyleSheet(styleSheet);
    }

    QTranslator translator;
    QLocale systemLocale = QLocale::system();
    if (translator.load(systemLocale, "TextEditor", "_", ":/i18n")) {
        app.installTranslator(&translator);
    }

    QQmlApplicationEngine engine;
    
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    
    engine.load(url);

    return app.exec();
}
