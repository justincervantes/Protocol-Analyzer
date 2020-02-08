#include <QDebug>
#include "session.h"

Session::Session()
{
    //behaviour;
    //ip;
    //port = ;
    protocol = "TCP";
    //filepath;
    packet_size = "1024B";
    transmissions = 10;
}

void Session::startTest(int action) {

    fm.buildBufferFromFile(packet_size);



    //if()
    //qDebug(currentBuffer);
    //qInfo(currentBuffer);
    /*
     * Case 0: TCP Client
     * Case 1: TCP Server
     * Case 2: UDP Client
     * Case 3: UDP Server
     **/
    switch(action)
    {
        case 0:
            qDebug("TCP Client");
            // Validate IP, Port,
            //
            break;
        case 1:
        printf("TCP Server");
            // Get current IP and launch
            break;
        case 2:
        printf("UDP Client");
            // Validate IP, Port,
            break;
        case 3:
        printf("UDP Server");
            // Get current IP and launch
            break;
    default:
        break;
    }

}
