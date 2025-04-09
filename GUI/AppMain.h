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

QT_BEGIN_NAMESPACE
namespace Ui { class AppMainClass; };
QT_END_NAMESPACE

class StateModificationCb;
// Custom stream buffer to capture stdout
 class StdoutStreamBuffer  : public QObject, public std::streambuf{
    Q_OBJECT
public:
    StdoutStreamBuffer(QObject* parent = nullptr) : m_parent(parent) {
        // Save the old buffer for stdout
        oldBuffer = std::cout.rdbuf(this);
    }

    ~StdoutStreamBuffer() {
        // Restore the old buffer
        std::cout.rdbuf(oldBuffer);
    }

protected:
    // Override the overflow method to capture output
    int_type overflow(int_type v) override {
        if (v == traits_type::eof()) {
            return traits_type::eof();
        }

        char ch = static_cast<char>(v);
        emitOutput(QString(ch));
        return v;
    }

    std::streamsize xsputn(const char* p, std::streamsize n) override {
        QString str = QString::fromLocal8Bit(p, n);
        emitOutput(str);
        return n;
    }

private:
    void emitOutput(const QString& text) {
        // Get the current timestamp
        QString timestamp = QDateTime::currentDateTime().toString("[dd:MM:yyyy][HH:mm:ss]");

        // Define the system name
        QString systemName = "[GUI]";

        // Format the output
        QString formattedOutput = timestamp + systemName + text;
        // Emit a signal with the captured text
        Q_EMIT outputCaptured(text);
    }

signals:
    void outputCaptured(const QString& text);

private:
    std::streambuf* oldBuffer;
    QObject* m_parent;
};


class AppMain : public QMainWindow
{
    Q_OBJECT

public:
    AppMain(QWidget *parent = nullptr);
    ~AppMain();

    virtual void closeEvent(QCloseEvent* e) override;
    void setEventManager(KEvents::EventsManager* evMan) { eventMan = evMan; }
    void setStreamDataStore(StreamDataStore* sDs);
    void setStatePlayerStateModifier(std::shared_ptr<StateModificationCb> stMod);
	void setExtGuiControl(std::shared_ptr<ExternalGUIControlEvents> extGuiControl);

    void openEventProcessorDialog();
	void openDistanceDialog();
    void PauseOutputStreamTrigger();
    void setLiveMode();
    void setReplayMode();
	void setSeekerPosition_1(int position=-1);
	void setSeekerPosition();
	void __updateSeekerTracker(int seekerPos);

    void updateStatusBarFrameCount();

    void sendSeekerEvent(int seekerPosition);
    void replayControl();

private slots:
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

private:
    Ui::AppMainClass *ui;
    QGraphicsScene* scene;
    PlayerPropertiesGroup* propsGroup;
    PlayerStateModifierGroup* playerStateModifier;
    DistanceObjectsManager* distanceObjectsGroup;
    EventProcessorDialog* event_proc_dialog;
	DistanceDialog* distance_dialog;
	std::shared_ptr<ExternalGUIControlEvents> extGuiControl;

    StdoutStreamBuffer* outputHandle;
    KEvents::EventsManager* eventMan;
    QIcon playIcon, pauseIcon;
    bool currentIcon, replayPaused, liveMode, seekingBack;

	DistanceObjectModel* distanceObjectModel;
    StreamDataStore* streamDs;
    std::shared_ptr<StateModificationCb> stateMod;
    int currentSliderPosition;
};
