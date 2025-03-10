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

	for (int i = 0; i < 10; i++)
	{
		DistanceWidget* testObject = new DistanceWidget(i);
		distanceList->addWidget(testObject, Qt::AlignTop);

		connect(testObject, &DistanceWidget::distanceObjectDeleted, this, &DistanceObjectsManager::distanceObjectDeleted);
	}
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
