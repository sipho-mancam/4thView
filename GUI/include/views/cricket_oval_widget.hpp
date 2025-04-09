#pragma once
#include <QtWidgets/QGraphicsItem>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QWidget>
#include <iomanip>
#include <string>
#include <sstream>
#include <nlohmann/json.hpp>
#include "player_props_widget.hpp"
#include "kevents.hpp"

using json = nlohmann::json;

class PlayerItemWidget : public QObject, public QGraphicsEllipseItem
{
	Q_OBJECT

public:
	enum E_STATE
	{
		DEFAULT = 0,
		DESELECTED = 4,
		SELECTED = 3,
		ANNOTATED = 1,
		HIGHLIGHTED = 2,
	};

	enum E_POSITION {
		FIELDER = 0,
		BOWLER,
		BATTER_FACING,
		BATTER,
		UMPIRE
	};

	explicit PlayerItemWidget(int id, std::tuple<double, double> coord, QGraphicsItem* parent = nullptr);
	PlayerItemWidget() = delete;

	void updateCoordinates(std::tuple<double, double>coord);
	void updateCoordinates(double x, double y);
	void updateGraphic();

	void setPosition(E_POSITION pos) { playerPosition = pos; }
	void setState(E_STATE st) { state = st; }

	std::tuple<double, double> getCoordinates() { return coordinates; }

	bool operator== (int& trackId);
	PlayerItemWidget& operator=(std::tuple<double, double> coord)
	{
		const auto [x, y] = coord;
		coordinates = coord;
		this->updateCoordinates(coord);
		return std::ref(*this);
	}

	PlayerItemWidget& operator=(int id)
	{
		this->trackId = id;
		return std::ref(*this);
	}

	virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);

signals:
	void updateClickedId(int id);

private:
	int trackId;
	std::tuple<double, double> coordinates;
	PlayerItemWidget::E_STATE state;
	QGraphicsTextItem* idText;
	E_POSITION playerPosition;
	double width, height;
};


class CricketOvalScene : public QGraphicsScene
{
	Q_OBJECT

public:
	CricketOvalScene(QObject* parent=nullptr);
	CricketOvalScene(QRect sceneRect);

	void init();
	void update();
	void updateFrameData(json frameData);
	void updateId(int id, json data);
	std::tuple<qreal, qreal> getDimensions() { return { _width, _height }; }
	void updateScene();

	void drawDistanceLines();
	QGraphicsLineItem* __drawDistanceLine__(std::tuple<double, double> start, std::tuple<double, double> end, QColor color);

public slots:
	void dataChangeUpdate(json data);
	void selectedIdChanged(int trackId);
	void previewDistanceLineReady(json data);
	void clearPreviewLine();

signals:
	void selectedIdChangedSig(int trackId);

private:
	qreal _width, _height; // bounding rect
	QRect _boundingRect;
	std::map<int, PlayerItemWidget*> playersMap;
	QGraphicsLineItem* previewDistanceLine;
	json distancePreviewLine;
};

QColor __state2color__(PlayerItemWidget::E_STATE);
QPen __state2pen__(PlayerItemWidget::E_STATE);
QColor __state2text__(PlayerItemWidget::E_STATE);