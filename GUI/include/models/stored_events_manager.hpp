#pragma once
#include <QListWidget>
#include <QWidget>
#include <nlohmann/json.hpp>
#include <iostream>
#include <map>
#include <QMessageBox>

using json = nlohmann::json;


class StoredEventsViewManager : public QObject
{
	Q_OBJECT

public:
	StoredEventsViewManager(QListWidget* listWidget, QObject* parent);
	~StoredEventsViewManager();
	void addEvent(json event);

	bool isNameTaken(std::string name)
	{
		return eventMap.contains(name);
	}

	void currentSelectedItemChangedSlot(QListWidgetItem* newItem, QListWidgetItem* oldItem);
	void ItemPressedSlot(QListWidgetItem* item);

signals:
	void loadEventSignal(std::string eventName);

private:
	QListWidget* listWidget;
	std::map<std::string, bool> eventMap;
	bool loadedEvent;
	QListWidgetItem* currentItem;
	
};