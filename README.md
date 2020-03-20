# BotNetClient
This is the botnet client code I wrote as a PoC in cooperation with AT&amp;T Labs Research to prototype new attack vectors from IoT devices such as Wii Game Console back in 2009.  Keep in mind this was cutting edge research at the time and term IoT was not invented yet.  We referred to it as "Intelligent Electronics Devices" in the paper. :)

My work was presented at the December 2010 Annual Computer Security Applications Conference below.  

http://www.acsac.org/2010/program/case/wed-1030-Coskun-paper.pdf 

http://www.acsac.org/2010/program/case/wed-1030-Coskun.pdf

We took a popular video game, embedded this code into the game and made it available for download for free as a PoC.  Once they load and play this game, it runs in the background as a botnet client connecting to the BotNet Command and Control Center and accepts commands and act as a Zombie device accepting tcpflood commands to effect DDoS on Internet systems.

Pete Hoang

