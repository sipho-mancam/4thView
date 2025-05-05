#pragma once
#include "ui_free_kick_side_dialog.h"
#include <QGraphicsObject>

enum E_SIDE {
	NONE = -1,
	LEFT_SIDE = 0,
	RIGHT_SIDE = 1
};

class PitchSideItem : public QObject, public QGraphicsRectItem
{
	Q_OBJECT
public:
	PitchSideItem(E_SIDE pSide, QGraphicsItem *parent = nullptr);
	~PitchSideItem();
	void init();
	virtual void mousePressEvent(QGraphicsSceneMouseEvent* evt) override;
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* evt) override;

signals:
	void selectedSideSig(E_SIDE s);

	

private:
	E_SIDE side;

	QGraphicsRectItem* rect, *rect2, *rect3;
	QGraphicsEllipseItem* ellipse, * ellipse2;
};

class FreeKickSideDialog : public QDialog
{
	Q_OBJECT
public:
	FreeKickSideDialog(QWidget* parent = nullptr);
	~FreeKickSideDialog();

	virtual void showEvent(QShowEvent* event) override;
signals:
	void selectedSideSig(E_SIDE s);

private:
	Ui::freeKickSideDialog* ui;
	E_SIDE selectedSide;
};