#include "worker.h"
#include <QThread>
#include <QDebug>

Worker::Worker(QObject *parent) : QObject(parent), m_abort(false)
{
}

Worker::~Worker()
{
    stopWork();
}

void Worker::doHeavyWork(int iterations, int delayMs)
{
    // Reset abort flag
    QMutexLocker locker(&m_mutex);
    m_abort = false;
    locker.unlock();

    QString result;

    for (int i = 0; i < iterations; ++i) {
        // Check if we need to abort
        locker.relock();
        bool abort = m_abort;
        locker.unlock();

        if (abort) {
            qDebug() << "Worker aborted";
            break;
        }

        // Simulate heavy computational work
        QThread::msleep(delayMs);

        // Perform some "work" - in this case, generating a string
        result += QString("Iteration %1 completed in thread %2\n")
                      .arg(i + 1)
                      .arg(reinterpret_cast<quintptr>(QThread::currentThreadId()));

        // Update progress - emit signal
        emit progressUpdated((i + 1) * 100 / iterations);
    }

    emit resultReady(result);
    emit workFinished(iterations);
}

void Worker::stopWork()
{
    QMutexLocker locker(&m_mutex);
    m_abort = true;
}
