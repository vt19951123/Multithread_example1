#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "taskmanager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // Create task manager instance
    TaskManager taskManager;

    // Register and setup QML engine
    QQmlApplicationEngine engine;

    // Expose TaskManager to QML
    engine.rootContext()->setContextProperty("taskManager", &taskManager);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("Multithreading_exercise1", "Main");

    return app.exec();
}
