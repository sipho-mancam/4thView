#include "AppMain.h"
#include <QtWidgets/QApplication>
#include <iostream>
#include <thread>
#include "kevents.hpp"
#include "stream_callback.hpp"
#include "stream_data_store.hpp"
#include "state_modification_callback.hpp"
#include "external_control_events_cb.hpp"
#include <Windows.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AppMain w;
    w.show();

    StreamDataStore streamDs;

    json globalConfig = KEvents::__load_config__();
    json systemConfig = globalConfig["systemSettings"];
    json moduleConfig = globalConfig["GUI"]; 

    std::string serviceName, serviceTopic;
    serviceName = moduleConfig["serviceName"];
    serviceTopic = moduleConfig["serviceTopic"];

    KEvents::EventsManager eventsManager(serviceTopic, serviceName);

    std::shared_ptr<StreamCallback> streamCb = KEvents::createCallback<StreamCallback>(globalConfig, serviceName);
    auto streamDataStore = std::bind(&StreamDataStore::dataCallback, &streamDs, std::placeholders::_1);
    streamCb->registerDataCallback(streamDataStore);
    w.setStreamDataStore(&streamDs);

    std::shared_ptr<StateModificationCb> stateMod = KEvents::createCallback<StateModificationCb>(globalConfig, serviceName);
    w.setStatePlayerStateModifier(stateMod);
    eventsManager.registerCallback(EN_STREAM_DATA_UPDATE, streamCb);
    eventsManager.registerCallback(EN_STATE_MOD, stateMod);

    std::shared_ptr<ExternalGUIControlEvents> extGuiControlCb = KEvents::createCallback<ExternalGUIControlEvents>();
    eventsManager.registerCallback(EN_FRAME_STORE_SIZE, extGuiControlCb);
    eventsManager.registerCallback(EN_SET_SEEKER_POSITION, extGuiControlCb);
    w.setExtGuiControl(extGuiControlCb);

    eventsManager.startEventLoop();
    w.setEventManager(&eventsManager);

    return a.exec();
}
