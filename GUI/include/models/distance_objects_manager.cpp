#include "distance_objects_manager.hpp"
#include <iostream>


DistanceWidget::DistanceWidget(int id, QWidget* parent)
	:QWidget(parent), 
	ui(new Ui::distance_widget()),
	objectId(id),
	iconScene(new QGraphicsScene())
{
	ui->setupUi(this);
	ui->gridLayout->setContentsMargins(0, 0, 0, 0);
	ui->gridLayout->setSpacing(0);
	ui->gridLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);

	ui->horizontalLayout_2->setAlignment(Qt::AlignTop);
	distanceColorIcon = iconScene->addEllipse(QRect(0, 0, 35, 35), QPen(QColorConstants::Gray, 0), QBrush(QColorConstants::Gray));
	ui->graphicsView->setScene(iconScene);
	distanceColorIcon->setBrush(QBrush(QColorConstants::Red));
	connect(ui->pushButton, &QPushButton::clicked, this, &DistanceWidget::deletePressed);
	ui->label_3->setText(QString::number(id));
	ui->label_4->setText(QString::number(0.0));
}



DistanceWidget::~DistanceWidget()
{
	delete ui;
}

void DistanceWidget::setColor(QColor col)
{
	distanceColorIcon->setBrush(QBrush(col));
	distanceColorIcon->setPen(QPen(col, 1));
}

void DistanceWidget::deletePressed()
{
	emit distanceObjectDeleted(objectId);
}

DistanceObjectsManager::DistanceObjectsManager(QWidget* layoutObject, QScrollArea* scroll,  QWidget* parent)
	: QWidget(parent),
	listLayout(layoutObject),
	scrollArea(scroll)
{
	
	distanceList = new QVBoxLayout();
	scrollArea->widget()->setLayout(distanceList);
	distanceList->setAlignment(Qt::AlignTop);
	distanceList->setContentsMargins(0, 0, 0, 0);
	distanceList->setSpacing(0);
}

void DistanceObjectsManager::addDistanceObject(json distanceObject)
{
	int objectId = distanceObject["objectId"];
	QColor col = QColor(distanceObject["lineColor"][0], distanceObject["lineColor"][1], distanceObject["lineColor"][2]);
	DistanceWidget* dObj = new DistanceWidget(objectId);
	distanceList->addWidget(dObj, Qt::AlignTop);
	dObj->setColor(col);
	connect(dObj, &DistanceWidget::distanceObjectDeleted, this, &DistanceObjectsManager::distanceObjectDeleted);
	std::cout << "Distance Object added" << std::endl;
}




void DistanceObjectsManager::distanceObjectDeleted(int objectId)
{
	for (int i = 0; i < distanceList->count(); i++)
	{
		QLayoutItem* item = distanceList->itemAt(i);
		QWidget* wdgt = item->widget();
		DistanceWidget* dObj = reinterpret_cast<DistanceWidget*>(wdgt);
		if (dObj->getObjectId() == objectId)
		{
			distanceList->removeItem(item);
			wdgt->deleteLater();
			break;
		}
	}
	
	emit distanceObjectDeletedSignal(objectId);
	//std::cout << "Distance Object Deleted with ID: " << objectId << std::endl;
}
