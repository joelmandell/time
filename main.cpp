#include <QApplication>
#include "ttimer.h"
#include <QTranslator>

int main(int argc, char **argv)
{

    QApplication app(argc, argv);
          
    QTranslator translator;
    translator.load("lang/"+QLocale::system().name());
    app.installTranslator(&translator);
    
    TTimer timeApp;
    timeApp.show();
    
    return app.exec();
}
