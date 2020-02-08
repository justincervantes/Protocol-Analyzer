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
    int packet_size;
    int transmissions;
    FileManager fm;

    Session();
    void startTest(int action);

private:
    void tcp_clnt();
    void tcp_serv();
    void udp_clnt();
    void udp_serv();
};

#endif // SESSION_H
