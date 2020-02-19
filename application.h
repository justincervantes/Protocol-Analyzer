#ifndef APPLICATION_H
#define APPLICATION_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Application; }
QT_END_NAMESPACE

class Application : public QMainWindow
{
    Q_OBJECT

public:
    Application(QWidget *parent = nullptr);
    ~Application();

    void append_console(QString);

    void set_total_data_trasmitted(int input);

    void set_total_packets_lost(int input);
private slots:
    void on_actionSet_IP_triggered();

    void on_actionSet_Port_triggered();

    void on_actionClient_triggered();

    void on_actionServer_triggered();

    void on_action10_triggered();

    void on_action100_triggered();

    void on_action1024B_triggered();

    void on_action4096B_triggered();

    void on_action20KB_triggered();

    void on_action60KB_triggered();

    void on_action65KB_triggered();

    void on_actionTCP_triggered();

    void on_actionUDP_triggered();

    void on_start_button_clicked();



private:
    Ui::Application *ui;
};
#endif // APPLICATION_H
