#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_AppMain.h"
#include <iostream>
#include <QDateTime>
#include <QtWidgets/QMessageBox>
#include "kevents.hpp"
#include "stream_data_store.hpp"
#include "views/cricket_oval_widget.hpp"
#include "views/player_state_mod.hpp"
#include "models/distance_objects_manager.hpp"
#include "models/event_processor_dialog.hpp"
#include "models/distance_dialog.hpp"
#include "external_control_events_cb.hpp"
#include "time_conversions.hpp"
#include "models/distance_object_model.hpp"
#include "models/stored_events_manager.hpp"
#include "models/teams_config_dialog.hpp"
#include "plotter_controller.hpp"
#include "stream_buffer_manipulator.hpp"
#include "system_events_sender.hpp"
#include "models/FreeKickDialog.hpp"
#include <QButtonGroup>

QT_BEGIN_NAMESPACE
namespace Ui { class AppMainClass; };
QT_END_NAMESPACE

class StateModificationCb;

class AppMain : public QMainWindow
{
    Q_OBJECT

public:
    AppMain(QWidget *parent = nullptr);
    ~AppMain();
    inline void init();
    virtual void closeEvent(QCloseEvent* e) override;

    void setEventManager(KEvents::EventsManager* evMan); 
    void setStreamDataStore(StreamDataStore* sDs);
    void setStatePlayerStateModifier(std::shared_ptr<StateModificationCb> stMod);
	void setExtGuiControl(std::shared_ptr<ExternalGUIControlEvents> extGuiControl);
    void openEventProcessorDialog();
	void openDistanceDialog();
	void openTeamsConfigDialog();
    void PauseOutputStreamTrigger();
    void setLiveMode();
    void setReplayMode();
	void setSeekerPosition_1(int position=-1);
	void setSeekerPosition();
	void __updateSeekerTracker(int seekerPos);
	void sendSystemEvent(std::string topic, KEvents::Event e);
    void updateStatusBarFrameCount();
    void sendSeekerEvent(int seekerPosition);
    void replayControl();

private:
    Ui::AppMainClass* ui;
    QGraphicsScene* scene;
    PlayerPropertiesGroup* propsGroup;
    PlayerStateModifierGroup* playerStateModifier;
    DistanceObjectsManager* distanceObjectsGroup;
    EventProcessorDialog* eventProcDialog;
    QGraphicsEllipseItemS* teamAColor, * teamBColor;
    StoredEventsViewManager* storedEventsManager;
    DistanceDialog* distanceDialog;
    TeamsConfigManager* teamsConfigDialog;
    std::shared_ptr<ExternalGUIControlEvents> extGuiControl;
    QAction* selectedSportingCode;
    StdoutStreamBuffer* outputHandle;
    KEvents::EventsManager* eventMan;
    QIcon playIcon, pauseIcon;
    bool currentIcon, replayPaused, liveMode, seekingBack;
    DistanceObjectModel* distanceObjectModel;
    StreamDataStore* streamDs;
    std::shared_ptr<StateModificationCb> stateMod;
    int currentSliderPosition;
    PlotterController* plotterController;
	SystemEventsSender* systemEventsSender;
    FreeKickSideDialog* freeKickDialog;
	QButtonGroup* teamsRadioGroup, * skinColorRadioGroup;

private slots:

    void outputEventData(std::string topic_, KEvents::Event e);
    void distanceDataChanged(json data);
    void deleteDistanceId(int id);

    void appendOutput(const QString& text)
    {
        ui->plainTextEdit->moveCursor(QTextCursor::End);
        ui->plainTextEdit->insertPlainText(text);
    }

    void updateSeekerPosition(int seekerPos)
    {
		if (!liveMode)
		{
            ui->seeker_bar->setSliderPosition(seekerPos);
			std::string timeString = Int2TimeString::int2TimeString(seekerPos);
			ui->elapsed_time->setText(QString::fromStdString(timeString));
        }
        else {
            std::string timeString = Int2TimeString::int2TimeString(ui->seeker_bar->maximum());
            ui->elapsed_time->setText(QString::fromStdString(timeString));
        }
    }

    void updateSeekerInterval(int storeSize)
    {
        if(storeSize < 10)
			storeSize = 10;

		ui->seeker_bar->setMinimum(0);
        ui->seeker_bar->setMaximum(storeSize);

        if(liveMode)
			ui->seeker_bar->setSliderPosition(storeSize);

        std::string timeString = Int2TimeString::int2TimeString(storeSize);
		ui->max_replay_time->setText(QString::fromStdString(timeString));
    }

    void updateFrameRate(double frameRate)
    {
        if(!ui->frame_rate_update->checkOverflow(frameRate))
            ui->frame_rate_update->display(frameRate);
    }

    void sendEventProcessorName(json eventProcData)
    {
      
        if (eventMan)
        {
            KEvents::Event e;
            e.setEventData(eventProcData)->
            setEventName(EN_STATE_CAPTURE_START)->
            setSourceModule("gui")->
            setEventType(KEvents::E_GUI);
            json config = KEvents::__load_config__();
            eventMan->sendEvent(config["Processor"]["serviceTopic"], e);
        }
    }


	void switchToStoredStateSlot(std::string eventName)
	{
		if (eventMan)
		{
			KEvents::Event e;
			e.setEventData({ {"sport_event_name", eventName} })->
				setEventName(EN_LOAD_STORED_STATE)->
				setSourceModule("gui")->
				setEventType(KEvents::E_GUI);
			json config = KEvents::__load_config__();
			eventMan->sendEvent(config["SportEventProcessor"]["serviceTopic"], e);
		}
	}


};
