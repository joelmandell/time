#ifndef TTIMER_H
#define TTIMER_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QTimer>
#include <QDomDocument>
#include <QXmlStreamReader>
#include <QStringList>
#include <QFile>
#include <QObject>
#include <QTextCodec>
#include <QProgressBar>
#include <QComboBox>
#include <QSignalMapper>
#include <Phonon/MediaObject>
#include <Phonon/VideoWidget>
#include <Phonon/AudioOutput>
#include <QThread>
class QComboBox;
class QFile;
class QStringList;
class QXmlStreamReader;
class QDomDocument;
class QTimer;
class QPushButton;
class QLineEdit;
class QVBoxLayout;
class QHBoxLayout;
class QWidget;
class QGridLayout;
class QLabel;
class QObject;
class QTextCodec;
class QProgressBar;
class MediaObject;
class VideoWidget;
class AudioOutput;
class QScrollArea;
class QThread;

class TTimerThread : public QThread
{
 Q_OBJECT
 
  public:
    void run();
    QTimer timer;
    QString data;
};

class TTimer : public QMainWindow
{
    Q_OBJECT

    public:
        TTimer(QWidget *parent = 0);
        TTimerThread *thread;
        void firstRunWizard();
        void startTiming();

        QSignalMapper *signalMapper;

        /*VAL*/
        int seconds;
        int minutes;
        int countSpeechItem;
        int totalNrOfItems;
        int totalSeconds;

        /*Config set:ed variables for the application.*/
        QString xmlTimeSchemaFile;		
        /*Config variables end*/
        QWidget *mainWidget;
        QComboBox *timeSchedules;
        QProgressBar *progress;     
        QLabel *lblName;
        QLabel *lblClock;
        
        QStringList *allowingItemOverride;
        QStringList *speechItemLength;     

        QPushButton *btnTimeAlert;      		
        QPushButton *btnTimeStart;
        QPushButton *btnTimeStop; 

        QHBoxLayout *mainLayout;       
        QHBoxLayout *cLayout;
        QGridLayout *gridLayout;
        QScrollArea *scrollArea;

        /*Widgets for configuration dialog:*/
        QWidget *configWidget;
        QGridLayout *configLayout;
        QLineEdit *configEdit;
        QLabel *configLabel;
        QPushButton *btnSpara;

        void init();
        void layoutResize();

    public slots:
        void endAlarm();

        void playAlarm();
        void setConfig();
        void loadConfig();
        void startClock();
        void setTime();
        void timeProperties(QString foo);
        void stopClock();
        void moveIt(QString command);
    
    private:
        void createActions();
        void createMenus();
        QMenu *editMenu;
        QAction *editProperties;
        QAction *createSchema;

    private slots:
        void configDialog();

    protected:
        bool eventFilter(QObject *obj, QEvent *event);
        void paintEvent ( QPaintEvent * event );

};

#endif
