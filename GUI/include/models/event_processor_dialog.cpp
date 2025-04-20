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

	connect(this, &QDialog::accepted, this, &EventProcessorDialog::DialogAccepted);

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

	emit event_processor_name(eventProcData);
	std::cout << eventProcData << std::endl;
	
}

