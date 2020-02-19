/*---------------------------------------------------------------------------------------
--	SOURCE FILE:	udp_clnt.c - A simple UDP client program.
--
--	PROGRAM:		uclnt.exe
--
--	FUNCTIONS:		Winsock 2 API
--
--	DATE:			January 6, 2008
--
--	REVISIONS:		(Date and Description)
--
--	DESIGNERS:
--
--	PROGRAMMERS:	Aman Abdulla
--
--	NOTES:
--	The program will send a UDP packet to a user specifed server.
--  The server can be specified using a fully qualified domain name or and
--	IP address. The packet is time stamped and then sent to the server. The
--  response (echo) is also time stamped and the delay is displayed.
---------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <winsock2.h>
#include <errno.h>
#include <string.h>
#include "session.h"
#include <sys/types.h>

extern struct sessionVariables globals;

void udp_clnt() {

    int	data_size = globals.packet_size, port = globals.port;
    int	i, j, server_len, client_len;
    SOCKET sd;
    char *host, rbuf[globals.packet_size], sbuf[globals.packet_size];
    struct	hostent	*hp;
    struct	sockaddr_in server, client;
    SYSTEMTIME stStartTime, stEndTime;
    WSADATA stWSAData;
    WORD wVersionRequested = MAKEWORD (2,2);


    // Initialize the DLL with version Winsock 2.2
    WSAStartup(wVersionRequested, &stWSAData ) ;


    // Create a datagram socket
    if ((sd = socket(PF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror ("Can't create a socket\n");
        exit(1);
    }

    struct timeval read_timeout;
    read_timeout.tv_sec = 0;
    read_timeout.tv_usec = 3;
//    int iResult = setsockopt(sd, SOL_SOCKET, SO_RCVTIMEO, (char*) &read_timeout, sizeof read_timeout);
//    if (iResult == SOCKET_ERROR) {
//        qDebug("getsockopt for SO_KEEPALIVE failed with error: %u\n", WSAGetLastError());
//    } else
//        qDebug("SO_KEEPALIVE Value: %ld\n", read_timeout);

    int bOptLen = sizeof(BOOL);
    BOOL bOptVal = TRUE;
    char * msto = {"5"};
    qDebug("%s", (char*) &read_timeout);
    int iResult = setsockopt(sd, SOL_SOCKET, SO_RCVTIMEO, msto, sizeof(msto));
    if (iResult == SOCKET_ERROR) {
        qDebug("setsockopt for SO_RCVTIMEO failed with error: %u\n", WSAGetLastError());
    } else
        qDebug("Set SO_RCVTIMEO: ON\n");

    // Store server's information
    memset((char *)&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    const char* myIP = globals.ip.toStdString().c_str();

    if ((hp = gethostbyname(myIP)) == NULL)
    {
        qDebug("Can't get server's IP address\n");
        exit(1);
    }
    //strcpy((char *)&server.sin_addr, hp->h_addr);
    memcpy((char *)&server.sin_addr, hp->h_addr, hp->h_length);

    // Bind local address to the socket
    memset((char *)&client, 0, sizeof(client));
    client.sin_family = AF_INET;
    client.sin_port = htons(0);  // bind to any available port
    client.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sd, (struct sockaddr *)&client, sizeof(client)) == -1)
    {
        perror ("Can't bind name to socket");
        exit(1);
    }
    // Find out what port was assigned and print it
    client_len = sizeof(client);
    if (getsockname (sd, (struct sockaddr *)&client, &client_len) < 0)
    {
        perror ("getsockname: \n");
        exit(1);
    }
    qDebug ("Client's port assigned is %d\n", ntohs(client.sin_port));

    if (data_size > globals.packet_size)
    {
        qDebug("Data is too big\n");
        exit(1);
    }


    server_len = sizeof(server);

    int ns = 0;

    for(int j = 0; j < globals.transmissions; j++) {

        // transmit data
        if (sendto (sd, globals.fm.buffer, globals.packet_size, 0, (struct sockaddr *)&server, server_len) == -1)
        {
            perror("Send to failure");
            exit(1);
        }

        ns += globals.packet_size;

        // receive data
        memset(rbuf, 0, sizeof rbuf);
        recvfrom (sd, rbuf, globals.packet_size, 0, (struct sockaddr *)&server, &server_len);

        //globals.app->append_console(rbuf);

        if (strncmp(globals.fm.buffer, rbuf, data_size) != 0) {
            qDebug("Data is corrupted\n");
            j--;
        }


    }

    globals.app->set_total_data_trasmitted(ns);
    globals.app->set_total_packets_lost((ns - (globals.packet_size * globals.transmissions))/globals.packet_size);
    closesocket(sd);
    WSACleanup();
}
