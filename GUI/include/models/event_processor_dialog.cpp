#include "event_processor_dialog.hpp"
#include "iostream"


EventProcessorDialog::EventProcessorDialog(QWidget* parent)
	:QDialog(parent),
	ui(new Ui::event_processor_dialog())
{
	ui->setupUi(this);
	setModal(true);
	
	QRegularExpression re("\\b[a-zA-Z]+[-_]?[a-zA-Z0-9]*\\b");
	QRegularExpressionValidator* regValidator = new QRegularExpressionValidator(re);
	ui->lineEdit->setValidator(regValidator);

	QRegularExpression re2("[0-6]");
	QRegularExpressionValidator* regVal = new QRegularExpressionValidator(re2);
	ui->lineEdit_2->setValidator(regVal);
	connect(this, &QDialog::accepted, this, &EventProcessorDialog::DialogAccepted);
	connect(ui->pushButton, &QPushButton::clicked, this, &EventProcessorDialog::switchInnings);
}



EventProcessorDialog::~EventProcessorDialog()
{
	delete ui;
}

void EventProcessorDialog::DialogAccepted()
{
	const QValidator *nameVal = ui->lineEdit->validator();
	int pos = 0;
	QString eventName = ui->lineEdit->text();
	json eventProcData;
	if (nameVal->validate(eventName, pos) == QValidator::Acceptable)
	{
		eventProcData["sport_event_name"] = eventName.toStdString();
	}
	else {
		QMessageBox::critical(this, "Invalid Input", "Please make sure the event\nName is of the form: [Event-name]");
		this->open();
		return;
	}

	QString inning = ui->pushButton->text();
	eventProcData["inning"] = inning.toStdString();

	const QValidator* ballVal = ui->lineEdit_2->validator();
	QString ballNumber = ui->lineEdit_2->text();
	if (ballVal->validate(ballNumber, pos) == QValidator::Acceptable)
	{
		eventProcData["ball_number"] = std::atoi(ballNumber.toStdString().c_str());
		emit event_processor_name(eventProcData);

		std::cout << eventProcData << std::endl;
	}
	else {
		QMessageBox::critical(this, "Invalid Ball Number", "Insert a valid number:\nA number between 1 - 6.");
		this->open();
		return;
	}
}

void EventProcessorDialog::switchInnings()
{
	QString btnText = ui->pushButton->text();
	if (btnText == "1st")
	{
		ui->pushButton->setText(QString("2nd"));
	}
	else {
		ui->pushButton->setText(QString("1st"));
	}
}
