#pragma once
#include "models/ui_teams_configuration.h"
#include <QColorDialog>
#include <QGraphicsItem>
#include <QMessageBox>
#include <nlohmann/json.hpp>


using json = nlohmann::json;

class QGraphicsEllipseItemS : public QObject, public QGraphicsEllipseItem
{
	Q_OBJECT
public:
	QGraphicsEllipseItemS(const QRectF& rect, const QPen& pen, const QBrush& brush, QGraphicsItem* parent = nullptr)
		: QGraphicsEllipseItem(rect, parent),
		QObject(),
		m_item(nullptr)
	{
		setPen(pen);
		setBrush(brush);
	}

	QGraphicsEllipseItemS(QGraphicsEllipseItem* item)
		: m_item(item) 
	{
	}

	void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
		QGraphicsEllipseItem::mousePressEvent(event);
		emit clicked();
	}

signals:
	void clicked();

private:
	QGraphicsEllipseItem* m_item;
};


class TeamsConfigManager : public QDialog
{
	Q_OBJECT
public:
	TeamsConfigManager(QWidget* parent = nullptr);
	~TeamsConfigManager();

signals:
	void teamAConfigured(QColor color, std::string name);
	void teamBConfigured(QColor color, std::string name);
	void  teamsConfigured(QColor colorA, std::string nameA, QColor colorB, std::string nameB);

private:
	Ui::teamsConfig* ui;
	std::string teamAName, teamBName;
	QColor teamAColor, teamBColor;
	QGraphicsEllipseItemS* teamAColorPicker, *teamBColorPicker;
};
