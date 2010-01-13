#include <QApplication>
#include "ttimer.h"

int main(int argc, char **argv)
{

    QApplication app(argc, argv);
    TTimer timeApp;
    timeApp.show();
    
    return app.exec();
}
