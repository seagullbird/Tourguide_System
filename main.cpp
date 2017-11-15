#include <QApplication>
#include "welcome.h"
#include "database.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Welcome welcome_window;
    welcome_window.setWindowTitle("Welcome");
    welcome_window.show();


    return a.exec();
}
