#pragma once
#include <QtWidgets/QGraphicsItem>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QWidget>




class PlayerItemWidget : public QGraphicsEllipseItem
{
public:
	enum E_STATE
	{
		DEFAULT = 0,
		DESELECTED = 1,
		SELECTED = 2,
		HIGHLIGHTED = 3,
		ANNOTATED = 4
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

	void update(std::tuple<double, double>coord);
	void update(double x, double y);
	void updateGraphic();

	void setPosition(E_POSITION pos) { playerPosition = pos; }
	void setState(E_STATE st) { state = st; }

	bool operator== (int& trackId);
	PlayerItemWidget& operator=(std::tuple<double, double> coord)
	{
		const auto [x, y] = coord;
		coordinates = coord;
		this->update(coord);
		return std::ref(*this);
	}

	PlayerItemWidget& operator=(int id)
	{
		this->trackId = id;
		return std::ref(*this);
	}

	virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);

private:
	int trackId;
	std::tuple<double, double> coordinates;
	PlayerItemWidget::E_STATE state;
	E_POSITION playerPosition;
	QGraphicsEllipseItem* ellipse;
	double width, height;
};


class CricketOvalScene : public QGraphicsScene, public QWidget
{
public:
	CricketOvalScene(QObject* parent=nullptr);
	CricketOvalScene(QRect sceneRect);

	void init();
	void update();
private:
	qreal _width, _height; // bounding rect
	QRect _boundingRect;

};

QColor __state2color__(PlayerItemWidget::E_STATE);
QPen __state2pen__(PlayerItemWidget::E_STATE);