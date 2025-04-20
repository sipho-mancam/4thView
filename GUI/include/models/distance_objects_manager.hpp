#pragma once
#include <QtCore/QObject>
#include "distance_widget.h"
#include <QGraphicsEllipseItem>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

/**
* @brief
* This class manages the distance objects, by listening for 3 events
* 1. Init Add Distance,
* 2. Add Player 1
* 3. Add Player 2 
*/

class DistanceWidget : public QWidget
{
	Q_OBJECT
public:
	explicit DistanceWidget(int id, QWidget* parent = nullptr);
	~DistanceWidget();
	int getObjectId() { return objectId; }

	void setColor(QColor col);
	void setDistanceText(double d);

	void deletePressed();

signals:
	void distanceObjectDeleted(int objectId);

private:
	Ui::distance_widget* ui;
	int objectId;
	QGraphicsScene* iconScene;
	QGraphicsEllipseItem* distanceColorIcon;
};


class DistanceObjectsManager : public QWidget
{
	Q_OBJECT

public:
	explicit DistanceObjectsManager(QWidget* layoutObject,QScrollArea *scoll ,QWidget* parent = nullptr);
	void addDistanceObject(json distanceObject);

	void computedDistanceUpdate(json distanceObjects);
	
public slots:
	void distanceObjectDeleted(int objectId);

signals:
	void distanceObjectDeletedSignal(int objectId);
	
private:
	std::vector<DistanceWidget*> distanceObjects;
	QWidget* listLayout;
	QVBoxLayout* distanceList;
	QScrollArea* scrollArea;

	std::vector<QColor> colorsList;


};