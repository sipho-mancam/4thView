#include "views/cricket_oval_widget.hpp"

#include <iostream>

CricketOvalScene::CricketOvalScene(QObject* parent)
	:QGraphicsScene(parent),
	QWidget(static_cast<QWidget*>(parent)),
	_width(560),
	_height(520),
	_boundingRect(0, 0, _width, _height)
{
	init();

	this->addItem(new PlayerItemWidget(10, { 20, 40 }));
}

CricketOvalScene::CricketOvalScene(QRect sceneRect)
	:QGraphicsScene(sceneRect),
	QWidget(),
	_width(sceneRect.width()),
	_height(sceneRect.height()),
	_boundingRect(sceneRect)
{
	init();
	this->addItem(new PlayerItemWidget(10, { 20, 40 }));
}

void CricketOvalScene::init()
{
	int x, y, w, h;
	x = _boundingRect.x();
	y = _boundingRect.y();
	w = _boundingRect.width();
	h = _boundingRect.height();
	QGraphicsEllipseItem* greenEllipse = this->addEllipse(
		_boundingRect,
		QPen(QColorConstants::White),
		QBrush(QColorConstants::DarkGreen)
	);

	float scale = 0.95f;
	QGraphicsEllipseItem* boundaryLine = this->addEllipse(
		QRect(x + ((1 - scale)/2) * w, y + ((1 - scale)/2) * h, scale * w, scale * h),
		QPen(QColorConstants::White, 3)
	);

	int rWidth, rHeight;
	rWidth = w * 0.15; rHeight = h * 0.3;
	QColor rectColor(234, 227, 201);
	QGraphicsRectItem* pitchRect = this->addRect(
		QRect(w / 2 - rWidth/2, h / 2 - rHeight / 2, rWidth, rHeight),
		QPen(rectColor),
		QBrush(rectColor)
	);

	QFont font;
	font.setBold(true);
	font.setPointSizeF(40);
	int lineHeight = 40;

	QGraphicsTextItem* text = this->addText(QString("Leg"), font);
	QRectF r = text->boundingRect();
	text->setPos(w/2 - rWidth-100, h/2 - lineHeight);

	text = this->addText(QString("Off"), font);
	text->setPos(w/2 + rWidth, h/2 - lineHeight);
}

void CricketOvalScene::update()
{
}


PlayerItemWidget::PlayerItemWidget(int id, std::tuple<double, double> coord, QGraphicsItem* parent)
	:QGraphicsEllipseItem(parent), 
	trackId(id),
	coordinates(coord),
	state(E_STATE::DEFAULT),
	playerPosition(E_POSITION::FIELDER),
	width(20), height(20)
{
	const auto [x, y] = coordinates;
	QRect rect(x, y, width, height);
	//this->prepareGeometryChange();
	this->setRect(rect);
	this->setBrush(QBrush(__state2color__(state)));
	this->setPen(__state2pen__(state));

}

void PlayerItemWidget::update(std::tuple<double, double> coord)
{
	const auto [x, y] = coord;
	this->setPos(x, y);
	coordinates = coord;
}

void PlayerItemWidget::update(double x, double y)
{
	coordinates = { x, y };
	this->setPos(x, y);
}

void PlayerItemWidget::updateGraphic()
{
}

bool PlayerItemWidget::operator==(int& trackId)
{
	return (trackId == this->trackId);
}

void PlayerItemWidget::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	std::cout << "You clicked : " << trackId << "\n" << std::endl;
}

QColor __state2color__(PlayerItemWidget::E_STATE state)
{
	if (state == PlayerItemWidget::E_STATE::DEFAULT)
	{
		return QColorConstants::Gray;
	}
	else if (state == PlayerItemWidget::E_STATE::SELECTED)
	{
		return QColorConstants::Yellow;
	}
	else if (state == PlayerItemWidget::E_STATE::DESELECTED)
	{
		return QColorConstants::Gray;
	}
	else if (state == PlayerItemWidget::E_STATE::HIGHLIGHTED)
	{
		return QColorConstants::Green;
	}
	else if (state == PlayerItemWidget::E_STATE::ANNOTATED)
	{
		return QColorConstants::Blue;
	}
	
	return QColorConstants::Gray;
}

QPen __state2pen__(PlayerItemWidget::E_STATE state)
{
	int thickness = 3;
	if (state == PlayerItemWidget::E_STATE::DEFAULT)
	{
		return QPen(QColorConstants::Gray);
	}
	else if (state == PlayerItemWidget::E_STATE::SELECTED)
	{
		return QPen(QColorConstants::Blue, thickness);
	}
	else if (state == PlayerItemWidget::E_STATE::DESELECTED)
	{
		return QPen(QColorConstants::Gray, 1);
	}
	else if (state == PlayerItemWidget::E_STATE::HIGHLIGHTED)
	{
		return QPen(QColorConstants::Yellow, thickness);
	}
	else if (state == PlayerItemWidget::E_STATE::ANNOTATED)
	{
		return QPen(QColorConstants::Yellow, thickness);
	}
	return QPen(QColorConstants::Gray);
}
