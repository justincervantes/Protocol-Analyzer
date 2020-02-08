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

    void on_action100KB_triggered();

    void on_actionTCP_triggered();

    void on_actionUDP_triggered();

    void on_start_button_clicked();

    void on_actionSet_File_triggered();

private:
    Ui::Application *ui;
};
#endif // APPLICATION_H
