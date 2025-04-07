#pragma once
#include "kevents.hpp"
#include <QtCore/QObject>


class ExternalGUIControlEvents : public QObject, public KEvents::CallBackBase
{
	Q_OBJECT

public:
	ExternalGUIControlEvents(QObject* parent = nullptr);
	~ExternalGUIControlEvents() = default;
	virtual void execute(KEvents::Event e) override;

signals:
	void seekerPositionChanged(int seekerPosition);
	void frameStoreSizeChanged(int frameStoreSize);

private:
	json globalConfig, moduleConfig;

};