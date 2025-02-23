#pragma once
#include "kevents.hpp"
#include <QtCore/QObject>

class StateModificationCb : public QObject, public KEvents::CallBackBase
{
	Q_OBJECT

public:
	StateModificationCb(json globalConfig, std::string serviceName, QObject* parent = (nullptr));
	virtual void execute(KEvents::Event e) override;

public slots:
	void playerStateChanged(json stateObject);

private:
	json globalConfig;
	std::string serviceName, processor;

};