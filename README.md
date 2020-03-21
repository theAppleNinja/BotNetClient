# BotNetClient
This is the botnet client code I wrote as a PoC in cooperation with AT&amp;T Labs Research to prototype new attack vectors from IoT devices such as Wii Game Console back in 2009.  Keep in mind this was cutting edge research at the time and term IoT was not invented yet.  We referred to it as "Intelligent Electronics Devices" in the paper. :)

The research and code was completed in February 2009.  We were trying to warn the world this could happen one day but our research was not accepted for presentation until December 2010 as malware was unheard of for IoT devices.

My work was presented at the December 2010 Annual Computer Security Applications Conference below.  

http://www.acsac.org/2010/program/case/wed-1030-Coskun-paper.pdf 

http://www.acsac.org/2010/program/case/wed-1030-Coskun.pdf

We took a popular video game, embedded this code into the game and made it available for download for free as a PoC.  Once they load and play this game, it runs in the background as a botnet client connecting to the BotNet Command and Control Center and accepts commands and act as a Zombie device accepting tcpflood commands to effect DDoS on Internet systems.

I authored the source files: irc.h, irc.c, tcpflood.h, and tcpflood.c.  The rest of the source were library files I borrowed in order to compile on the gaming system.

Pete Hoang

