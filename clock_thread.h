#ifndef CLOCK_THREAD_H
#define CLOCK_THREAD_H
#include <QThread>
#include <QTimer>
#include <QDateTime>
#include <QDebug>
class Clock_Thread : public QThread
{
    Q_OBJECT
public:
    explicit Clock_Thread(QObject *parent = 0);
    void run();
    QTimer *clock_timer;

signals:
    void timer_signal();

};

#endif // CLOCK_THREAD_H
