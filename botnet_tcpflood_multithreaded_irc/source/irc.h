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
#include "tcpflood.h"

int ircconnect(char* IP_Address, int port);
int irclisten(int socketno);
