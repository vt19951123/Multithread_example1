#include "taskmanager.h"
#include <QDebug>

TaskManager::TaskManager(QObject *parent)
    : QObject(parent)
    , m_progress(0)
    , m_completedThreads(0)
    , m_totalThreads(0)
    , m_running(false)
{
}

TaskManager::~TaskManager()
{
    stopAllTasks();
}

int TaskManager::progress() const
{
    return m_progress;
}

QString TaskManager::result() const
{
    return m_result;
}

bool TaskManager::isRunning() const
{
    return m_running;
}

void TaskManager::startTask(int numThreads, int iterationsPerThread, int delayMs)
{
    // Clear previous state
    stopAllTasks();

    m_result = "";
    emit resultChanged(m_result);

    m_progress = 0;
    emit progressChanged(m_progress);

    m_completedThreads = 0;
    m_totalThreads = numThreads;

    m_running = true;
    emit isRunningChanged(m_running);

    // Create and start worker threads
    for (int i = 0; i < numThreads; ++i) {
        QThread* thread = new QThread();
        Worker* worker = new Worker();

        // Move worker to thread
        worker->moveToThread(thread);

        // Connect signals and slots
        connect(thread, &QThread::started, [worker, iterationsPerThread, delayMs]() {
            worker->doHeavyWork(iterationsPerThread, delayMs);
        });

        connect(worker, &Worker::progressUpdated, this, &TaskManager::handleWorkProgress);
        connect(worker, &Worker::workFinished, this, &TaskManager::handleWorkFinished);
        connect(worker, &Worker::resultReady, this, &TaskManager::handleResultReady);

        connect(thread, &QThread::finished, worker, &QObject::deleteLater);
        connect(thread, &QThread::finished, thread, &QObject::deleteLater);

        // Store threads and workers for later cleanup
        m_threads.append(thread);
        m_workers.append(worker);

        // Start thread
        thread->start();
    }
}

void TaskManager::stopAllTasks()
{
    if (m_threads.isEmpty()) {
        return;
    }

    // Request all workers to stop
    for (Worker* worker : m_workers) {
        if (worker) {
            worker->stopWork();
        }
    }

    // Wait for all threads to finish
    for (QThread* thread : m_threads) {
        if (thread) {
            thread->quit();
            thread->wait();
        }
    }

    // Clear collections
    m_threads.clear();
    m_workers.clear();

    m_running = false;
    emit isRunningChanged(m_running);
}

void TaskManager::handleWorkProgress(int progress)
{
    // This is a simplified progress calculation for multiple threads
    // In a real application, you might want a more sophisticated approach
    m_progress = (m_progress + progress) / 2;
    emit progressChanged(m_progress);
}

void TaskManager::handleWorkFinished(int iterations)
{
    m_completedThreads++;

    if (m_completedThreads >= m_totalThreads) {
        m_running = false;
        emit isRunningChanged(m_running);
        emit allTasksCompleted();
    }
}

void TaskManager::handleResultReady(const QString &partialResult)
{
    m_result += partialResult;
    emit resultChanged(m_result);
}
