#ifndef _CLI_H_
#define _CLI_H_
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


extern int cli_socket_fd;
extern int cli_udp_fd;
extern int pthread_flag;
typedef struct sockaddr com_addr;
typedef struct sockaddr_in ipv4_addr;
typedef struct epoll_event epoll_event;
#define ipv4_addr_len sizeof(ipv4_addr)
extern int login_flag;
extern int active_user_num;
extern int ep_fd_read;
extern epoll_event events_read[20];
extern pthread_mutex_t mutex_active_user;
extern pthread_mutex_t mutex_message_list;
extern ipv4_addr cli_udp_addr;
typedef struct pack_head_
{
	int type;
	int version;
	int len;
	char to_name[20];
	char from_name[20];
	int act_ope;
	//本为填补空间，后用于表明在线列表的变化状态。
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
extern USER current_user;
typedef struct common_data_
{
	PACK_HEAD head;
	char data[128];
}COMMON_DATA;
#define COMMON_DATA_LEN sizeof(COMMON_DATA)
typedef struct struct_data_
{
	PACK_HEAD head;
	USER user;
}STRUCT_DATA;
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
typedef int(*pFun)(int);
extern USER read_user;
extern ACTIVE_USER *online_user;
extern ACTIVE_USER *online_head;
//在线用户链表头
#define ACTIVE_USER_LEN sizeof(ACTIVE_USER)
//used to create active user list
#define ACTIVE_USER_DATA_LEN sizeof(ACTIVE_USER_DATA)
//used to send active user to all clients
//注册时，user.num = 0;

typedef struct message_
{
	char to_name[20];
	char from_name[20];
	char time[28];
	char data[128];
	int message_num;
	struct message_ *pNext;
}MESSAGE;
#define MESSAGE_LEN sizeof(MESSAGE)
MESSAGE *new_message_head;
MESSAGE *cli_message_head[50];
int GetAddress(char **);
int  ConnectServerHost(char ip[]);
int SendCommonData(char *src,int fd);
int ReadDataFromOtherUser(int fd);
int SendStructData(USER* user,char *to_name,int fd,int type);
static int  ReadStructDataFromOtherUsers(int fd);
static int ReadActiveUserDataFromHost(int fd,int ope);
void MyPrintf(char str[]);
static void MyPrintfStar();
static int ChangeStringToInt(char str[]);
static int CheckOperation( int num,int left,int right);
int ChooseOperation();
int UserLogin(int fd);
int CreateNewAccount(int fd);
void DeleteActiveUser(int fd);
int ShowActiveUser();
void AddActiveUser(ACTIVE_USER *online_user);
void SetNonBlockFife(int fd);
void SetBlockFife(int fd);
int  AddFdToEpollInstance(int fd);
int CreateUDPCommucation();
int  SettleReadyToReadFd();
int DelFdFromEpollInstance(int fd);
int ReadCommonDataFromOtherClients(int fd);
void *ReadPthreadFunction(void *p);
int ChooseOperationAfterLogin();
void GetCurrentTime(char cur_time[24]);
void  FindOtherClientAddressBySocketFd(ACTIVE_USER *cli_addr,int fd);
int SendMessageToAllClient(char data[]);
void  FindOtherClientAddressByIpAddress(ACTIVE_USER* act_user,ipv4_addr *addr);
void AddMessageToClientMessageList(MESSAGE* head,MESSAGE* message);
void ShowMessageList(MESSAGE *head);
void ClearNewMessageList();
void AddMessageToNewMessageList(MESSAGE* message);
#endif
