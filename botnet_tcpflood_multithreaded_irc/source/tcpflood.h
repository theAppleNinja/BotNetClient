/*	Author: Pete Hoang
		Date:   02-24-2009
*/

#include <stdio.h>
#include <string.h>
#include <ogcsys.h>
#include <gccore.h>
#include <ogc/ipc.h>
#include <network.h>
#include <errno.h>
#include "reset.h"

struct netPort
{
	char IP_Address[20];
	int	  port;
};

int tcpflood(char* IP_Address, int port, int threads);
void* tcpconnect(void *);
void initialise_network();
