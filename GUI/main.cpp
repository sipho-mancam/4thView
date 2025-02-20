#include "AppMain.h"
#include <QtWidgets/QApplication>
#include <iostream>
#include <thread>
#include "kevents.hpp"
#include "stream_callback.hpp"
#include "stream_data_store.hpp"
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

    eventsManager.registerCallback(EN_STREAM_DATA_UPDATE, streamCb);
    eventsManager.startEventLoop();

    w.setEventManager(&eventsManager);

    return a.exec();
}
