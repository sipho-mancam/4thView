#include "AppMain.h"
#include <QtWidgets/QGraphicsItem>
#include <iostream>
#include <streambuf>
#include <string>
#include <QObject>
#include "state_modification_callback.hpp"
#include "plotter_controller.hpp"



AppMain::AppMain(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AppMainClass()),
    eventMan(nullptr),
    eventProcDialog(new EventProcessorDialog(this)),
    distanceDialog(new DistanceDialog(this)),
	teamsConfigDialog(new TeamsConfigManager(this)),
    pauseIcon(QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaybackPause)),
    playIcon(QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaybackStart)),
    currentIcon(true),
	replayPaused(false),
	liveMode(true),
    seekingBack(false),
	currentSliderPosition(0),
	distanceObjectModel(new DistanceObjectModel(this))
{
    ui->setupUi(this);
    init(); 
}

AppMain::~AppMain()
{
	delete eventProcDialog;
    delete distanceDialog;
	delete distanceObjectModel;
	delete teamsConfigDialog;
    delete ui;
    delete systemEventsSender;
}

void AppMain::closeEvent(QCloseEvent* e)
{
    if (eventMan) {
        eventMan->exit();
   }
}

void AppMain::setEventManager(KEvents::EventsManager* evMan)
{
    eventMan = evMan; 
	systemEventsSender = new SystemEventsSender(eventMan, this);
}

void AppMain::setStreamDataStore(StreamDataStore* sDs)
{
    streamDs = sDs;
    PitchViewScene* cS = static_cast<PitchViewScene*>(scene);
    QMetaObject::Connection con = connect(sDs, &StreamDataStore::dataChanged, 
        cS , &PitchViewScene::dataChangeUpdate);


	connect(sDs, &StreamDataStore::dataChanged, distanceObjectModel, &DistanceObjectModel::updateFrameData);

    connect(sDs, &StreamDataStore::dataChanged,
        this, &AppMain::updateStatusBarFrameCount);


   con = connect(cS, &PitchViewScene::selectedIdChangedSig, sDs, &StreamDataStore::setCurrentClicked);

   con = connect(sDs, &StreamDataStore::currentSelectedChangedSig, 
       propsGroup, &PlayerPropertiesGroup::currentClickedTrack);

   con = connect(sDs, &StreamDataStore::selectedStateChagedSig,
       playerStateModifier, &PlayerStateModifierGroup::laodCurrentState);

   con = connect(sDs, &StreamDataStore::frameRateChangedSig,
	   this, &AppMain::updateFrameRate);


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

void AppMain::setExtGuiControl(std::shared_ptr<ExternalGUIControlEvents> extGuiControl)
{
	this->extGuiControl = extGuiControl;
	QMetaObject::Connection con;

	con = connect(this->extGuiControl.get(), &ExternalGUIControlEvents::frameStoreSizeChanged,
		this, &AppMain::updateSeekerInterval, Qt::QueuedConnection);


    con = connect(this->extGuiControl.get(), &ExternalGUIControlEvents::seekerPositionChanged,
        this, &AppMain::updateSeekerPosition, Qt::QueuedConnection);
}

void AppMain::init()
{
    selectedSportingCode = ui->actionSoccer;
    QGraphicsScene* teamAScene = new QGraphicsScene();
    ui->teamAGView->setScene(teamAScene);
    teamAColor = new QGraphicsEllipseItemS(QRect(0, 0, 80, 80), QPen(QColorConstants::Gray), QBrush(QColorConstants::Gray));
    teamAScene->addItem(teamAColor);

    QGraphicsScene* teamBScene = new QGraphicsScene();
    ui->teamBGView->setScene(teamBScene);
    teamBColor = new QGraphicsEllipseItemS(QRect(0, 0, 80, 80), QPen(QColorConstants::Gray), QBrush(QColorConstants::Gray));
    teamBScene->addItem(teamBColor);

    outputHandle = new StdoutStreamBuffer(this);
    connect(outputHandle, &StdoutStreamBuffer::outputCaptured, this, &AppMain::appendOutput);

    scene = (QGraphicsScene*) new PitchViewScene();
    ui->graphicsView->setScene(scene);

    plotterController = new PlotterController(reinterpret_cast<PitchViewScene*>(scene)->getCurrentSceneRect(), this);

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

    storedEventsManager = new StoredEventsViewManager(ui->storedEventsListWidget, this);


    /**Select Current Sporting Code*/
    connect(ui->actionCricket, &QAction::triggered, this, [&]() {
        PitchViewScene* cS = static_cast<PitchViewScene*>(scene);
        cS->selectSportingCode(CRICKET);
        selectedSportingCode->setChecked(false);
        selectedSportingCode = ui->actionCricket;
        selectedSportingCode->setChecked(true);
        });

    connect(ui->actionSoccer, &QAction::triggered, this, [&]() {
        PitchViewScene* cS = static_cast<PitchViewScene*>(scene);
        cS->selectSportingCode(SOCCER);
        selectedSportingCode->setChecked(false);
        selectedSportingCode = ui->actionSoccer;
        selectedSportingCode->setChecked(true);
        });


    /** Plotted Player Control **/
    connect(ui->actionCreate_Player, &QAction::triggered, plotterController, &PlotterController::createPlayerItem);
    connect(plotterController, &PlotterController::playerCreatedSig, this, [&](int trackId, std::vector<double> coordinates) {
        if (systemEventsSender)
        {
            systemEventsSender->createPlayer(trackId, QPointF( coordinates[0], coordinates[1] ));
        }
	});
    connect(plotterController, &PlotterController::playerCreated, reinterpret_cast<PitchViewScene*>(scene), &PitchViewScene::plotPlayerSlot);
    connect(reinterpret_cast<PitchViewScene*>(scene), &PitchViewScene::itemPositionChanged, this, 
        [&](int trackId, QPointF itemCoordiantesNormalized) {
            if (systemEventsSender) 
            {
				systemEventsSender->playerPositionChanged(trackId, itemCoordiantesNormalized);
            }
     });

	connect(ui->actionClear_All_Plotted, &QAction::triggered, reinterpret_cast<PitchViewScene*>(scene), &PitchViewScene::clearAllPlotted);
	connect(reinterpret_cast<PitchViewScene*>(scene), &PitchViewScene::clearAllPlottedSig, plotterController, &PlotterController::clearAllPlotted);
	connect(plotterController, &PlotterController::clearAllPlottedPlayersSig, [&]() {
        if (systemEventsSender) 
        {
			systemEventsSender->clearAllPlottedPlayers();
        }
	});

    connect(ui->actionConfigTeams, &QAction::triggered, this, &AppMain::openTeamsConfigDialog);
    connect(ui->actionStart_Live_Data_Capture, &QAction::triggered, this, &AppMain::openEventProcessorDialog);
    connect(eventProcDialog, &EventProcessorDialog::event_processor_name, this, &AppMain::sendEventProcessorName);
    connect(eventProcDialog, &EventProcessorDialog::event_processor_name, storedEventsManager, &StoredEventsViewManager::addEvent);
    connect(storedEventsManager, &StoredEventsViewManager::loadEventSignal, this, &AppMain::switchToStoredStateSlot);
    connect(ui->actionPause_Output_Stream, &QAction::triggered, this, &AppMain::PauseOutputStreamTrigger);
    connect(ui->actionAdd_Distance, &QAction::triggered, this, [&]() {
		distanceDialog->setDistanceType(E_DISTANCE_TYPES::DYNAMIC_DISTANCE);
		openDistanceDialog();
    });
    connect(ui->live_mode_button, &QPushButton::clicked, this, &AppMain::setLiveMode);
    connect(ui->seeker_bar, &QSlider::sliderPressed, this, &AppMain::setReplayMode);
    connect(ui->seeker_bar, &QSlider::sliderReleased, this, &AppMain::setSeekerPosition);
    connect(ui->seeker_bar, &QSlider::sliderMoved, this, &AppMain::setSeekerPosition_1);
    connect(ui->replay_control, &QPushButton::clicked, this, &AppMain::replayControl);

    PitchViewScene* cS = static_cast<PitchViewScene*>(scene);
    connect(cS, &PitchViewScene::selectedIdChangedSig, distanceDialog, &DistanceDialog::selectedPlayer);

    /*Distance Preview Line Information*/
    connect(distanceObjectModel, &DistanceObjectModel::distancePreviewObjectReadySig, cS, &PitchViewScene::previewDistanceLineReady);
    connect(distanceObjectModel, &DistanceObjectModel::clearPreviewObject, cS, &PitchViewScene::clearPreviewLine);
    connect(distanceDialog, &DistanceDialog::previewDistanceDataReady, distanceObjectModel, &DistanceObjectModel::distancePreviewObjectReadySig);
    connect(distanceDialog, &DistanceDialog::clearDistancePreview, distanceObjectModel, &DistanceObjectModel::clearPreviewObject);

    /*Distance Lines to be drawn*/
    connect(distanceDialog, &DistanceDialog::distanceData, distanceObjectModel, &DistanceObjectModel::addDistanceObject);
    connect(distanceObjectModel, &DistanceObjectModel::distanceObjectsUpdatedSig, distanceObjectsGroup, &DistanceObjectsManager::addDistanceObject);
    connect(distanceObjectModel, &DistanceObjectModel::distanceObjectsUpdatedSig, cS, &PitchViewScene::addDistanceInfo);

    connect(distanceObjectModel, &DistanceObjectModel::computedDistanceUpdate, distanceObjectsGroup, &DistanceObjectsManager::computedDistanceUpdate);
    connect(distanceObjectsGroup, &DistanceObjectsManager::distanceObjectDeletedSignal, distanceObjectModel, &DistanceObjectModel::deleteDistanceObject);
    connect(distanceObjectModel, &DistanceObjectModel::deleteDistanceObjectSig, cS, &PitchViewScene::deleteDistanceLine);


    /*The connections here, handle data leaving the app going to the event-bus (Output)*/
    connect(distanceObjectModel, &DistanceObjectModel::distanceObjectsUpdatedSig, this, &AppMain::distanceDataChanged);
    connect(distanceObjectModel, &DistanceObjectModel::deleteDistanceObjectSig, this, &AppMain::deleteDistanceId);

    /*Fixed Distance Line*/
    connect(ui->actionAdd_Fixed_Distance, &QAction::triggered, this, [&]() {
		distanceDialog->setDistanceType(E_DISTANCE_TYPES::FIXED_DISTANCE);
		openDistanceDialog();
    });

    /**Configuring Teams Informations*/
    connect(teamsConfigDialog, &TeamsConfigManager::teamsConfigured, this, [&](QColor tAColor, std::string teamAName, QColor tBColor, std::string teamBName) {
        ui->teamAName->setText(QString::fromStdString(teamAName));
        ui->teamBName->setText(QString::fromStdString(teamBName));
        teamAColor->setBrush(tAColor);
        teamBColor->setBrush(tBColor);
        });

    connect(teamsConfigDialog, &TeamsConfigManager::teamAConfigured, this, [&](QColor tAColor, std::string teamAName) {
        ui->teamAName->setText(QString::fromStdString(teamAName));
        teamAColor->setBrush(tAColor);
        });


    connect(teamsConfigDialog, &TeamsConfigManager::teamBConfigured, this, [&](QColor tBColor, std::string teamBName) {
        ui->teamBName->setText(QString::fromStdString(teamBName));
        teamBColor->setBrush(tBColor);
    });


    setLiveMode();
}

void AppMain::openEventProcessorDialog()
{
    if (eventProcDialog)
    {
        eventProcDialog->open();
    }
}

void AppMain::openDistanceDialog()
{
   
	if (distanceDialog)
	{
		distanceDialog->show();
	}
}

void AppMain::openTeamsConfigDialog()
{
	if (teamsConfigDialog)
	{
		teamsConfigDialog->show();
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
   
	ui->playback_mode_text->setText("Live Mode");

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
    // We are coming from live mode, we need to switch to replay mode first 
    ui->playback_mode_text->setText("Replay Mode");
    if (liveMode)
    {
        KEvents::Event e;
        e.setEventData({ {"live_mode", false } });
        e.setSourceModule("gui");
        e.setEventName(EN_SET_STREAM_MODE);
        json config = KEvents::__load_config__();

        if (eventMan)
            eventMan->sendEvent(config["SportEventProcessor"]["serviceTopic"], e);
    }

	liveMode = false;
    sendSeekerEvent(sliderPosition);
}

void AppMain::setSeekerPosition_1(int position)
{
    int sliderPosition = position;
	sendSeekerEvent(sliderPosition);
}

void AppMain::setSeekerPosition()
{
    int sliderPosition = ui->seeker_bar->sliderPosition();
    sendSeekerEvent(sliderPosition);
}

void AppMain::__updateSeekerTracker(int seekerPos)
{
    if (seekerPos > currentSliderPosition)
    {
        seekingBack = false; // we are seeking forward
    }
    else {
		seekingBack = true; // we are seeking backward
    }

    currentSliderPosition = seekerPos;
}

void AppMain::sendSystemEvent(std::string topic, KEvents::Event e)
{
    if (systemEventsSender)
    {
        //systemEventsSender->
    }
}

void AppMain::updateStatusBarFrameCount()
{
  
}

void AppMain::sendSeekerEvent(int seekerPosition)
{
	__updateSeekerTracker(seekerPosition);

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

void AppMain::distanceDataChanged(json data)
{
    json stateData;
	stateData["state_def"] = KEvents::STATES_DEF::DISTANCE;
	stateData["data"] = data;
	stateData["data"]["id"] = data["objectId"];
    stateData["data"]["set"] = true;
	
    KEvents::Event e;
    e.setEventData(stateData)->
		setEventName(EN_STATE_MOD)->
		setSourceModule("gui")->
		setEventType(KEvents::E_GUI);
	json config = KEvents::__load_config__();

	if (eventMan)
	{
        std::cout << stateData << std::endl;
        eventMan->sendEvent(config["DataAggregator"]["serviceTopic"], e);
	}
    std::cout << "Distance information sent\n";
}

void AppMain::deleteDistanceId(int id)
{
    json stateData;
    stateData["state_def"] = KEvents::STATES_DEF::DISTANCE;
    stateData["data"] = json();
    stateData["data"]["id"] = id;
    stateData["data"]["set"] = false;

    KEvents::Event e;
    e.setEventData(stateData)->
        setEventName(EN_STATE_MOD)->
        setSourceModule("gui")->
        setEventType(KEvents::E_GUI);
    json config = KEvents::__load_config__();

    if (eventMan)
    {
        eventMan->sendEvent(config["DataAggregator"]["serviceTopic"], e);
    }
    std::cout << "Distance With ID: " << id << " deleted.\n";
}

void AppMain::outputEventData(std::string topic_, KEvents::Event e)
{
	if (eventMan)
	{
		eventMan->sendEvent(topic_, e);
	}
}
