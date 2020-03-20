#include "tcpflood.h"
#include <ogc/lwp_watchdog.h>
#include <sys/types.h>

/*	Author: Pete Hoang
		Date:   02-24-2009
*/

int tcpflood(char* IP_Address, int port, int nothreads)
{
	int	returnStatus=0;
	int i, rc;
	lwp_t threads[10];
	struct netPort	netport;

	strcpy(netport.IP_Address,IP_Address);
	netport.port=port;

	for (i=0; i < nothreads; i++)
	{
		rc = LWP_CreateThread(&threads[i], tcpconnect, (void *) &netport, NULL, 0, 1);
		if(rc!=0)
			printf("Thread %d ERROR; return code from LWP_CreateThread is %d\n", i, rc);
		else
			printf("Thread %d created\n", i);
	}
	return returnStatus;
}


void* tcpconnect(void *netportvar)
{
	int simpleSocket = 0;
	int simplePort = 0;
	int returnStatus = 0;
	char buffer[256] = "";
	struct sockaddr_in simpleServer;
	struct netPort *netport=(struct netPort *) netportvar;
	int	j=0;

	for (j=0; j < 100; j++)
	{
		/* create a streaming socket */
		simpleSocket = net_socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
		if (simpleSocket < 0) {
			fprintf(stderr, "Could not create a socket! Socket #%d\n", simpleSocket);
			return;
			}
		else
			fprintf(stderr, "Socket created! Socket #%d\n", simpleSocket);

		/* retrieve the port number for connecting */
		simplePort = netport->port;
		/* set up the address structure */
		/* use the IP address argument for the server address */
		memset(&simpleServer, 0, sizeof(simpleServer));
		simpleServer.sin_family = AF_INET;
		simpleServer.sin_addr.s_addr= inet_addr(netport->IP_Address);
		simpleServer.sin_port = htons(simplePort);
		strcpy(buffer, "This is a test!\n");

		/* connect to the address and port with our socket */
		returnStatus = net_connect(simpleSocket, (struct sockaddr *)&simpleServer, sizeof(simpleServer));
		if (returnStatus >= 0)
			fprintf(stderr, "Connect successful!\n");
		else {
			fprintf(stderr, "Could not connect to address!\n");
			return;
			}

		/* send a message to the server */
		returnStatus = net_write(simpleSocket, buffer, sizeof(buffer));
		if ( returnStatus > 0 ) {
			printf("%d: %s", returnStatus, buffer);
		} else {
			fprintf(stderr, "Return Status = %d \n", returnStatus);
		}

		net_close(simpleSocket);
	}
	return 0;
}
