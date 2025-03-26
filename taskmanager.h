#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <QObject>
#include <QThread>
#include <QVector>
#include <memory>
#include "worker.h"

class TaskManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(QString result READ result NOTIFY resultChanged)
    Q_PROPERTY(bool isRunning READ isRunning NOTIFY isRunningChanged)

public:
    explicit TaskManager(QObject *parent = nullptr);
    ~TaskManager();

    int progress() const;
    QString result() const;
    bool isRunning() const;

public slots:
    void startTask(int numThreads, int iterationsPerThread, int delayMs);
    void stopAllTasks();

signals:
    void progressChanged(int progress);
    void resultChanged(const QString &result);
    void isRunningChanged(bool isRunning);
    void allTasksCompleted();

private slots:
    void handleWorkProgress(int progress);
    void handleWorkFinished(int iterations);
    void handleResultReady(const QString &partialResult);

private:
    QVector<QThread*> m_threads;
    QVector<Worker*> m_workers;
    int m_progress;
    QString m_result;
    int m_completedThreads;
    int m_totalThreads;
    bool m_running;
};

#endif // TASKMANAGER_H
