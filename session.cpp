#include <QDebug>
#include "session.h"
#include <stdio.h>
#include <winsock2.h>
#include <errno.h>

Session::Session()
{
    behaviour = "Client Mode";
    ip = "192.168.51.125";
    port = 8050;
    protocol = "TCP";
    packet_size = 1024;
    transmissions = 10;
}

void Session::startTest(int action) {

    fm.buildBufferFromFile(packet_size);

    /*
     * Case 0: TCP Client
     * Case 1: TCP Server
     * Case 2: UDP Client
     * Case 3: UDP Server
     **/
    switch(action)
    {
        case 0:
            tcp_clnt();
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


void Session::tcp_clnt ()
{
    qDebug("Starting TCP Client Mode");

    int n; // counter for the number of bytes sent
    int ns;
    int err;
    SOCKET sd;
    struct hostent	*hp;
    struct sockaddr_in server;
    char  *host;
    char  *bp; // Used to store the server responses
    WSADATA WSAData;
    WORD wVersionRequested;
    QByteArray ba = ip.toLocal8Bit();
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
    server.sin_port = htons(port);
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
    qDebug("Sending handshake - Bytes: %d, Transmission Times: %d", packet_size, transmissions);

    // Send handshake (packet size should be specific)
    char handshake1[9];
    sprintf(handshake1, "%06d%03d", packet_size, transmissions);
    qDebug(handshake1);
    ns = send(sd, handshake1, 9, 0);

    // Wait for OK response 0
    qDebug("Waiting on response from server...");

    // Use bp as storage for the response codes from the server
    bp = (char*) malloc(sizeof(char) * 2);

    // Client makes repeated calls to recv until no more data is expected to arrive.
    // The while loop ensures data is read at the correct amount
    int firstcode = 2;
    while ((n = recv (sd, bp, firstcode, 0)) < 2)
    {
        firstcode -= n;
        bp += n;
        if (n == 0)
            break;
    }

    // Show me the firt response
    qDebug(bp);

    // Transmit the message the correct number of times, and print out each of the servers' response codes
    for(int j = 0; j < transmissions; j++) {
        ns = send (sd, fm.buffer, packet_size, 0);
        int bytes_to_read = 2;
        while ((n = recv (sd, bp, bytes_to_read, 0)) < 2)
        {
            bp += n;
            bytes_to_read -= n;
            if (n == 0)
                break;
        }
        // Show me subsequent responses
        qDebug(bp);

    }

    // Wait for the sequence code
    closesocket (sd);
    WSACleanup();
    //exit(0);
}

void Session::tcp_serv ()
{

}

void Session::udp_clnt ()
{

}

void Session::udp_serv ()
{

}
