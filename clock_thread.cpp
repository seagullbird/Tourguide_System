#include "clock_thread.h"

Clock_Thread::Clock_Thread(QObject *parent) : QThread(parent)
{

}

void Clock_Thread::run()
{
    clock_timer = new QTimer;
    connect(clock_timer, SIGNAL(timeout()), this, SIGNAL(timer_signal()));
    clock_timer->setInterval(1);
    clock_timer->start();
    this->exec();
}
