#include "models/stored_events_manager.hpp"

StoredEventsViewManager::StoredEventsViewManager(QListWidget* listWidget, QObject * parent)
	:QObject(parent),
	currentItem(nullptr), eventMap(), loadedEvent(false)
{
	this->listWidget = listWidget;
	connect(this->listWidget, &QListWidget::currentItemChanged, this, &StoredEventsViewManager::currentSelectedItemChangedSlot);
	connect(this->listWidget, &QListWidget::itemPressed, this, &StoredEventsViewManager::ItemPressedSlot);
}

StoredEventsViewManager::~StoredEventsViewManager()
{
}

void StoredEventsViewManager::addEvent(json event)
{
	if(!event.contains("sport_event_name"))
		return;

	if (eventMap.contains(event["sport_event_name"]))
		return;

	std::string evtName = event["sport_event_name"];
	eventMap[evtName] = true;
	QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(evtName));
	item->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::FolderOpen));
	
	listWidget->addItem(item);
	listWidget->scrollToBottom();
	
}

void StoredEventsViewManager::currentSelectedItemChangedSlot(QListWidgetItem* newItem, QListWidgetItem* oldItem)
{
	if (newItem != currentItem)
	{
		loadedEvent = false;
	}
}

void StoredEventsViewManager::ItemPressedSlot(QListWidgetItem* item)
{
	if (!loadedEvent)
	{
		std::string evtName = item->text().toStdString();
		std::cout << "Current Clicked Event is: " << evtName << std::endl;
		std::string msg = "Are you sure you want to load event: " + evtName + " ?";
		QMessageBox::StandardButton btn = QMessageBox::information(nullptr, "Load Event", QString::fromStdString(msg), QMessageBox::StandardButton::Ok, QMessageBox::StandardButton::Cancel);
		if (btn == QMessageBox::StandardButton::Ok)
		{
			/*Switch Playback Mode to Replay Mode*/
			/*Load Event data From Disk*/
			/*Load Event Data to replay Buffer and trigger play*/
			currentItem = item;
			loadedEvent = true;
		}
		else if (btn == QMessageBox::StandardButton::Cancel)
		{
			std::cout << "Ignore Event, user cancelled the message" << std::endl;
		}
	}
}
