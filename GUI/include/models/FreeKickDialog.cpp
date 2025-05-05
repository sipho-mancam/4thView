#include "FreeKickDialog.hpp"
#include <QMessageBox>
#include <iostream>
#include <QGraphicsSceneMouseEvent>

FreeKickSideDialog::FreeKickSideDialog(QWidget* parent)
	:QDialog(parent),
	ui(new Ui::freeKickSideDialog()),
	selectedSide(E_SIDE::NONE)
{
	ui->setupUi(this);

	connect(this, &QDialog::accepted, [&]() {
		if (selectedSide == E_SIDE::NONE)
		{
			QMessageBox::critical(nullptr, "Select Side", "Select the side the free kick will be directed towards.");
			this->show();
		}
		else {

			std::cout << kickerId << " : " << kickerCoordinates.x() << " , " << kickerCoordinates.y() << std::endl;
			Q_EMIT selectedSideSig(selectedSide);
			QPointF sideCoordinates;
			if (selectedSide == E_SIDE::LEFT_SIDE)
			{
				sideCoordinates = QPointF(0, 0.5);
			}
			else if (selectedSide == E_SIDE::RIGHT_SIDE)
			{
				sideCoordinates = QPointF(1, 0.5);
			}
			Q_EMIT kickerPlacedSig(kickerId, kickerCoordinates, sideCoordinates);
		}
	});

	QGraphicsScene* rightScene = new QGraphicsScene();
	ui->rightSide->setScene(rightScene);
	PitchSideItem* rightItem = new PitchSideItem(E_SIDE::RIGHT_SIDE);
	rightScene->addItem(rightItem);

	ui->rightSide->setMouseTracking(true);


	QGraphicsScene* leftScene = new QGraphicsScene();
	ui->leftSide->setScene(leftScene);
	PitchSideItem* leftItem = new PitchSideItem(E_SIDE::LEFT_SIDE);
	leftScene->addItem(leftItem);
	ui->leftSide->setMouseTracking(true);

	connect(ui->rightbtn, &QPushButton::clicked, [&]() {
		selectedSide = E_SIDE::RIGHT_SIDE;
		this->accept();
	});

	connect(ui->leftbtn, &QPushButton::clicked, [&]() {
		selectedSide = E_SIDE::LEFT_SIDE;
		this->accept();
	});
}

FreeKickSideDialog::~FreeKickSideDialog()
{
	delete ui;
}



void FreeKickSideDialog::showEvent(QShowEvent* event)
{
	QDialog::showEvent(event);
	selectedSide = E_SIDE::NONE;

}

void FreeKickSideDialog::kickerPlacedSlot(int trackId, QPointF coordinates)
{
	kickerId = trackId;
	kickerCoordinates = coordinates;
}

PitchSideItem::PitchSideItem(E_SIDE pSide, QGraphicsItem* parent)
	:QGraphicsRectItem(parent),
	QObject(),
	side(pSide)
{
	init();
	if (side == E_SIDE::RIGHT_SIDE)
	{
		this->setRotation(180);
	}
}

PitchSideItem::~PitchSideItem()
{

}

void PitchSideItem::init()
{
	// the ratio of height to width height:width = 1:1.3
	int height = 80;
	int width = height;
	int penSize = 1;

	this->setPen(QPen(QColorConstants::White, penSize));
	QRectF _boundingRect = QRectF(0, 0, width, height);
	_boundingRect = QRect(0, 0, width, height);

	rect = new QGraphicsRectItem(_boundingRect, this);
	rect->setBrush(QColorConstants::DarkGreen);
	rect->setPen(QPen(QColorConstants::White, penSize));

	/*Left 18 area */
	int lx = 0, y = int(_boundingRect.height() * ((1 - 0.44) / 2)), w = int(_boundingRect.width() * 0.14), h = int(_boundingRect.height() * 0.44);
	float pX = _boundingRect.width() * (12.0 / 130.0), pY = _boundingRect.height() * 0.5;

	/*Penalty arc Left*/
	float pAwidth = _boundingRect.width() * 0.16;
	ellipse = new QGraphicsEllipseItem(QRectF(pX - pAwidth / 2, pY - pAwidth / 2, pAwidth, pAwidth), this);
	ellipse->setPen(QPen(QColorConstants::White, penSize));

	/*Left 18 Area rectangle*/
	rect2 = new QGraphicsRectItem(QRect(0, y, w, h), this);
	rect2->setPen(QPen(QColorConstants::White, penSize));
	rect2->setBrush(QBrush(QColorConstants::DarkGreen));

	/*Left penalty dot*/
	ellipse2 = new QGraphicsEllipseItem(pX - 1, pY - 1, 2, 2, this);
	ellipse2->setPen(QPen(QColorConstants::White, penSize));
	ellipse2->setBrush(QBrush(QColorConstants::White));

	/*Left Goal Area*/
	int gL = 0, gT = int(y + _boundingRect.height() * 0.12), gW = int(_boundingRect.width() * 0.05), gH = int(_boundingRect.height() * 0.20);
	rect3 = new QGraphicsRectItem(QRect(gL, gT, gW, gH), this);
	rect3->setPen(QPen(QColorConstants::White, penSize));
	//std::cout << "Selected side: " << side << std::endl;
}

void PitchSideItem::mousePressEvent(QGraphicsSceneMouseEvent* evt)
{
	evt->accept();
	this->prepareGeometryChange();
	this->setPen(QPen(QColorConstants::Yellow, 3));
	Q_EMIT selectedSideSig(side);
	std::cout << "Selected side: " << side << std::endl;
	QGraphicsRectItem::mousePressEvent(evt);
}

void PitchSideItem::mouseMoveEvent(QGraphicsSceneMouseEvent* evt)
{
	std::cout << "Mouse move event" << std::endl;
}
