#include "AppMain.h"
#include <QtWidgets/QGraphicsItem>
#include <iostream>
#include <streambuf>
#include <string>
#include <QObject>
#include "state_modification_callback.hpp"


AppMain::AppMain(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AppMainClass()),
    eventMan(nullptr),
    event_proc_dialog(new EventProcessorDialog(this)),
    pauseIcon(QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaybackPause)),
    playIcon(QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaybackStart)),
    currentIcon(true),
	replayPaused(true),
	liveMode(true)
{
    ui->setupUi(this);
    outputHandle = new StdoutStreamBuffer(this);
    connect(outputHandle, &StdoutStreamBuffer::outputCaptured, this, &AppMain::appendOutput);

    scene = (QGraphicsScene*) new CricketOvalScene();
    ui->graphicsView->setScene(scene);

    propsGroup = new PlayerPropertiesGroup(
        ui->label_3,
        ui->lineEdit_2,
        ui->lineEdit_3,
        ui->label_7
    );

    playerStateModifier = new PlayerStateModifierGroup(
        ui->checkBox,
        ui->lineEdit,
        ui->checkBox_2,
        ui->comboBox,
        ui->pushButton
    );

    distanceObjectsGroup = new DistanceObjectsManager(
        ui->objectsList,
        ui->scrollArea
    );

   

    connect(ui->actionStart_Live_Data_Capture, &QAction::triggered, this, &AppMain::openEventProcessorDialog);
    connect(event_proc_dialog, &EventProcessorDialog::event_processor_name, this, &AppMain::sendEventProcessorName);
    connect(ui->actionPause_Output_Stream, &QAction::triggered, this, &AppMain::PauseOutputStreamTrigger);
	connect(ui->live_mode_button, &QPushButton::clicked, this, &AppMain::setLiveMode);
	connect(ui->seeker_bar, &QSlider::sliderPressed, this, &AppMain::setReplayMode);
	connect(ui->seeker_bar, &QSlider::sliderReleased, this, &AppMain::setSeekerPosition);
	connect(ui->replay_control, &QPushButton::clicked, this, &AppMain::replayControl);

    setLiveMode();
}

AppMain::~AppMain()
{
    delete ui;
}

void AppMain::closeEvent(QCloseEvent* e)
{
    if (eventMan) {
        eventMan->exit();
   }
}

void AppMain::setStreamDataStore(StreamDataStore* sDs)
{
    streamDs = sDs;
    CricketOvalScene* cS = static_cast<CricketOvalScene*>(scene);
    QMetaObject::Connection con = connect(sDs, &StreamDataStore::dataChanged, 
        cS , &CricketOvalScene::dataChangeUpdate);

    connect(sDs, &StreamDataStore::dataChanged,
        this, &AppMain::updateStatusBarFrameCount);


   con = connect(cS, &CricketOvalScene::selectedIdChangedSig, sDs, &StreamDataStore::setCurrentClicked);

   con = connect(sDs, &StreamDataStore::currentSelectedChangedSig, 
       propsGroup, &PlayerPropertiesGroup::currentClickedTrack);

   con = connect(sDs, &StreamDataStore::selectedStateChagedSig,
       playerStateModifier, &PlayerStateModifierGroup::laodCurrentState);


  if (con)
  {
      std::cout << "Connection was made successfully\n";
  }
  else {
      std::cout << "Failed to make the connection!!!!!!!!!!\n";
  }
}

void AppMain::setStatePlayerStateModifier(std::shared_ptr<StateModificationCb> stMod)
{
    stateMod = stMod;
    QMetaObject::Connection con;
    con = connect(playerStateModifier, &PlayerStateModifierGroup::playerStateChanged,
        stateMod.get(), &StateModificationCb::playerStateChanged, Qt::DirectConnection);

    if (con)
    {
        std::cout << "Connection was made successfully\n";
    }
    else {
        std::cout << "Failed to make the connection!!!!!!!!!!\n";
    }

}

void AppMain::openEventProcessorDialog()
{
    if (event_proc_dialog)
    {
        event_proc_dialog->open();
    }
}

void AppMain::PauseOutputStreamTrigger()
{
    
    QAction* aToggleOutStream = ui->actionPause_Output_Stream;
    // flip the icon
    KEvents::Event e;
    e.setEventData({});
    e.setSourceModule("gui");
    if (currentIcon)
    {
        aToggleOutStream->setIcon(playIcon);
        e.setEventName(EN_STREAM_PAUSE);
        std::cout << "Stream Pause pressed" << std::endl;
       
        aToggleOutStream->setText("Resume Output Stream");
    }
    else {
        aToggleOutStream->setIcon(pauseIcon);
        e.setEventName(EN_STREAM_RESUME);
        std::cout << "Stream Resume pressed" << std::endl;
        aToggleOutStream->setText("Pause Output Stream");
    }
    currentIcon = !currentIcon;

    json config = KEvents::__load_config__();
    eventMan->sendEvent(config["DataAggregator"]["serviceTopic"], e);
   
}

void AppMain::setLiveMode()
{
    ui->live_mode_button->setDisabled(true);
	ui->seeker_bar->setSliderPosition(ui->seeker_bar->maximum());
	int sliderPosition = ui->seeker_bar->maximum();
	liveMode = true;

    KEvents::Event e;
    e.setEventData({ {"live_mode", liveMode } });
    e.setSourceModule("gui");
    e.setEventName(EN_SET_STREAM_MODE);
    json config = KEvents::__load_config__();

    if(eventMan)
        eventMan->sendEvent(config["SportEventProcessor"]["serviceTopic"], e);
	
}

void AppMain::setReplayMode()
{
	ui->live_mode_button->setDisabled(false);
    int sliderPosition = ui->seeker_bar->sliderPosition();
	liveMode = false;
	std::cout << sliderPosition << std::endl;
    sendSeekerEvent(sliderPosition);
}

void AppMain::setSeekerPosition()
{
	int sliderPosition = ui->seeker_bar->sliderPosition();
	sendSeekerEvent(sliderPosition);
}

void AppMain::updateStatusBarFrameCount()
{
  
}

void AppMain::sendSeekerEvent(int seekerPosition)
{
	KEvents::Event e;
    e.setEventData({ {"seeker_position", seekerPosition}, {"live_mode" , false} });
	e.setSourceModule("gui");
	e.setEventName(EN_SET_SEEKER_POSITION);
	json config = KEvents::__load_config__();
	eventMan->sendEvent(config["SportEventProcessor"]["serviceTopic"], e);
}

void AppMain::replayControl()
{
	replayPaused = !replayPaused;
    if (!replayPaused)
    {
        ui->replay_control->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaybackPause));
	}
	else {
		ui->replay_control->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaybackStart));
    }

    KEvents::Event e;
    e.setEventData({ {"playerbackpaused", replayPaused } });
    e.setSourceModule("gui");
    e.setEventName(EN_PLAYBACK_CONTROL);
    json config = KEvents::__load_config__();
    eventMan->sendEvent(config["SportEventProcessor"]["serviceTopic"], e);
}
