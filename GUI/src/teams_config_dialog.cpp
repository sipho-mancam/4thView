#include "models/teams_config_dialog.hpp"


TeamsConfigManager::TeamsConfigManager(QWidget* parent)
	:QDialog(parent),
	ui(new Ui::teamsConfig),
	teamAColor(QColorConstants::Gray),
	teamBColor(QColorConstants::Gray)	
{

	ui->setupUi(this);
	QGraphicsScene* sceneA = new QGraphicsScene();
	ui->teamAColor->setScene(sceneA);

	teamAColorPicker = new QGraphicsEllipseItemS(QRect(0, 0, 100, 100), QPen(QColorConstants::Gray), QBrush(QColorConstants::Gray));
	sceneA->addItem(teamAColorPicker);

	connect(teamAColorPicker, &QGraphicsEllipseItemS::clicked, this, [&]() {
		QColor color = QColorDialog::getColor(Qt::white, this, "Select Team A Color");
		if (color.isValid()) {
			teamAColor = color;
			teamAColorPicker->setBrush(teamAColor);
		}
		qDebug() << "Team A Color: " << color.red() << color.green() << color.blue();
	});

	connect(ui->teamAEdit, &QLineEdit::returnPressed, this, [&]() {
		if (ui->teamAEdit->text().isEmpty()) {
			QMessageBox::critical(this, "Error", "Team A name cannot be empty");
			return;
		}
		teamAName = ui->teamAEdit->text().toStdString();
		Q_EMIT teamAConfigured(teamAColor, teamAName);
	});

	QGraphicsScene* sceneB = new QGraphicsScene();
	ui->teamBColor->setScene(sceneB);

	teamBColorPicker = new QGraphicsEllipseItemS(QRect(0, 0, 100, 100), QPen(QColorConstants::Gray), QBrush(QColorConstants::Gray));
	sceneB->addItem(teamBColorPicker);

	connect(teamBColorPicker, &QGraphicsEllipseItemS::clicked, this, [&]() {
		QColor color = QColorDialog::getColor(Qt::white, this, "Select Team A Color");
		if (color.isValid()) {
			teamBColor = color;
			teamBColorPicker->setBrush(teamBColor);
		}	
	});

	connect(ui->teamBEdit, &QLineEdit::returnPressed, this, [&]() {
		if (ui->teamBEdit->text().isEmpty()) {
			QMessageBox::critical(this, "Error", "Team B name cannot be empty");
			return;
		}
		teamBName = ui->teamBEdit->text().toStdString();
		Q_EMIT teamBConfigured(teamBColor, teamBName);
	});

	connect(ui->teamASave, &QPushButton::clicked, this, [&]() {
		if (ui->teamAEdit->text().isEmpty()) {
			QMessageBox::critical(this, "Error", "Team A name cannot be empty");
			return;
		}
		teamAName = ui->teamAEdit->text().toStdString();
		Q_EMIT teamAConfigured(teamAColor, teamAName);
	});

	connect(ui->teamBSave, &QPushButton::clicked, this, [&]() {
		if (ui->teamBEdit->text().isEmpty()) {
			QMessageBox::critical(this, "Error", "Team B name cannot be empty");
			return;
		}
		teamBName = ui->teamBEdit->text().toStdString();
		Q_EMIT teamBConfigured(teamBColor, teamBName);
	});

	connect(this, &QDialog::accepted, this, [&]() {
		if (ui->teamAEdit->text().isEmpty())
		{
			QMessageBox::critical(this, "Error", "Team A name cannot be empty");
			return;
		}
		else {
			teamAName = ui->teamAEdit->text().toStdString();
		}

		if (ui->teamBEdit->text().isEmpty())
		{
			QMessageBox::critical(this, "Error", "Team B name cannot be empty");
			return;
		}
		else {
			teamBName = ui->teamBEdit->text().toStdString();
		}
		Q_EMIT teamsConfigured(teamAColor, teamAName, teamBColor, teamBName);
	});

	

}

TeamsConfigManager::~TeamsConfigManager()
{
	delete ui;
}
