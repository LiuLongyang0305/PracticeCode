#ifndef _SER_H_
#define _SER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/select.h>
#include <error.h>
#include <sys/epoll.h>
#include <ifaddrs.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>

#define SER_PORT 60000
#define CLOSE 2
#define READ  1
typedef struct sockaddr com_addr;
typedef struct sockaddr_in ipv4_addr;
typedef struct epoll_event epoll_event;
#define ipv4_addr_len sizeof(ipv4_addr)
extern int ep_fd_read,ep_fd_close;
extern epoll_event events_read[20], events_close[20];
extern pthread_mutex_t mutex;
extern int pthread_flag;

typedef struct pack_head_
{
	int type;
	int version;
	int len;
	char to_name[20];
	char from_name[20];
	int act_state;
	//本用于填补空间，后用于表明同步在线用户的状态
}PACK_HEAD;
#define PACK_HEAD_LEN sizeof(PACK_HEAD)
typedef struct user_
{
	uint32_t num;
	char name[20];
	char password[20];
	struct user_ *pNext;
}USER;
#define USER_LEN sizeof(USER)
extern USER  *read_user;
extern USER current_user;
typedef struct common_data_
{
	PACK_HEAD head;
	char data[0];
}COMMON_DATA;
typedef struct struct_data_
{
	PACK_HEAD head;
	USER user;
}STRUCT_DATA;
//注册时，user.num = 0;
typedef struct ip_info_
{
	ipv4_addr ip_info;
	struct ip_info_ *pNext;
}IP_INFO;
extern IP_INFO *ip_head;
typedef struct active_user_
{
	USER user;
	ipv4_addr addr;
	int socket_fd;
	struct active_user_ *pNext;
}ACTIVE_USER;
typedef struct active_user_data_
{
	PACK_HEAD head;
	ACTIVE_USER act_user;
}ACTIVE_USER_DATA;
#define ACTIVE_USER_LEN sizeof(ACTIVE_USER)
//used to create active user list
#define ACTIVE_USER_DATA_LEN sizeof(ACTIVE_USER_DATA)
//used to send active user to all clients
extern  USER *reg_head;
extern ACTIVE_USER *online_head;
int GetAddress(char **);
int  AddFdToEpollInstance(int fd ,int type);
int DelFdFromEpollInstance(int fd);
int  SettleReadyToReadFd(int listen_fd);
int  SettleReadyCloseFd();
void *NewPthreadFunction(void *);
int SendCommonData(char *src,char *to_name,int fd);
int ReadDataFromOtherUser(int fd);
int SendStructData(USER* user,char *to_name,int fd);
int SendActiveUserData(ACTIVE_USER* user,int fd,int ope);
static int  ReadStructDataFromOtherUsers(int fd,int type);
static int ReadActiveUserDataFromHost(int fd);
void AddNewAccount(USER *user);
void DeleteRegisterUser(USER *user);
int  WriteRegisterUserToFile();
int ReadRegisterUserfromFile();
uint32_t CreatNewAccountNum();
void ShowRegisterUser();
void CheckLogin(USER *user);
void ClearRegisteruserList();
void AddActiveUser(ACTIVE_USER *online_user);
void DeleteActiveUser(int fd);
void ShowActiveUser();
void AddConnectUserIp(IP_INFO *ip);
void FindUserIpInfo(IP_INFO *ip_info);
int SendAllActiveUser(int fd);
int SendNewJoinUser(ACTIVE_USER *user);
int SendExitUser(ACTIVE_USER *user);
#endif
