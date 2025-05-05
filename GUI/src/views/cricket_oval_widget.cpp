#include "views/cricket_oval_widget.hpp"
#include <QGraphicsSceneDragDropEvent>
#include <iostream>

PitchViewScene::PitchViewScene(QObject* parent)
	:
	QGraphicsScene(parent),
	_width(540),
	_height(500),
	_boundingRect(0, 0, _width, _height),
	cricketBoundingRect(_boundingRect)
{
	//setSceneRect(_boundingRect);
	init();
}

PitchViewScene::PitchViewScene(QRect sceneRect)
	:
	QGraphicsScene(sceneRect),
	_width(sceneRect.width()),
	_height(sceneRect.height()),
	_boundingRect(sceneRect)
{
	//setSceneRect(_boundingRect);
	init();
}

void PitchViewScene::init()
{
	currentClickedId = -1;
	__draw_soccer_background();
}

void PitchViewScene::update()
{
}

void PitchViewScene::dataChangeUpdate(json frameData)
{
	updateFrameData(frameData);
}

void PitchViewScene::updateFrameData(json frameData)
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
				try {
					//std::cout << track << std::endl;
					// we need to scale up the coordinates data before we send it through.
					auto coords = track["coordinates"];
					double x = coords[0], y = coords[1], xScaled, yScaled;
					xScaled = x * _boundingRect.width();
					yScaled = y * _boundingRect.height();
					track["coordinates"] = std::vector<double>({ xScaled, yScaled });
					this->updateId(trackId, track);
				}
				catch (json::type_error& pe) {
					// These are invalid tracks, just ignore the error and move on
				}
				catch (std::exception& e) {
					
				}	
			}
		}
	}
	updateScene();
}

void PitchViewScene::updateId(int id, json data)
{
	// find out if the object already exists
	if (playersMap.contains(id))
	{
		PlayerItemWidget* player = playersMap[id];
		// get state
		if (data.contains("state"))
		{
			PlayerItemWidget::E_STATE state = (PlayerItemWidget::E_STATE)data["state"];
			player->setState(state);
		}
		if (data.contains("coordinates"))
		{
			auto coordinates = data["coordinates"];
			player->updateCoordinates({ coordinates[0], coordinates[1] });
		}

		if (data.contains("position"))
		{
			int pos = data["position"]["position"];
			player->setPosition(pos);
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
		connect(player, &PlayerItemWidget::updateClickedId, this, &PitchViewScene::selectedIdChanged);
	}
}

void PitchViewScene::updateScene()
{
	// check if we have distance preview information
	drawDistanceLines();
	for (auto item : playersMap)
	{
		item.second->updateGraphic();
	}
}

void PitchViewScene::clearAllPlotted()
{
	for (int id : plottedIds)
	{
		if (playersMap.contains(id))
		{
			PlayerItemWidget* player = playersMap[id];
			this->removeItem(player);
			playersMap.erase(id);
		}
	}
	updateScene();
	Q_EMIT clearAllPlottedSig();
}

void PitchViewScene::deletePlayer(int trackId)
{
	if (playersMap.contains(trackId))
	{
		PlayerItemWidget* player = playersMap[trackId];
		this->removeItem(player);
		playersMap.erase(trackId);
		plottedIds.erase(std::remove(plottedIds.begin(), plottedIds.end(), trackId), plottedIds.end());
		//delete player;
	}
}

void PitchViewScene::plotPlayerSlot(PlayerItemWidget* player)
{
	int id = player->getTrackId();
	this->addItem(player);
	playersMap[id] = player;
	connect(player, &PlayerItemWidget::updateClickedId, this, &PitchViewScene::selectedIdChanged);
	connect(player, &PlayerItemWidget::itemDraggedSig, this, &PitchViewScene::itemDraggedSig);
	connect(player, &PlayerItemWidget::itemPositionChanged, this, &PitchViewScene::playerPositionChangedSlot);
	plottedIds.push_back(id);
}

void PitchViewScene::drawDistanceLines()
{
	// Check if we have a preview line to draw
	if (!distancePreviewLine.empty())
	{
		int previewP1 = distancePreviewLine["player1"], previewP2 = distancePreviewLine["player2"];
		QPointF p1, p2;
		bool p1Found = false, p2Found = false;
		auto lineColor = distancePreviewLine["lineColor"];

		for (auto item : playersMap)
		{
			if (item.first == previewP1)
			{
				p1 = item.second->mapToScene(item.second->boundingRect().center());
				p1Found = true;
			}
			else if (item.first == previewP2)
			{
				p2 = item.second->mapToScene(item.second->boundingRect().center());
				p2Found = true;
			}

			if (p1Found && p2Found)
				break;
		}
		if (p1Found && p2Found)
		{
			if (previewDistanceLine)
				this->removeItem(previewDistanceLine);
			else
				previewDistanceLine = nullptr;

			previewDistanceLine = __drawDistanceLine__(p1, p2, QColor(lineColor[0], lineColor[1], lineColor[2]));
			previewDistanceLine->setZValue(1);
		}	
	}

	if (!distanceObjects.empty())
	{
		for (auto item : distanceObjects)
		{
			auto distanceInfo = item.second;
			int p1 = distanceInfo["player1"], p2 = distanceInfo["player2"];
			QPointF p1Coord, p2Coord;
			bool p1Found = false, p2Found = false;
			auto lineColor = distanceInfo["lineColor"];
			for (auto item : playersMap)
			{
				if (item.first == p1)
				{
					p1Coord = item.second->mapToScene(item.second->boundingRect().center());
					p1Found = true;
				}
				else if (item.first == p2)
				{
					p2Coord = item.second->mapToScene(item.second->boundingRect().center());
					p2Found = true;
				}
				if (p1Found && p2Found)
					break;
			}
			if (p1Found && p2Found)
			{
				if (distanceLines.contains(item.first))
					this->removeItem(distanceLines[item.first]);
				else
					distanceLines[item.first] = nullptr;

				distanceLines[item.first] = __drawDistanceLine__(p1Coord, p2Coord, QColor(lineColor[0], lineColor[1], lineColor[2]));
				distanceLines[item.first]->setZValue(0);
			}
		}
	}
}

QGraphicsLineItem* PitchViewScene::__drawDistanceLine__(QPointF start, QPointF end, QColor color)
{
	int offset = 10;
	QGraphicsLineItem* line = addLine(QLineF(start, end));
	line->setPen(QPen(color, 3));
	return line;
}

void PitchViewScene::addDistanceInfo(json distanceInfo)
{
	long long objectId = distanceInfo["objectId"];
	distanceObjects[objectId] = distanceInfo;
	std::cout << "Distance Object with ID: " << distanceInfo["objectId"] << " is added to the map." << std::endl;
	
}

void PitchViewScene::deleteDistanceLine(long long id)
{
	if (distanceObjects.contains(id))
	{
		distanceObjects.erase(id);
	}
	if (distanceLines.contains(id))
	{
		auto line = distanceLines[id];
		this->removeItem(line);
		distanceLines.erase(id);

	}
	std::cout << "Distance Object with ID: " << id << " deleted.\n";
}

void PitchViewScene::selectSportingCode(SPORTING_CODE code)
{
	if (code == CRICKET)
	{
		__draw_cricket_background();
	}
	else if (code == SOCCER)
	{
		__draw_soccer_background();
	}

}

void PitchViewScene::playerPositionChangedSlot(int trackId, QPointF playerSceneCoordinates)
{
	// normalize the coordinates of the player
	QPointF playerCoordinatesNormalized = QPointF(playerSceneCoordinates.x()/_boundingRect.width(), 
		playerSceneCoordinates.y()/_boundingRect.height());

	if (playerCoordinatesNormalized.x() > 1)
		playerCoordinatesNormalized.setX(1);
	if (playerCoordinatesNormalized.y() > 1)
		playerCoordinatesNormalized.setY(1);
	if (playerCoordinatesNormalized.x() < 0)
		playerCoordinatesNormalized.setX(0);
	if (playerCoordinatesNormalized.y() < 0)
		playerCoordinatesNormalized.setY(0);

	Q_EMIT itemPositionChanged(trackId, playerCoordinatesNormalized);
}

void PitchViewScene::itemDraggedCoordinatesSlot(int id, QPointF itemSceneCoordinates)
{
	Q_EMIT itemDraggedSig(id, itemSceneCoordinates);
}

void PitchViewScene::selectedIdChanged(int trackId)
{
	if (playersMap.contains(currentClickedId))
	{
		PlayerItemWidget* player = playersMap[currentClickedId];
		player->setState(PlayerItemWidget::E_STATE::DEFAULT);
		currentClickedId = trackId;
	}

	if (playersMap.contains(trackId))
	{
		PlayerItemWidget* player = playersMap[trackId];
		player->setState(PlayerItemWidget::E_STATE::SELECTED);
		currentClickedId = trackId;
	}

	updateScene();
	emit selectedIdChangedSig(trackId);
}

void PitchViewScene::previewDistanceLineReady(json data)
{
	distancePreviewLine = data;
	std::cout << "Distance Preview Line: " << distancePreviewLine<< std::endl;
}

void PitchViewScene::clearPreviewLine()
{
	distancePreviewLine = json();
	if(previewDistanceLine)
		this->removeItem(previewDistanceLine);
}

void PitchViewScene::__draw_cricket_background()
{
	setSceneRect(cricketBoundingRect);
	_boundingRect = cricketBoundingRect;
	this->clear();
	int x, y, w, h;
	x = _boundingRect.x();
	y = _boundingRect.y();
	w = _boundingRect.width();
	h = _boundingRect.height();

	/*Cricket Oval Construction*/
	QGraphicsEllipseItem* greenEllipse = this->addEllipse(
		_boundingRect,
		QPen(QColorConstants::White),
		QBrush(QColorConstants::DarkGreen)
	);

	float scale = 0.95f;
	QGraphicsEllipseItem* boundaryLine = this->addEllipse(
		QRect(x + ((1 - scale) / 2) * w, y + ((1 - scale) / 2) * h, scale * w, scale * h),
		QPen(QColorConstants::White, 3)
	);

	int rWidth, rHeight;
	rWidth = w * 0.15; rHeight = h * 0.3;
	QColor rectColor(234, 227, 201);
	QGraphicsRectItem* pitchRect = this->addRect(
		QRect(w / 2 - rWidth / 2, h / 2 - rHeight / 2, rWidth, rHeight),
		QPen(rectColor),
		QBrush(rectColor)
	);

	QFont font;
	font.setBold(true);
	font.setPointSizeF(40);
	int lineHeight = 40;

	QGraphicsTextItem* text = this->addText(QString("Leg"), font);
	QRectF r = text->boundingRect();
	text->setPos(w / 2 - rWidth - 100, h / 2 - lineHeight);

	text = this->addText(QString("Off"), font);
	text->setPos(w / 2 + rWidth, h / 2 - lineHeight);
}

void PitchViewScene::__draw_soccer_background()
{
	this->clear();
	// the ratio of height to width height:width = 1:1.3
	int height = 590;
	int width = height * 1.3;
	_boundingRect = QRect(0, 0, width, height);
	setSceneRect(_boundingRect);
	QGraphicsRectItem* rect = this->addRect(
		_boundingRect,
		QPen(QColorConstants::White, 3),
		QBrush(QColorConstants::DarkGreen)
	);

	/*Left 18 area */
	int lx = 0, y = int(_boundingRect.height() * ((1 - 0.44) / 2)), w = int(_boundingRect.width() * 0.14), h = int(_boundingRect.height() * 0.44);
	float pX = _boundingRect.width() * (12.0 / 130.0), pY = _boundingRect.height() * 0.5;
	/*Penalty arc Left*/
	float pAwidth = _boundingRect.width() * 0.16;
	this->addEllipse(QRectF(pX - pAwidth/2, pY - pAwidth/2, pAwidth,  pAwidth), 
		QPen(QColorConstants::White, 2)
	);

	/*Penalty Arc right*/
	float rPX = _boundingRect.width() * (1 - 12.0 / 130.0);
	this->addEllipse(QRectF(rPX - pAwidth / 2, pY - pAwidth / 2, pAwidth, pAwidth),
		QPen(QColorConstants::White, 2)
	);

	/*Left 18 Area rectangle*/
	this->addRect(QRect(0, y, w, h), 
		QPen(QColorConstants::White, 2), QBrush(QColorConstants::DarkGreen));

	/*Left penalty dot*/
	this->addEllipse(pX - 2.5, pY - 2.5, 5, 5, QPen(QColorConstants::White, 2), QBrush(QColorConstants::White));

	/*Right 18 area*/
	this->addRect(QRect(int(_boundingRect.width() - _boundingRect.width() * 0.14), y, w, h),
		QPen(QColorConstants::White, 2),
		QBrush(QColorConstants::DarkGreen));

	/*right penalty dot*/
	this->addEllipse(rPX - 2.5, pY - 2.5, 5, 5, QPen(QColorConstants::White, 2), QBrush(QColorConstants::White));

	/*Left Goal Area*/
	int gL = 0, gT = int(y + _boundingRect.height() * 0.12), gW = int(_boundingRect.width() * 0.05), gH = int(_boundingRect.height() * 0.20);
	this->addRect(QRect(gL, gT, gW, gH), QPen(QColorConstants::White, 2));

	/*Right Goal Area*/ 
	this->addRect(QRect(gL + _boundingRect.width()*(1-0.05), gT, gW, gH), QPen(QColorConstants::White, 2));

	/*Center Circle*/
	float cX = _boundingRect.width() * 0.5, cY = _boundingRect.height() *0.5, bW = _boundingRect.height() * 0.20;
	float rX = cX - bW / 2, rY = cY - bW / 2;
	this->addEllipse(rX, rY, bW, bW, QPen(QColorConstants::White, 2));
	this->addEllipse(cX-5, cY-5, 10.0f, 10.0f, QPen(QColorConstants::White, 2), QBrush(QColorConstants::White));

	/*Center Line*/
	this->addLine(_boundingRect.width() * 0.5, 0,
		_boundingRect.width() * 0.5, _boundingRect.height(),
		QPen(QColorConstants::White, 2)
	);

	
}


PlayerItemWidget::PlayerItemWidget(int id, std::tuple<double, double> coord, PLAYER_TYPE pt, QGraphicsItem* parent)
	:QGraphicsEllipseItem(parent), 
	trackId(id),
	coordinates(coord),
	state(E_STATE::DEFAULT),
	playerPosition(E_POSITION::FIELDER),
	width(20), height(20),
	playerType(pt),
	playerPositionChanged(false)
{
	const auto [x, y] = coordinates;
	QRect rect(x, y, width, height);
	this->setRect(rect);
	this->setBrush(QBrush(__state2color__(state)));
	this->setPen(__state2pen__(state));
	
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
	this->setZValue(3);
	currentPlayerPosition = QPointF(x, y);
	init();
}

void PlayerItemWidget::init()
{
	if (playerType == PLAYER_TYPE::PLOTTED)
	{
		// add a little traingle on the objects top to indicate that it's plotted
		auto [x, y] = coordinates;
		double t_width = 10, t_height = 10;
		x = x + (width / 2);
		y = y - (height / 2);
		QPolygonF polygon({
			QPointF(x - (t_width / 2), y - (t_height / 2)),
			QPointF(x + (t_width / 2), y - (t_height / 2)),
			QPointF(x, y + (t_height / 2))
		});

		QGraphicsPolygonItem* triangle = new QGraphicsPolygonItem(polygon, this);
		triangle->setBrush(QBrush(QColorConstants::Blue));
		triangle->setPen(QPen(QColorConstants::Blue));
	}
	else if (playerType == PLAYER_TYPE::PHANTOM) 
	{
		auto [x, y] = coordinates;
		double t_width = 35, t_height = 25;
		x = x + (width / 2);
		y = y + (height / 2);
		QPolygonF polygon({
			QPointF(x - (t_width / 2), y - (t_height / 2)),
			QPointF(x + (t_width / 2), y - (t_height / 2)),
			QPointF(x, y + (t_height / 2))
			});

		QGraphicsPolygonItem* triangle = new QGraphicsPolygonItem(polygon, this);
		triangle->setBrush(QBrush(QColorConstants::Blue));
		triangle->setPen(QPen(QColorConstants::Blue));
		triangle->setZValue(3);
	}
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

	if (playerType != PLAYER_TYPE::PLOTTED)
	{
		this->setRect(QRectF(x, y, width, height));
		idText->setPos(x - 2, y - 3);
	}
	
	if (playerPosition == KEvents::EPLAYER_POSITIONS::BATMAN || playerPosition == KEvents::EPLAYER_POSITIONS::BATMAN_A)
	{
		QColor c = QColorConstants::Red;
		this->setBrush(c);
		if (state == E_STATE::SELECTED)
		{
			this->setPen(QPen(QColorConstants::Yellow, 3));
		}
		else {
			this->setPen(c);
		}
	}
	else if (playerPosition == KEvents::EPLAYER_POSITIONS::UMPIRE)
	{
		QColor c = QColor::fromRgb(51, 255, 255);
		this->setBrush(c);
		if (state == E_STATE::SELECTED)
		{
			this->setPen(QPen(QColorConstants::Yellow, 3));
		}
		else {
			this->setPen(c);
		}
	}
	else {

		this->setBrush(QBrush(__state2color__(state)));
		this->setPen(__state2pen__(state));
	}
}

bool PlayerItemWidget::operator==(int& trackId)
{
	return (trackId == this->trackId);
}

void PlayerItemWidget::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	emit updateClickedId(this->trackId);
	state = E_STATE::SELECTED;
	updateGraphic();

}

void PlayerItemWidget::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	if (event->buttons() & Qt::LeftButton && playerType == PLAYER_TYPE::PLOTTED)
	{
		QPointF deltaPos = event->pos() - event->lastPos();
		QRectF sceneRect = scene()->sceneRect();
		if (sceneRect.contains(event->scenePos()))
		{
			this->setPos(this->pos() + deltaPos);
			playerPositionChanged = true;
			if (QGraphicsView* view = scene()->views().first()) {
				QPoint viewPos = view->mapFromScene(event->scenePos());
				//coordinates = { viewPos.x(), viewPos.y() };
				currentPlayerPosition = viewPos;
				Q_EMIT itemDraggedSig(trackId, viewPos);
			}
		}
	}
}

void PlayerItemWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	if (playerPositionChanged)
	{
		playerPositionChanged = false;
		std::cout << "Item position changed ..." << std::endl;
		Q_EMIT itemPositionChanged(trackId, currentPlayerPosition);
	}
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
