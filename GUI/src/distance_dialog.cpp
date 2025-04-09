#include "models/distance_dialog.hpp"
#include <QMessageBox>
#include <QGraphicsEllipseItem>
#include <QColorDialog>
#include <iostream>
#include <sstream>
#include <iomanip>


DistanceDialog::DistanceDialog(QWidget* parent)
	: ui(new Ui::DistanceD()),
	QDialog(parent),
	player1Id(0),
	player2Id(0),
	currentPlayerId(0),
	lineColor(Qt::gray)
{
	ui->setupUi(this);
	this->setModal(false);

	connect(this, &QDialog::accepted, this, &DistanceDialog::dialogAccepted);
	connect(this, &QDialog::rejected, this, &DistanceDialog::dialogRejected);
	
	/*******
	* Setup the player widget items
	*/
	QGraphicsScene* scene = new QGraphicsScene(this);
	int circleWidth = 35, circleHeight=35;
	player1Widget = scene->addEllipse(QRect(-2*circleWidth, 0, circleWidth, circleHeight), QPen(Qt::gray), QBrush(Qt::gray));
	player2Widget = scene->addEllipse(QRect(2*circleWidth, 0, circleWidth, circleHeight), QPen(Qt::gray), QBrush(Qt::gray));
	
	QFont font;
	font.setPointSize(16);
	font.setBold(true);
	
	
	player1Text = scene->addText("");
	player1Text->setPos(( - 2 * circleWidth +1), -1);
	player1Text->setFont(font);
	player1Text->setDefaultTextColor(Qt::black);
	

	player2Text = scene->addText("");
	player2Text->setPos((2 * circleWidth + 1), -1);
	player2Text->setFont(font);
	player2Text->setDefaultTextColor(Qt::black);

	ui->graphicsView->setScene(scene);

	connect(ui->player1_btn, &QPushButton::clicked, this, [this]() {
		currentPlayerId = 1;
		player1Widget->setPen(QPen(Qt::green,3));
		player2Widget->setPen(QPen(Qt::gray));
	});

	connect(ui->player2_btn, &QPushButton::clicked, this, [this]() {
		currentPlayerId = 2;
		player2Widget->setPen(QPen(Qt::green, 3));
		player1Widget->setPen(QPen(Qt::gray));
	});

	connect(ui->select_color, &QPushButton::clicked, this, [this]() {
			QColor color = QColorDialog::getColor(Qt::green, this, "Select Color");
			if (color.isValid())
			{
				lineColor = color;
			}
	});
}


DistanceDialog::~DistanceDialog()
{
	delete ui;
}

void DistanceDialog::dialogAccepted()
{
	if (player1Id != 0 && player2Id != 0 && player1Id != player2Id)
	{
		json data;
		data["player1"] = player1Id;
		data["player2"] = player2Id;
		data["lineColor"] = std::vector<int>({ lineColor.red(), lineColor.green(), lineColor.blue() });
		Q_EMIT distanceData(data);

		resetState();
	}
	else if (player1Id == player2Id)
	{
		QMessageBox::critical(this, "Invalid Input", "Player 1 and Player 2 cannot be the same.");
		this->show();
		return;
	}
	else {
		QMessageBox::critical(this, "Invalid Input", "Please select both players.");
		this->show();
		return;
	}
}

void DistanceDialog::dialogRejected()
{
	resetState();
}

void DistanceDialog::resetState()
{
	currentPlayerId = 0;
	player1Id = 0;
	player2Id = 0;
	player1Widget->setBrush(QBrush(Qt::gray));
	player2Widget->setBrush(QBrush(Qt::gray));
	player1Widget->setPen(QPen(Qt::gray));
	player2Widget->setPen(QPen(Qt::gray));

	player1Text->setPlainText("");
	player2Text->setPlainText("");
	Q_EMIT clearDistancePreview();
}

void DistanceDialog::showEvent(QShowEvent* evnt)
{
	
}

void DistanceDialog::selectedPlayer(int id)
{
	if (currentPlayerId == 1)
	{
		player1Id = id;
		player1Widget->setBrush(QBrush(Qt::green));

		std::ostringstream oss;
		oss << std::setw(2) << std::setfill('0') << std::to_string(player1Id);
		player1Text->setPlainText(QString::fromStdString(oss.str()));
		// update the player ID widget
	}
	else if (currentPlayerId == 2)
	{
		player2Id = id;
		player2Widget->setBrush(QBrush(Qt::green));
		std::ostringstream oss;
		oss << std::setw(2) << std::setfill('0') << std::to_string(player2Id);
		player2Text->setPlainText(QString::fromStdString(oss.str()));
		// update the player ID widget
	}

	if (player1Id != 0 && player2Id != 0)
	{
		if (player1Id == player2Id)
			return;

		Q_EMIT previewDistanceDataReady({
			{"player1", player1Id},
			{"player2", player2Id},
			{"lineColor", std::vector<int>({ lineColor.red(), lineColor.green(), lineColor.blue() }) }
		});
	}
}

