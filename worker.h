#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QThread>
#include <QMutex>

class Worker : public QObject
{
    Q_OBJECT

public:
    explicit Worker(QObject *parent = nullptr);
    ~Worker();

public slots:
    void doHeavyWork(int iterations, int delayMs);
    void stopWork();

signals:
    void progressUpdated(int progress);
    void workFinished(int totalIterations);
    void resultReady(const QString &result);

private:
    bool m_abort;
    QMutex m_mutex;
};

#endif // WORKER_H
