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

enum SPORTING_CODE
{
	CRICKET = 0,
	SOCCER = 1,
	RUGBY = 2,
	HOCKEY = 3,
	BASEBALL = 4
};

enum PLAYER_TYPE
{
	TRACKED = 0,
	PLOTTED = 1,
	PHANTOM = 2
};

enum PLAYER_MODIFIERS { // this modifiers add extra looks to the player without really affecting how they interact with the scene
	NONE_MODIFIER = 0xFFFF,
	KICKER_MODIFIER = 0x0001
};

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

	explicit PlayerItemWidget(int id, std::tuple<double, double> coord, PLAYER_TYPE pT = PLAYER_TYPE::TRACKED, QGraphicsItem* parent = nullptr);
	PlayerItemWidget() = delete;

	void init();

	void updateCoordinates(std::tuple<double, double>coord);
	void updateCoordinates(double x, double y);
	void updateGraphic();

	void setPosition(int pos) { playerPosition = pos; }
	void setState(E_STATE st) { state = st; }
	E_STATE getPlayerState() { return state; }
	int getTrackId() { return trackId; }	

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

	void setPlayerType(PLAYER_TYPE type) { playerType = type; }
	PLAYER_TYPE getPlayerType() { return playerType; }
	void setPlayerModifiers(PLAYER_MODIFIERS modifiers) { playerModifiers = modifiers; }

	virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
	
signals:
	void updateClickedId(int id);
	void itemDraggedSig(int id, QPointF itemSceneCoordinates);
	void itemPositionChanged(int itemId, QPointF itemPosition);

private:
	
	std::tuple<double, double> coordinates;
	QPointF currentPlayerPosition;
	PlayerItemWidget::E_STATE state;
	QGraphicsTextItem* idText;
	QGraphicsPolygonItem * triangle;
	int playerPosition;
	double width, height;
	int trackId;
	bool playerPositionChanged;
	PLAYER_TYPE playerType;
	PLAYER_MODIFIERS playerModifiers;
	
};


class PitchViewScene : public QGraphicsScene
{
	Q_OBJECT
public:
	PitchViewScene(QObject* parent=nullptr);
	PitchViewScene(QRect sceneRect);

	void init();
	void update();
	void updateFrameData(json frameData);
	void updateId(int id, json data);
	std::tuple<qreal, qreal> getDimensions() { return { _width, _height }; }

	void updateScene();
	void clearAllPlotted();
	void deletePlayer(int trackId);
	void plotPlayerSlot(PlayerItemWidget* player);
	void drawDistanceLines();
	QGraphicsLineItem* __drawDistanceLine__(QPointF start, QPointF end, QColor color);
	void addDistanceInfo(json distanceInfo);
	void deleteDistanceLine(long long id);
	void selectSportingCode(SPORTING_CODE code);
	QRect getCurrentSceneRect() { return _boundingRect; }


public slots:
	void dataChangeUpdate(json data);
	void selectedIdChanged(int trackId);
	void previewDistanceLineReady(json data);
	void clearPreviewLine();
	void itemDraggedCoordinatesSlot(int id, QPointF itemSceneCoordinates);
	void playerPositionChangedSlot(int trackId, QPointF playerSceneCoordiantes);

signals:
	void selectedIdChangedSig(int trackId);
	void itemDraggedSig(int id, QPointF itemSceneCoordinates);
	void itemPositionChanged(int itemId, QPointF itemPosition);
	void clearAllPlottedSig();

private:
	void __draw_cricket_background();
	void __draw_soccer_background();

private:
	qreal _width, _height; // bounding rect
	QRect _boundingRect, cricketBoundingRect;
	std::map<int, PlayerItemWidget*> playersMap;
	std::map<long long, QGraphicsLineItem*> distanceLines;
	QGraphicsLineItem* previewDistanceLine;
	json distancePreviewLine;
	std::map<long long, json> distanceObjects;
	std::vector<int> plottedIds;
	int currentClickedId;
};

QColor __state2color__(PlayerItemWidget::E_STATE);
QPen __state2pen__(PlayerItemWidget::E_STATE);
QColor __state2text__(PlayerItemWidget::E_STATE);