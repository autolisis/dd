/*
 * ip.c
 * Copyright (C) 2019 g <g@ABCL>
 *
 * Distributed under terms of the MIT license.
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <errno.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

void printIP() {
  char hostbuffer[256];
  char *IPbuffer;
  struct hostent *host_entry;
  int hostname;

  hostname = gethostname(hostbuffer, sizeof(hostbuffer));
  if (hostname == -1)
    perror("Some error occured");

  host_entry = gethostbyname(hostbuffer);
  if (!host_entry)
    perror("Some error occured");

  IPbuffer = inet_ntoa(*((struct in_addr *)host_entry->h_addr_list[0]));

  printf("Host IP: %s\n\n", IPbuffer);
}

int main() {
  unsigned char ip_address[15];
  int fd;
  struct ifreq ifr;
  struct sockaddr_in *addr;

  printIP();

  printf("Enter new Ip Address: ");
  scanf("%s", ip_address);

  /*AF_INET - to define network interface IPv4*/
  /*Creating soket for it.*/
  fd = socket(AF_INET, SOCK_DGRAM, 0);

  /*AF_INET - to define IPv4 Address type.*/
  ifr.ifr_addr.sa_family = AF_INET;

  /*eth0 - define the ifr_name - port name
  where network attached.*/
  memcpy(ifr.ifr_name, "eth0", IFNAMSIZ - 1);

  /*defining the sockaddr_in*/
  addr = (struct sockaddr_in *)&ifr.ifr_addr;

  /*convert ip address in correct format to write*/
  inet_pton(AF_INET, ip_address, &addr->sin_addr);

  /*Setting the Ip Address using ioctl*/
  ioctl(fd, SIOCSIFADDR, &ifr);
  /*closing fd*/
  close(fd);

  printf("IP Address updated sucessfully.\n");

  /*Getting the Ip Address after Updating.*/

  /*clear ip_address buffer with 0x20- space*/
  memset((unsigned char *)ip_address, 0x20, 15);
  ioctl(fd, SIOCGIFADDR, &ifr);
  /*Extracting Ip Address*/
  strcpy(ip_address,
         inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));

  printf("Updated IP Address is: %s\n", ip_address);

  return 0;
}
