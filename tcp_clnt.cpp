#include "session.h"
#include <winsock.h>
#include <windows.h>
#include <stdio.h>

extern struct sessionVariables globals;


void tcp_clnt()
{
    qDebug("Starting TCP Client Mode");

    int ns = 0;
    int err;
    SOCKET sd;
    struct hostent	*hp;
    struct sockaddr_in server;
    char  *host;
    char  *bp = static_cast<char*>(malloc(sizeof(char) * static_cast<size_t>(globals.packet_size))); // Used to store the server responses
    WSADATA WSAData;
    WORD wVersionRequested;

    // QString to char array
    QByteArray ba = globals.ip.toLocal8Bit();
    char *ipAsCharPointer = ba.data();
    host = ipAsCharPointer;

    // Access the Windows Socket DLL
    wVersionRequested = MAKEWORD( 2, 2 );
    err = WSAStartup( wVersionRequested, &WSAData );
    if ( err != 0 ) //No usable DLL
    {
        printf ("DLL not found!\n");
        exit(1);
    }


    // Create the socket
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Cannot create socket");
        exit(1);
    }





    // Initialize and set up the address structure
    memset((char *)&server, 0, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_port = htons(globals.port);
    if ((hp = gethostbyname(host)) == NULL)
    {
        fprintf(stderr, "Unknown server address\n");
        exit(1);
    }

    // Copy the server address
    memcpy((char *)&server.sin_addr, hp->h_addr, hp->h_length);

    // Connecting to the server
    if (connect (sd, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        fprintf(stderr, "Can't connect to server\n");
        perror("connect");
        exit(1);
    }

    // Display connection details
    qDebug("Connected to: %s", inet_ntoa(server.sin_addr));



    for(int j = 0; j < globals.transmissions; j++) {


        // Transmit the message
        if( send (sd, globals.fm.buffer, globals.packet_size, 0) == SOCKET_ERROR) {
            qDebug("Send error: %u\n", WSAGetLastError());
            j--;
        } else {
            ns+= globals.packet_size;
            qDebug("Number of bytes transmitted: %d", ns);
        }

    }
    globals.app->set_total_data_trasmitted(ns);
    globals.app->set_total_packets_lost((ns - (globals.packet_size * globals.transmissions))/globals.packet_size);
    closesocket (sd);
    WSACleanup();
}
