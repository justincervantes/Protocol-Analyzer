#ifndef SESSION_H
#define SESSION_H

#include <QString>
#include "filemanager.h"


class Session
{
public:

    QString behaviour;
    QString ip;
    int port;
    QString protocol;
    QString filepath;
    QString packet_size;
    int transmissions;
    FileManager fm;

    Session();
    void startTest(int action);
};

#endif // SESSION_H
