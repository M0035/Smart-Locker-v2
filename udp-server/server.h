#include <stdio.h>
#include <sqlite3.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 8888
#define DBFILENAME "/home/linux/udpServer/lockers.db"

int do_login(char *buf);
int do_register(char *buf);
int do_pickup(char *buf);
int do_deliver(char *buf);
void init_lockers();
int check_all_tables();
int check_all_triggers();
