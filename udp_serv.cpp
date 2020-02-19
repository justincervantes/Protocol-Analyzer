/*---------------------------------------------------------------------------------------
--	SOURCE FILE:		udp_svr.c -   A simple echo server using UDP
--
--	PROGRAM:			usvr.exe
--
--	FUNCTIONS:			winsock2 API
--
--	DATE:				January 6, 2008
--
--	REVISIONS:			(Date and Description)
--
--	DESIGNERS:
--
--	PROGRAMMERS:		Aman Abdulla
--
--	NOTES:
--	The program will accept UDP packets from client machines.
-- The program will read data from the client socket and simply echo it back.
---------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <winsock2.h>
#include "session.h"

#define TRUE					1

extern struct sessionVariables globals;

void udp_serv() {

    int	client_len, n, port = globals.port;
    SOCKET sd;
    char	buf[globals.packet_size];
    struct	sockaddr_in server, client;
    WSADATA stWSAData;
    WORD wVersionRequested = MAKEWORD(2, 2);

    // Initialize the DLL with version Winsock 2.2
    WSAStartup(wVersionRequested, &stWSAData);

    // Create a datagram socket
    if ((sd = socket(PF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("Can't create a socket");
        exit(1);
    }

    // Bind an address to the socket
    memset((char*)&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sd, (struct sockaddr*) & server, sizeof(server)) == -1)
    {
        perror("Can't bind name to socket");
        exit(1);
    }

    int total_data_sent = 0;
    int total_data_read = 0;

    for(int i = 0; i < globals.transmissions; i++)
    {
        client_len = sizeof(client);
        if ((n = recvfrom(sd, buf, globals.packet_size, 0, (struct sockaddr*) & client, &client_len)) < 0)
        {
            perror("recvfrom error");
            exit(1);
        }

        total_data_read += n;

        if (sendto(sd, buf, n, 0, (struct sockaddr*) & client, client_len) != n)
        {
            perror("sendto error");
            exit(1);
        }

        total_data_sent += globals.packet_size;

    }

    globals.app->set_total_data_trasmitted(total_data_sent);
    globals.app->set_total_packets_lost(total_data_sent - total_data_read);

    closesocket(sd);
    WSACleanup();
}
