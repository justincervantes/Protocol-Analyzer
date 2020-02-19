#ifndef SESSION_H
#define SESSION_H
#include <QString>
#include "filemanager.h"
#include "application.h"

struct sessionVariables {
    Application * app;
    QString behaviour = "Client Mode";
    QString ip = "192.168.0.15";
    int port = 8050;
    QString protocol = "TCP";
    QString filepath;
    int packet_size = 1024;
    int transmissions = 10;
    FileManager fm;
};



#endif // SESSION_H
