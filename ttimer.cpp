#include <QtGui>
#include "ttimer.h"

/*
TODO LIST:

  * CLEAN UP CODE
  * CHANGING AND SAVING SETTINGS frontend for the Xml file.
  * Function to integrate with the web, PHP, AJAX.
  * trUtf8 all strings so we support internalisation from the beginning if we need to.
  * Log the times, or make option to save the time-tracking for a session.
  * Look into the possibility to make more use of threading!

*/

TTimer::TTimer( QWidget *parent ) : QMainWindow( parent )
{
    QCoreApplication::setApplicationName( "TIMER" );
    
    mainWidget = new QWidget;
    setCentralWidget( mainWidget );
    
    setWindowTitle( trUtf8( "Tidtagningsverktyg för tal och framställningar." ) );
    statusBar()->showMessage( trUtf8( "Välkommen till tidtagnings programmet! Här visas status-meddelanden." ) );
 
    
    //This will be the mainWidgets main-layout!
    mainLayout = new QHBoxLayout( mainWidget );

    this->minutes=0; //Global variable that is keeping track on the elapsed _minutes_
    this->seconds=-1; //Global variable that is keeping track on the elapsed _seconds_
    this->countSpeechItem=0; //Variable that holds the current number of speech item that is used.
    this->totalNrOfItems=0; //Total number of items that we keep track on.
    this->totalSeconds=0; //Holds the total elapsed time as seconds, we use this to make percentage changes. For example in QProcessBar

    cLayout = new QHBoxLayout;

    QLabel *lb=new QLabel( trUtf8( "Välj schema:" ) );
    lb->setText( "" );
    timeSchedules = new QComboBox(); //A combo box that will allow to quickly change wich time schedule/profile we will use.

    TODO: cLayout->addWidget( lb );
    cLayout->addSpacing( 1 );

   
    //TODO: cLayout->addWidget(timeSchedules);
    QVBoxLayout *buttonLayout = new QVBoxLayout;
    // QWidget *scrollWidget = new QWidget();  //The widget that will be hosting a scrollarea!

    scrollArea = new QScrollArea(); //The Scroll Area should be in mainWidget
    scrollArea->setAlignment( Qt::AlignTop );

    // mainLayout->addWidget(scrollArea);
    //mainLayout->addLayout(buttonLayout);
    scrollArea->setWidgetResizable( true );     
        QSizePolicy sizePol(QSizePolicy::Maximum, QSizePolicy::Expanding);
sizePol.setHorizontalStretch(0);
sizePol.setVerticalStretch(0);

   scrollArea->setSizePolicy(sizePol);
    //QVBoxLayout *scrollAreaVLayout = new QVBoxLayout(); 
    QWidget * scrollAreaWidgetContents = new QWidget(); 
       //scrollAreaWidgetContents->setSizePolicy(sizePol);

   // scrollAreaWidgetContents->setLayout(scrollAreaVLayout);
    scrollArea->setWidget( scrollAreaWidgetContents );      
    
    //scrollAreaVLayout->setAlignment(Qt::AlignTop);

   // mainLayout->addWidget(scrollWidget); //Add the scroll hosting widget to the mainLayout.
    mainLayout->addLayout( buttonLayout ); //Add the button layout that will have the buttons to start and stop the time.

   

    gridLayout = new QGridLayout();
    scrollAreaWidgetContents->setLayout( gridLayout );
    gridLayout->setSizeConstraint( QLayout::SetNoConstraint );

    //scrollAreaVLayout->addLayout(gridLayout);
    mainLayout->addWidget( scrollArea );

    speechItemLength = new QStringList; //A list that tracks each and every timeitems length in minutes.
    allowingItemOverride = new QStringList; //A list that tracks wich item that allows the time to continue after its limits.

    btnTimeStart = new QPushButton( trUtf8( "Starta" ) );
    btnTimeStop = new QPushButton( trUtf8( "Stoppa" ) );

    lblClock = new QLabel();  //The label-clock that will show the elapsed time in minutes and seconds.
    
    btnTimeAlert = new QPushButton( "Status" );
    
    buttonLayout->addWidget( btnTimeStart );
    buttonLayout->addWidget( btnTimeStop );
    buttonLayout->addWidget( btnTimeAlert );
    buttonLayout->addWidget( lblClock );

    //Layout the button widgets in a grid.
   /* gridLayout->addWidget(btnTimeStart,0,0);
    gridLayout->addWidget(btnTimeStop,1,0);
    gridLayout->addWidget(btnTimeAlert,6,0);*/
    gridLayout->setRowStretch( 2 , 10 );
    mainLayout->addStretch( 1 );
	
    gridLayout->addItem(new QSpacerItem( 2 , 10 ), 0 , 0 );
    gridLayout->addItem(new QSpacerItem( 2 , 10 ), 0 , 1 );
    gridLayout->addItem(new QSpacerItem( 5 , 5 ), 0 , 3 );
    
    //gridLayout->addWidget(lblClock,5,0);

    progress = new QProgressBar();
    progress->setVisible( false );
    statusBar()->addPermanentWidget( progress );
    //mainLayout->addWidget(progress);
    
    thread = new TTimerThread(); //The timer is in a thread.
    
    thread->start();
    connect( &thread->timer, SIGNAL( timeout() ), this, SLOT( setTime() ) );
    connect( btnTimeStop, SIGNAL( clicked() ), this, SLOT( stopClock() ) );
    connect( btnTimeStart, SIGNAL( clicked() ), this, SLOT( startClock() ) );
		
    createActions();
    createMenus();
    loadConfig();
    init();
    showMaximized();
    this->setMinimumSize( 400 , 350 );
    
    layoutResize();

}
 
void TTimerThread::run()
{
   exec();
}

/*
TTimer::configDialog
  This is a dialog that will give the use possibility to make configuration changes.
*/
void TTimer::configDialog()
{
    
    mainWidget->setEnabled( false );
    configWidget = new QWidget();

    configWidget->setWindowTitle( trUtf8( "Konfiguering" ) );
    configLayout = new QGridLayout( configWidget );
	
    configLabel = new QLabel( "Standard schema:" );
    configEdit = new QLineEdit( xmlTimeSchemaFile );
    btnSpara = new QPushButton( "Spara" );
	
    configLayout->addWidget( configLabel , 0 , 0 );
    configLayout->addWidget( configEdit , 0 , 1 );
    configLayout->addWidget( btnSpara , 2 , 0 );
	
    configWidget->installEventFilter( this );
	
    configWidget->resize( 250 , 100 );
    configWidget->show();
}


/*
TTimer::eventFilter
Function to listen to signals from child widgets like the "configWidget" for example.

QObject will return the widget that the signal came from. And event will provide you with wich
event that was called from that widget.

You can find all the events in the Qt Documentation, search for "QEvent::Type".
*/
bool TTimer::eventFilter(QObject *obj, QEvent *event)
{
     if( obj==configWidget ) //If the signal that is triggered is from the config Widget, then listen to the events!
     {
         if ( event->type() == QEvent::Close ) {
	    mainWidget->setEnabled( true );
             return true;
         } else {
             // standard event processing
             return QObject::eventFilter( obj , event );
         }
     } 
}

void TTimer::paintEvent ( QPaintEvent * event )
{
   // if(thread->timer.isActive()) //Save some memory. Only do this if the timer is active!
    //{
        layoutResize(); 
    //}
}

void TTimer::layoutResize()
{
   QSize newsize=size();
   newsize.setWidth(newsize.width()-100);
   newsize.setHeight(newsize.height()-70);
   
   scrollArea->resize(newsize);

}

void TTimer::createMenus()
{
     editMenu = menuBar()->addMenu( tr( "&Redigera" ) );
     editMenu->addAction( editProperties );
     editMenu->addAction( createSchema );
}

void TTimer::createActions()
{
     editProperties = new QAction( trUtf8( "&Inställningar" ) , this );
     editProperties->setShortcut( trUtf8( "Ctrl+P" ) );
     editProperties->setStatusTip( trUtf8( "Ändra inställningar för programmet." ) );
	 
     createSchema = new QAction( trUtf8( "&Nytt schema") , this );

     connect( editProperties, SIGNAL( triggered() ), this, SLOT( configDialog() ) );
}

void TTimer::loadConfig()
{
  
    /*It's time to load the configuration xml file!*/
    QDomDocument doc( "CONFIGURATION" );
    QFile file( QString("%1/config.xml").arg( QCoreApplication::applicationDirPath() ) );
    if ( !file.open( QIODevice::ReadOnly ) )
         return;
          if ( !doc.setContent( &file ) )
          {
               file.close();
                    return;
          }
    file.close();
          
    QDomElement root = doc.documentElement();
         
    QDomNode n = root.firstChild();
    
    while( !n.isNull() )
    {
    
        QDomElement e = n.toElement();
        if( !e.isNull() )
        {
            if( e.tagName() == "config" )
            {
	        //Tag with module attribute-value 'schema', that means we load that as a standard schedule.
                if( e.attribute( "module" )=="schema" )
                {
                    xmlTimeSchemaFile=e.text();
                }

                //TODO: Possibility to load more configurations, IE: Alarm signal etc.
		
	        //Tag with module attribute-value 'window_title', sets the Application main window title.
		if( e.attribute( "module" )=="window_title" )
                {
                    setWindowTitle( e.text() );
                }
            }
        }

        n = n.nextSibling(); 
        totalNrOfItems++;		
    }
}

void TTimer::setConfig()
{

}

void TTimer::init()
{
    /*This loads the selected time scheme/schedule/profile and creates the widgets for them.*/
    QDomDocument doc( "Speeches pattern" );
    QFile file( QString( "%1/%2" ).arg( QCoreApplication::applicationDirPath() , xmlTimeSchemaFile ) );
    if ( !file.open( QIODevice::ReadOnly ) )
         return;
          if ( !doc.setContent( &file ) )
          {
              file.close();
              return;
          }
    file.close();
          
    QDomElement root = doc.documentElement();
 
    int col=2,row=0, col_2=3, row_2=0;
          
    QDomNode n = root.firstChild();
    
    while( !n.isNull() )
    {
    
        QDomElement e = n.toElement();
        if( !e.isNull() )
        {
            if( e.tagName() == "speech" )
            {
                lblName=new QLabel();
                /*Set an object name to those dynamic widgets, based on a serie of numbers - with the int variable row as datasource for it.*/
                lblName->setObjectName( QString( "speech_time%1" ).arg( row ) );
                lblName->setStyleSheet( "border:1px solid #d3d3d3; background-color:transparent;" );
					
                lblName->setTextFormat( Qt::RichText );
                lblName->setText( e.text() );
				
                /*Store the length for the different speech items!*/
                speechItemLength->append( e.attribute( "length" ) );
 
                /*Store overriding time info for the speeches, this allows us to manually stop the time, and not automatically by the time limit.*/
                allowingItemOverride->append( e.attribute( "override" ) );

                QLineEdit *speechEdit=new QLineEdit();
                speechEdit->setObjectName( QString( "speech%1" ).arg( row ) );
                speechEdit->setEnabled( false );
                speechEdit->setText( "" );

                QPushButton *pbUp = new QPushButton( QIcon( QString( "%1/arrow-heart-shaped-up.png" ).arg( QCoreApplication::applicationDirPath() ) ),"" );
                pbUp->setObjectName( QString( "pbItemEarlier%1" ).arg( row ) );
                pbUp->setToolTip( QString( "pbItemEarlier%1" ).arg( row ) );

                QPushButton *pbDown = new QPushButton( QIcon( QString( "%1/arrow-heart-shaped-down.png" ).arg( QCoreApplication::applicationDirPath() ) ),"" );
                pbDown->setObjectName( QString( "pbItemLater%1" ).arg( row ) );
                pbDown->setToolTip( QString( "pbItemLater%1" ).arg( row ) );

                signalMapper = new QSignalMapper( this );

                signalMapper->setMapping( pbUp , QString( "pbItemEarlier%1" ).arg( row ) );
                signalMapper->setMapping( pbDown , QString( "pbItemLater%1" ).arg( row ) );
                connect( pbUp , SIGNAL( clicked() ), signalMapper, SLOT( map() ) );
                connect( pbDown , SIGNAL( clicked() ), signalMapper, SLOT( map() ) );
                connect( signalMapper , SIGNAL( mapped( const QString & ) ),this, SLOT( moveIt( const QString & ) ) );

                gridLayout->addWidget( lblName , row , col );
                row++;

                gridLayout->addWidget( speechEdit,row_2,col_2 );
                if(row_2!=0) gridLayout->addWidget( pbUp,row_2,col_2+1 );
                gridLayout->addWidget( pbDown,row_2,col_2+2 );

                row_2++;
		totalNrOfItems++;		

            }

        }
        n = n.nextSibling(); 
    }
    totalNrOfItems=totalNrOfItems-2; //Need to subtract 2 cause the for loop also counts the root element tags(<speeches> and </speeches>). 

        QPushButton *pbRem = mainWidget->findChild<QPushButton *>( QString( "pbItemLater%1" ).arg( row_2-1 ) );

        /*If we dont hide the last pbItemLater button the widget will not be removed.*/
        pbRem->hide();
        gridLayout->removeWidget( pbRem );
	
        btnTimeStop->setEnabled( false ); //Disable the possibility to push the stop button because the time is already stopped!


}

void TTimer::moveIt(QString command)
{
    if( command.startsWith( "pbItemLater" ) )
    {
         //Okey we are going to move the item up!!
        int id = QString( command.remove( "pbItemLater",Qt::CaseSensitive ) ).toInt();
	QLineEdit *checklbl = mainWidget->findChild<QLineEdit *>( QString( "speech%1" ).arg( id ) );

        if(checklbl->text() != "") {
            statusBar()->showMessage( trUtf8( "Du kan inte flytta denna tiden, eftersom den har redan slutförts." ),5000 );
	} else {
	speechItemLength->swap( id,id+1 );
	allowingItemOverride->swap( id,id+1 );
        qDebug() << "The id of the pbItemLater pushbutton widget:" << id;
        QLabel *lnSource; 
        QLabel *lnTarget;    
      
        lnSource = mainWidget->findChild<QLabel *>( QString( "speech_time%1" ).arg( id ) );
        lnTarget = mainWidget->findChild<QLabel *>( QString( "speech_time%1" ).arg( id+1 ) );            
        
        QString copyOrginal = lnSource->text();
        statusBar()->showMessage( QString( "%2%1" ).arg( copyOrginal,trUtf8( "Senarelade tidtagning: " ) ) );

        lnSource->setText( lnTarget->text() );
        lnTarget->setText( copyOrginal );
	}
    }

    if(command.startsWith("pbItemEarlier"))
    {     
        int id = QString( command.remove( "pbItemEarlier",Qt::CaseSensitive ) ).toInt();
	QLineEdit *checklbl = mainWidget->findChild<QLineEdit *>( QString( "speech%1" ).arg( id-1 ) );

        if(checklbl->text() != "") {
            statusBar()->showMessage( trUtf8( "Du kan inte flytta denna tiden, eftersom ovanstående tid har redan slutförts." ),5000 );
	} else {
        /*TODO: Implement so you cannot step behind a item that is currently in use. THAT SHOULD NOT BE POSSIBLE!*/
         //Okey we are going to move the item down!!
	speechItemLength->swap( id,id-1 );
	allowingItemOverride->swap( id,id-1 );

        QLabel *lnSource = mainWidget->findChild<QLabel *>( QString( "speech_time%1" ).arg( id-1 ) );
        QLabel *lnTarget = mainWidget->findChild<QLabel *>( QString( "speech_time%1" ).arg( id) );
        QString copyOrginal = lnSource->text();
        statusBar()->showMessage( QString( "%2%1" ).arg( copyOrginal,trUtf8( "Tidigarelade tidtagning: " ) ) );
        lnSource->setText( lnTarget->text() );
        lnTarget->setText( copyOrginal );
	}
    }
}

void TTimer::firstRunWizard()
{

}

void TTimer::timeProperties( QString yaa )
{
}

void TTimer::startClock()
{
    qDebug() << "startClock() countSpeechItem is:" << countSpeechItem;
    qDebug() << "startClock() totalNrOfItems is:" << totalNrOfItems;
    qDebug() << "startClock() time limit is:" << speechItemLength->at( countSpeechItem );

    if(countSpeechItem!=totalNrOfItems)
    {
        btnTimeStart->setEnabled( false ); //We have to disable the start button otherwise somebody may push it in mistake and mess up everything.
	btnTimeStop->setEnabled( true ); //Enable the possibility to push the stop button because the time is started!

        lblClock->setStyleSheet( "font-family:Tahoma; font-size:32px; color: #000" );
        QLineEdit *speechEdit = mainWidget->findChild<QLineEdit *>( QString( "speech%1" ).arg( countSpeechItem ) );
        speechEdit->setEnabled( true ); 
	speechEdit->setText( trUtf8( "Räknar denna tid nu!" ) );

	if( countSpeechItem==0 ) {
	    QPushButton *pbMove = mainWidget->findChild<QPushButton *>( QString( "pbItemLater%1" ).arg( countSpeechItem ) );
            pbMove->setEnabled( false );
	} else {
            QPushButton *pbMove = mainWidget->findChild<QPushButton *>( QString( "pbItemEarlier%1" ).arg( countSpeechItem ) );
            pbMove->setEnabled( false );
            pbMove = mainWidget->findChild<QPushButton *>( QString( "pbItemLater%1" ).arg( countSpeechItem ) );
            pbMove->setEnabled( false );
	}
        progress->setVisible( true );
        progress->setValue( 0 );
        progress->setMaximum( speechItemLength->at( countSpeechItem ).toInt()*60 );

        thread->timer.start( 1000 );
    	
	if( countSpeechItem==totalNrOfItems )
	{
	    btnTimeStart->setEnabled( false ); 
	}
    }
}

void TTimer::stopClock()
{
    layoutResize();

    thread->timer.stop();
    lblClock->setText( "" );
    totalSeconds=0;
    
    qDebug() << "stopClock() countSpeechItem is:" << countSpeechItem;
    qDebug() << "stopClock() totalNrOfItems is:" << totalNrOfItems;

    if( countSpeechItem!=totalNrOfItems )
    {
        btnTimeStart->setEnabled( true ); //Enable the button to start the watch again. 
        btnTimeStop->setEnabled( false ); //Disable the possibility to push the stop button because the time is already stopped!
        /*Found the QLineEdit according to the speech control id. That is countSpeechItem that beholds the last used one.*/
        QLineEdit *speechEdit = mainWidget->findChild<QLineEdit *>( QString( "speech%1" ).arg( countSpeechItem ) );
	
	if( countSpeechItem==0 ) {
	    QPushButton *pbMove = mainWidget->findChild<QPushButton *>( QString( "pbItemLater%1" ).arg( countSpeechItem ) );
            pbMove->setEnabled( false );
	} else {
            QPushButton *pbMove = mainWidget->findChild<QPushButton *>( QString( "pbItemEarlier%1" ).arg( countSpeechItem ) );
            pbMove->setEnabled( false );
            pbMove = mainWidget->findChild<QPushButton *>( QString( "pbItemLater%1" ).arg( countSpeechItem ) );
            pbMove->setEnabled( false);
	}
	
        progress->setVisible( false );
        progress->setValue( 0 );

        QString resultSeconds;
        /*If it's below 10 seconds, then there needs to be appended a zero as for design.*/
        if( seconds<=9 ) resultSeconds.append( "0" );

        speechEdit->setEnabled( false ); 
	speechEdit->setText( QString( "%1:%2%3" ).arg( minutes ).arg( resultSeconds ).arg( seconds ) );

        countSpeechItem++;
	
	if( countSpeechItem==totalNrOfItems )
	{
	    btnTimeStart->setEnabled( false ); 
	}
	
    } 
    minutes=0;
    seconds=-1;

}

void TTimer::setTime()
{   

    QLabel *speechItemName;
	
    if( countSpeechItem!=totalNrOfItems )
    {
        speechItemName = mainWidget->findChild<QLabel *>( QString( "speech_time%1" ).arg( countSpeechItem ) );
    }
	
    int val;
	
    if( countSpeechItem!=totalNrOfItems )
    {
        val=speechItemLength->at( countSpeechItem ).toInt();
    } else {
        btnTimeStart->setEnabled( false );
	btnTimeStop->setEnabled( false );
    }
	
    seconds++;
    totalSeconds++;
    progress->setValue( totalSeconds );
	
    if( seconds==60 )
    {
        minutes++;
        seconds=0;
    }

    if( minutes<=val-1 )
    {
        btnTimeAlert->setStyleSheet( "background-color:#48a73e" ); //Green status
    }

    if( minutes>=val-1 && seconds>=30 )
    {
        btnTimeAlert->setStyleSheet( "background-color:#e9f101" ); //Yellow Status
    }
    
    if( minutes>=val-1 && seconds>=50 )
    {
        btnTimeAlert->setStyleSheet( "background-color:#e51f16" ); //Red status
    }

    if( minutes>=val )
    {
        btnTimeAlert->setStyleSheet( "background-color:#e51f16" ); //Keep having red status cause we are going over the time here!!

        /*If the override attribute is not set then stop the clock*/
        if( allowingItemOverride->at( countSpeechItem )!="yes" )
        {
            stopClock();
	    playAlarm();
        } 
    }
    
    if( seconds==0 ) lblClock->setText( QString( "%1 : 0%2" ).arg( minutes ).arg( seconds ) ); setWindowTitle( QString( trUtf8( "%3 - %1:0%2" ) ).arg( minutes ).arg( seconds ).arg( speechItemName->text() ) );
    if( seconds>!10 ) lblClock->setText( QString( "%1 : 0%2" ).arg( minutes ).arg( seconds ) ); setWindowTitle( QString( trUtf8( "%3 - %1:0%2" ) ).arg( minutes ).arg( seconds ).arg( speechItemName->text() ) );
    if( seconds>=10 ) lblClock->setText( QString( "%1 : %2" ).arg( minutes ).arg( seconds ) ); setWindowTitle( QString( trUtf8( "%3 - %1:%2" ) ).arg( minutes ).arg( seconds ).arg( speechItemName->text() ) );

}

void TTimer::playAlarm()
{
   Phonon::MediaObject *player = new Phonon::MediaObject( this );
   Phonon::VideoWidget *videoWidget = new Phonon::VideoWidget( this );
   Phonon::createPath( player, videoWidget );

   Phonon::AudioOutput *audioOutput = new Phonon::AudioOutput( Phonon::VideoCategory, this );
   Phonon::createPath( player, audioOutput );
   QUrl url( QString( "file:///%1/test.mp3" ).arg( QCoreApplication::applicationDirPath() ) );

   qDebug() << "Using this device:" << audioOutput->outputDevice();
   player->setCurrentSource( url );
   player->play();
   
   /*If there is not a signal connected that the play-sequence is ended there will be a total application lockup resulting
   in the need to kill it the hard way! 
   */
   connect( player, SIGNAL( finished() ), SLOT( endAlarm() ) );
}

void TTimer::endAlarm()
{
   /*Function FOO, nothing to be done here anymore, then providing a slot connected to the finished signal of 
   the MediaObject. This exists to prevent the application to lockup!
   */
}