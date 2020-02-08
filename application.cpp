#include "application.h"
#include "ui_application.h"
#include <QDateTime>
#include <QFileDialog>
#include <QInputDialog>
#include "session.h"

Session session;

Application::Application(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Application)
{
    ui->setupUi(this);
}

Application::~Application()
{
    delete ui;
}


void Application::on_actionSet_IP_triggered()
{
    bool ok;
    QString text = QInputDialog::getText(this,
                                         tr("Set IP Address"),
                                         tr("IP Address:"),
                                         QLineEdit::Normal,
                                         "192.168.0.3", &ok);
    ui->ip_value->setText(text);
    session.ip = text;
}

void Application::on_actionSet_Port_triggered()
{
    bool ok;
    int text = QInputDialog::getInt(this,
                                    tr("Get Port Number"),
                                    tr("Port:"),
                                    8050, 0, 9999, 1, &ok);
    ui->port_value->setText(QString::number(text));
    session.port = text;
}

void Application::on_actionClient_triggered()
{
    ui->actionClient->setChecked(true);
    ui->actionServer->setChecked(false);
    ui->behaviour_value->setText("Client Mode");
    session.behaviour = "Client Mode";
}

void Application::on_actionServer_triggered()
{
    ui->actionClient->setChecked(false);
    ui->actionServer->setChecked(true);
    ui->behaviour_value->setText("Server Mode");
    session.behaviour = "Server Mode";
}

void Application::on_action10_triggered()
{
    ui->action10->setChecked(true);
    ui->action100->setChecked(false);
    ui->transmission_value->setText("10 times");
    session.transmissions = 10;
}

void Application::on_action100_triggered()
{
    ui->action10->setChecked(false);
    ui->action100->setChecked(true);
    ui->transmission_value->setText("100 times");
    session.transmissions = 100;
}

void Application::on_action1024B_triggered()
{
    ui->action1024B->setChecked(true);
    ui->action4096B->setChecked(false);
    ui->action20KB->setChecked(false);
    ui->action60KB->setChecked(false);
    ui->action100KB->setChecked(false);
    ui->packet_value->setText("1024B");
    session.packet_size = 1024;
}

void Application::on_action4096B_triggered()
{
    ui->action1024B->setChecked(false);
    ui->action4096B->setChecked(true);
    ui->action20KB->setChecked(false);
    ui->action60KB->setChecked(false);
    ui->action100KB->setChecked(false);
    ui->packet_value->setText("4096B");
    session.packet_size = 4096;
}

void Application::on_action20KB_triggered()
{
    ui->action1024B->setChecked(false);
    ui->action4096B->setChecked(false);
    ui->action20KB->setChecked(true);
    ui->action60KB->setChecked(false);
    ui->action100KB->setChecked(false);
    ui->packet_value->setText("20KB");
    session.packet_size = 20000;
}

void Application::on_action60KB_triggered()
{
    ui->action1024B->setChecked(false);
    ui->action4096B->setChecked(false);
    ui->action20KB->setChecked(false);
    ui->action60KB->setChecked(true);
    ui->action100KB->setChecked(false);
    ui->packet_value->setText("60KB");
    session.packet_size = 60000;
}

void Application::on_action100KB_triggered()
{
    ui->action1024B->setChecked(false);
    ui->action4096B->setChecked(false);
    ui->action20KB->setChecked(false);
    ui->action60KB->setChecked(false);
    ui->action100KB->setChecked(true);
    ui->packet_value->setText("100KB");
    session.packet_size = 100000;
}

void Application::on_actionTCP_triggered()
{
    ui->actionTCP->setChecked(true);
    ui->actionUDP->setChecked(false);
    ui->protocol_value->setText("TCP");
    session.protocol = "TCP";
}

void Application::on_actionUDP_triggered()
{
    ui->actionTCP->setChecked(false);
    ui->actionUDP->setChecked(true);
    ui->protocol_value->setText("UDP");
    session.protocol = "UDP";
}


void Application::on_start_button_clicked()
{
    // Set Time
    QString time = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss,zzz");
    ui->start_value->setText(time);
    ui->console_label->append("Starting test at " + time);

    //Get packet

    if(ui->behaviour_value->text() == "Client Mode" && ui->protocol_value->text() == "TCP") {
        session.startTest(0);
        ui->console_label->append(session.fm.buffer);

    } else if (ui->behaviour_value->text() == "Server Mode" && ui->protocol_value->text() == "TCP") {
       session.startTest(1);
    } else if (ui->behaviour_value->text() == "Client Mode" && ui->protocol_value->text() == "UDP") {
        session.startTest(2);
    } else {
        session.startTest(3);
    }
}

void Application::on_actionSet_File_triggered()
{
    QString text = QFileDialog::getOpenFileName(this,
                                                tr("File Selector"),
                                                tr("File"));
    ui->file_value->setText(text);
}
