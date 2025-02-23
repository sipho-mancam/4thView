#include "views/cricket_oval_widget.hpp"

#include <iostream>

CricketOvalScene::CricketOvalScene(QObject* parent)
	:
	QGraphicsScene(parent),
	_width(540),
	_height(500),
	_boundingRect(0, 0, _width, _height)
{
	setSceneRect(_boundingRect);
	init();
	/*this->addItem(new PlayerItemWidget(6, { 100, 100 }));*/
}

CricketOvalScene::CricketOvalScene(QRect sceneRect)
	:
	QGraphicsScene(sceneRect),
	_width(sceneRect.width()),
	_height(sceneRect.height()),
	_boundingRect(sceneRect)
{
	setSceneRect(_boundingRect);
	init();
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

void CricketOvalScene::dataChangeUpdate(json frameData)
{
	updateFrameData(frameData);
}

void CricketOvalScene::updateFrameData(json frameData)
{
	// check if the data has tracks
	if (frameData.contains("tracks"))
	{
		std::vector<json> tracks = frameData["tracks"];
		for (json& track : tracks)
		{
			if (track.contains("track_id"))
			{
				auto trackId = track["track_id"];
				// we need to scale up the coordinates data before we send it through.
				auto coords = track["coordinates"];
				double x = coords[0], y = coords[1], xScaled, yScaled;
				xScaled = x * _boundingRect.width()/2;
				yScaled = y * _boundingRect.height()/2;
				track["coordinates"] = std::vector<double>({ xScaled, yScaled });
				this->updateId(trackId, track);
			}
		}
	}
}

void CricketOvalScene::updateId(int id, json data)
{
	// find out if the object already exists
	if (playersMap.contains(id))
	{
		PlayerItemWidget* player = playersMap[id];
		// get state
		if (data.contains("state"))
		{
			PlayerItemWidget::E_STATE state = (PlayerItemWidget::E_STATE)data["state"];
			//std::cout << "State for player ID: " << id << " Changed \n";
			player->setState(state);
		}
		if (data.contains("coordinates"))
		{
			auto coordinates = data["coordinates"];
			player->updateCoordinates({ coordinates[0], coordinates[1] });
		}

		if (data.contains("position"))
		{
			/*PlayerItemWidget::E_POSITION pos = (PlayerItemWidget::E_POSITION)data["position"];
			player->setPosition(pos);*/
		}
		player->updateGraphic();
	}
	else { // we need to create the object for this new ID.
		if (!data.contains("coordinates"))
			return;

		auto coordinates = data["coordinates"];
		PlayerItemWidget* player = new PlayerItemWidget(id, { coordinates[0], coordinates[1]});
		this->addItem(player);
		playersMap[id] = player;
		connect(player, &PlayerItemWidget::updateClickedId, this, &CricketOvalScene::selectedIdChanged);
	}
}

void CricketOvalScene::selectedIdChanged(int trackId)
{
	emit selectedIdChangedSig(trackId);
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
	this->setRect(rect);
	this->setBrush(QBrush(__state2color__(state)));
	this->setPen(__state2pen__(state));
	this->setPos(x, y);

	idText = new QGraphicsTextItem(this);
	std::string idTextStr;
	std::ostringstream oss;

	QFont font;
	font.setPointSize(10);
	font.setBold(true);
	oss << std::setw(2) << std::setfill('0') << std::to_string(trackId);
	idTextStr = oss.str();

	idText->setPlainText(QString(const_cast<char*>(idTextStr.c_str())));
	idText->setFont(font);
	idText->setZValue(this->zValue() + 2);
	idText->setDefaultTextColor(__state2text__(state));
	idText->setPos(x-2, y-3);
}

void PlayerItemWidget::updateCoordinates(std::tuple<double, double> coord)
{
	coordinates = coord;	
	updateGraphic();
}

void PlayerItemWidget::updateCoordinates(double x, double y)
{
	coordinates = { x, y };
	updateGraphic();
}

void PlayerItemWidget::updateGraphic()
{
	const auto [x, y] = coordinates;
	idText->setDefaultTextColor(__state2text__(state));
	
	this->prepareGeometryChange();
	this->setPos(x, y);
	this->setBrush(QBrush(__state2color__(state)));
	this->setPen(__state2pen__(state));
	
}

bool PlayerItemWidget::operator==(int& trackId)
{
	return (trackId == this->trackId);
}

void PlayerItemWidget::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	/*std::cout << "You clicked : " << trackId << "\n";
	std::cout << "X : " << this->pos().x() << "Y: " << this->pos().y() <<"\n";*/
	emit updateClickedId(this->trackId);
	state = E_STATE::SELECTED;
	updateGraphic();
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
		return QColorConstants::Color1;
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
		return QPen(QColorConstants::Blue, thickness);
	}
	else if (state == PlayerItemWidget::E_STATE::ANNOTATED)
	{
		return QPen(QColorConstants::Yellow, thickness);
	}
	return QPen(QColorConstants::Gray);
}

QColor __state2text__(PlayerItemWidget::E_STATE state)
{
	if (state == PlayerItemWidget::E_STATE::DEFAULT)
	{
		return QColorConstants::Black;
	}
	else if (state == PlayerItemWidget::E_STATE::SELECTED)
	{
		return QColorConstants::Black;
	}
	else if (state == PlayerItemWidget::E_STATE::DESELECTED)
	{
		return QColorConstants::Black;
	}
	else if (state == PlayerItemWidget::E_STATE::HIGHLIGHTED)
	{
		return QColorConstants::Black;
	}
	else if (state == PlayerItemWidget::E_STATE::ANNOTATED)
	{
		return QColorConstants::White;
	}

	return QColorConstants::Black;
	
}
