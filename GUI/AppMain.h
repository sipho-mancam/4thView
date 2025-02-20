#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_AppMain.h"
#include <iostream>
#include <QDateTime>
#include <views/cricket_oval_widget.hpp>
#include <QtWidgets/QMessageBox>
#include "kevents.hpp"
#include "stream_data_store.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class AppMainClass; };
QT_END_NAMESPACE


// Custom stream buffer to capture stdout
 class StdoutStreamBuffer  : public QObject, public std::streambuf{
    Q_OBJECT
public:
    StdoutStreamBuffer(QObject* parent = nullptr) : m_parent(parent) {
        // Save the old buffer for stdout
        oldBuffer = std::cout.rdbuf(this);
    }

    ~StdoutStreamBuffer() {
        // Restore the old buffer
        std::cout.rdbuf(oldBuffer);
    }

protected:
    // Override the overflow method to capture output
    int_type overflow(int_type v) override {
        if (v == traits_type::eof()) {
            return traits_type::eof();
        }

        char ch = static_cast<char>(v);
        emitOutput(QString(ch));
        return v;
    }

    std::streamsize xsputn(const char* p, std::streamsize n) override {
        QString str = QString::fromLocal8Bit(p, n);
        emitOutput(str);
        return n;
    }

private:
    void emitOutput(const QString& text) {
        // Get the current timestamp
        QString timestamp = QDateTime::currentDateTime().toString("[dd:MM:yyyy][HH:mm:ss]");

        // Define the system name
        QString systemName = "[GUI]";

        // Format the output
        QString formattedOutput = timestamp + systemName + text;
        // Emit a signal with the captured text
        Q_EMIT outputCaptured(text);
    }

signals:
    void outputCaptured(const QString& text);

private:
    std::streambuf* oldBuffer;
    QObject* m_parent;
};


class AppMain : public QMainWindow
{
    Q_OBJECT

public:
    AppMain(QWidget *parent = nullptr);
    ~AppMain();

    virtual void closeEvent(QCloseEvent* e) override;
    void setEventManager(KEvents::EventsManager* evMan) { eventMan = evMan; }
    void setStreamDataStore(StreamDataStore* sDs);

private slots:
    void appendOutput(const QString& text)
    {
        ui->plainTextEdit->moveCursor(QTextCursor::End);
        ui->plainTextEdit->insertPlainText(text);
    }

private:
    Ui::AppMainClass *ui;
    QGraphicsScene* scene;
    PlayerPropertiesGroup* propsGroup;
    StdoutStreamBuffer* outputHandle;
    KEvents::EventsManager* eventMan;
    StreamDataStore* streamDs;
};
