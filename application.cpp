#include "application.h"
#include "ui_application.h"
#include <QDateTime>
#include <QFileDialog>
#include <QInputDialog>
#include "session.h"
#include "tcp_clnt.cpp"
#include "tcp_serv.cpp"
#include "udp_clnt.cpp"
#include "udp_serv.cpp"

extern struct sessionVariables globals;

void startTest(int action);

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
    globals.ip = text;
}

void Application::on_actionSet_Port_triggered()
{
    bool ok;
    int text = QInputDialog::getInt(this,
                                    tr("Get Port Number"),
                                    tr("Port:"),
                                    8050, 0, 9999, 1, &ok);
    ui->port_value->setText(QString::number(text));
    globals.port = text;
}

void Application::on_actionClient_triggered()
{
    ui->actionClient->setChecked(true);
    ui->actionServer->setChecked(false);
    ui->behaviour_value->setText("Client Mode");
    globals.behaviour = "Client Mode";
}

void Application::on_actionServer_triggered()
{
    ui->actionClient->setChecked(false);
    ui->actionServer->setChecked(true);
    ui->behaviour_value->setText("Server Mode");
    globals.behaviour = "Server Mode";
}

void Application::on_action10_triggered()
{
    ui->action10->setChecked(true);
    ui->action100->setChecked(false);
    ui->transmission_value->setText("10 times");
    globals.transmissions = 10;
}

void Application::on_action100_triggered()
{
    ui->action10->setChecked(false);
    ui->action100->setChecked(true);
    ui->transmission_value->setText("100 times");
    globals.transmissions = 100;
}

void Application::on_action1024B_triggered()
{
    ui->action1024B->setChecked(true);
    ui->action4096B->setChecked(false);
    ui->action20KB->setChecked(false);
    ui->action60KB->setChecked(false);
    ui->action65KB->setChecked(false);
    ui->packet_value->setText("1024B");
    globals.packet_size = 1024;
}

void Application::on_action4096B_triggered()
{
    ui->action1024B->setChecked(false);
    ui->action4096B->setChecked(true);
    ui->action20KB->setChecked(false);
    ui->action60KB->setChecked(false);
    ui->action65KB->setChecked(false);
    ui->packet_value->setText("4096B");
    globals.packet_size = 4096;
}

void Application::on_action20KB_triggered()
{
    ui->action1024B->setChecked(false);
    ui->action4096B->setChecked(false);
    ui->action20KB->setChecked(true);
    ui->action60KB->setChecked(false);
    ui->action65KB->setChecked(false);
    ui->packet_value->setText("20KB");
    globals.packet_size = 20000;
}

void Application::on_action60KB_triggered()
{
    ui->action1024B->setChecked(false);
    ui->action4096B->setChecked(false);
    ui->action20KB->setChecked(false);
    ui->action60KB->setChecked(true);
    ui->action65KB->setChecked(false);
    ui->packet_value->setText("60KB");
    globals.packet_size = 60000;
}

void Application::on_action65KB_triggered()
{
    ui->action1024B->setChecked(false);
    ui->action4096B->setChecked(false);
    ui->action20KB->setChecked(false);
    ui->action60KB->setChecked(false);
    ui->action65KB->setChecked(true);
    ui->packet_value->setText("65KB");
    globals.packet_size = 65000;
}

void Application::on_actionTCP_triggered()
{
    ui->actionTCP->setChecked(true);
    ui->actionUDP->setChecked(false);
    ui->protocol_value->setText("TCP");
    globals.protocol = "TCP";
}

void Application::on_actionUDP_triggered()
{
    ui->actionTCP->setChecked(false);
    ui->actionUDP->setChecked(true);
    ui->protocol_value->setText("UDP");
    globals.protocol = "UDP";
}

void Application::on_start_button_clicked()
{

    globals.fm.buildBufferFromFile(globals.packet_size);

    // Set Time
    QTime t;
    t.restart();
    t.start();
    QString start_time = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss,zzz");
    ui->start_value->setText(start_time);
    ui->console_label->append("<font color='green'>Starting test at " + start_time + "</font>");


    if(ui->behaviour_value->text() == "Client Mode" && ui->protocol_value->text() == "TCP") {

        ui->console_label->append("<font color='green'>Sending the following text as a packet...</font>");
        ui->console_label->append(globals.fm.buffer);

        tcp_clnt();

        QString end_time = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss,zzz");
        ui->end_value->setText(end_time);
        ui->console_label->append("<font color='red'>Ending test at " + end_time + "</font>");
        ui->total_time_value->setText(QString::number(t.elapsed()) + "ms");

    } else if (ui->behaviour_value->text() == "Server Mode" && ui->protocol_value->text() == "TCP") {

        ui->console_label->append("<font color='green'>Locking the UI until test completion!</font>");
        ui->console_label->append("<font color='green'>Expecting to receive the following text as a packet...</font>");
        ui->console_label->append(globals.fm.buffer);

        tcp_serv();


        ui->console_label->append("<font color='red'>Ending test.</font>");

    } else if (ui->behaviour_value->text() == "Client Mode" && ui->protocol_value->text() == "UDP") {

        ui->console_label->append("<font color='green'>Sending the following text as a packet...</font>");
        ui->console_label->append(globals.fm.buffer);

        udp_clnt();

        QString end_time = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss,zzz");
        ui->end_value->setText(end_time);
        ui->console_label->append("<font color='red'>Ending test at " + end_time + "</font>");
        ui->total_time_value->setText(QString::number(t.elapsed()) + "ms");

    } else {

        ui->console_label->append("<font color='green'>Locking the UI until test completion!</font>");
        ui->console_label->append("<font color='green'>Expecting to receive the following text as a packet...</font>");
        ui->console_label->append(globals.fm.buffer);

        udp_serv();

        QString end_time = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss,zzz");
        ui->end_value->setText(end_time);
        ui->console_label->append("<font color='red'>Ending test at " + end_time + "</font>");
        ui->total_time_value->setText(QString::number(t.elapsed()) + "ms");
    }
}

void Application::append_console(QString input)
{
    ui->console_label->append(input);
}

void Application::set_total_data_trasmitted(int input)
{
    ui->total_data_value->setText(QLocale(QLocale::English).toString(input));
}

void Application::set_total_packets_lost(int input)
{
    ui->total_packets_lost_value->setText(QString::number(input));
}








