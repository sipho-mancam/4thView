#pragma once
#include "ui_event_proc_giu.h"
#include <QRegularExpressionValidator>
#include <QMessageBox>
#include <nlohmann/json.hpp>


using json = nlohmann::json;

class EventProcessorDialog : public QDialog 
{
	Q_OBJECT

public:
	explicit EventProcessorDialog(QWidget* parent = nullptr);
	~EventProcessorDialog();

	void DialogAccepted();


signals:
	void event_processor_name(json data);

private:
	Ui::event_processor_dialog* ui;
};