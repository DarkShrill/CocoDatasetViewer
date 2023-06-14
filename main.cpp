#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QObject>
#include <QString>
#include <QFile>
#include <QList>
#include <QTextStream>
#include <QQuickView>
#include <QQmlContext>
#include "imagelabeler.h"

static ImageLabeler * image_labeler = nullptr;

static QObject * Get_ImageLabeler(QQmlEngine * engine, QJSEngine *scriptEngine)
{


    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);


    if(image_labeler == nullptr){
        image_labeler = new ImageLabeler();
    }

    return image_labeler;
}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    app.setOrganizationName("DarkShrill");
    app.setOrganizationDomain("https://github.com/DarkShrill");

    QQmlApplicationEngine engine;

    qmlRegisterSingletonType<ImageLabeler>("it.videx.label",1,0,"ImageLabeler",Get_ImageLabeler);

    ImageLabeler labeler;
    engine.rootContext()->setContextProperty("labeler", &labeler);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
