#include "ser.h"
USER *read_user;
USER current_user;
int pthread_flag = 0;
int ep_fd_read = -1,ep_fd_close = -1;
epoll_event events_read[20], events_close[20];
pthread_mutex_t mutex;

USER *reg_head;
ACTIVE_USER *online_head;
IP_INFO *ip_head;
/**********************************
 * 描述：获取当前主机IP 
 * ******************************/
int GetAddress(char **host)
{
	struct ifaddrs *ifaddr, *ifa;
	int family, s;
	if (getifaddrs(&ifaddr) == -1)
	{
		perror("getifaddrs:");
		return EXIT_FAILURE;
	}
	for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
	{
		if (ifa->ifa_addr == NULL)
			continue;
		family = ifa->ifa_addr->sa_family;
		if (family == AF_INET) 
		{
			s = getnameinfo(ifa->ifa_addr,sizeof(struct sockaddr_in),
					*host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
			if (s != 0) 
			{
				return EXIT_FAILURE;
			}
		}
	}
	freeifaddrs(ifaddr);
	return EXIT_SUCCESS;
}

/**********************************
 * 描述：将客户端链接产生的connect_fd加入监控结合
 * ******************************/


int  AddFdToEpollInstance(int fd ,int type)
{
	epoll_event ev;
	memset(&ev,'\0',sizeof(ev));
	int ret = -1;
	ev.data.fd = fd;
	if(type == CLOSE)
	{
		ev.events = EPOLLET | EPOLLRDHUP;
		ret = epoll_ctl(ep_fd_close,EPOLL_CTL_ADD,fd,&ev);
	}
	if(type == READ)
	{
		ev.events = EPOLLIN | EPOLLET; 
		ret = epoll_ctl(ep_fd_read,EPOLL_CTL_ADD,fd,&ev);
	}
	if(ret == -1)
	{
		perror("Failed to add fd to epoll_instance:");
	}
	return ret;
}

/**********************************
 * 描述：从相应epoll_instance中删除
 *       客户端已经退出的对应的链接fd 
 *       此函数功能略微鸡肋。Linux内核自己有相应的机制：
 *       1.关闭服务端链接的fd，内核会自动将其从监控结合移除
 *       2.删除fd时，epoll_ctl()函数忽略参数ev
 * ******************************/

int DelFdFromEpollInstance(int fd)
{
	epoll_event ev;
	memset(&ev,'\0',sizeof(ev));
	int ret = -1;
	ev.data.fd = fd;
	ev.events = EPOLLIN | EPOLLET ;
	 
	ret = epoll_ctl(ep_fd_read,EPOLL_CTL_DEL,fd,&ev);
	if(ret == -1)
	{
		perror("Failed to add fd to epoll_instance:");
	}
	ev.events = EPOLLRDHUP | EPOLLET ;
	ret = epoll_ctl(ep_fd_close,EPOLL_CTL_DEL,fd,&ev);
	if(ret == -1)
	{
		perror("Failed to add fd to epoll_instance:");
	}
	return ret;
}

/**********************************
 * 描述：处理触发读事件的fd
 *       1.对应fd确实有数据写入
 *       2.客户端关闭
 * ******************************/

int  SettleReadyToReadFd(int listen_fd)
{
	int connect_fd = -1;
	int socket_fd = -1;
	ipv4_addr cli_addr;
	int len = ipv4_addr_len;
	memset(&cli_addr,'\0',ipv4_addr_len);
	int ret = 0;
	int nfound = epoll_wait(ep_fd_read,events_read,20,0);
	if(nfound > 0)
	{
		for(int i = 0;i < nfound;i ++)
		{
			if(events_read[i].data.fd == listen_fd)
			{
				connect_fd = accept(listen_fd,(com_addr*)&cli_addr,&len);
				if(connect_fd == -1)
				{
					perror("Failed to get connect_fd:");
					return -1;
				}
			    ret = AddFdToEpollInstance(connect_fd,READ);
				if(ret == -1)
				{
					printf("Failed to add fd = %d to read!!\n",connect_fd);
					return -1;
				}
			    ret = AddFdToEpollInstance(connect_fd,CLOSE);
				if(ret == -1)
				{
					printf("Failed to add fd = %d to close!!\n",connect_fd);
					return -1;
				}
				IP_INFO *ip_info = (IP_INFO*)malloc(sizeof(IP_INFO));
				if(ip_info == NULL)
				{
					printf("Malloc space error!!\n");
					return -1;
				}
				memset(ip_info,'\0',sizeof(IP_INFO));
				ip_info->ip_info = cli_addr;
				ip_info->ip_info.sin_family = (short)connect_fd;
				ip_info->pNext = NULL;
				AddConnectUserIp(ip_info);
			}
			else
			{
				socket_fd = events_read[i].data.fd;
			    ret = ReadDataFromOtherUser(socket_fd);
				if(ret <= 0)
				{
					continue;
					//当这个描述符出现异常，读取下一个描述符;
				}
				if(ret < 0)
				{
					printf("Failed to write to client: ");
					return -1;
				}
				return ret;
			}
		}
	}
}

/**********************************
 * 描述：如果客户端断开链接（正常或者非正常）
 *       删除监控集合里的对应socket_fd
 * ******************************/

int SettleReadyCloseFd()
{
	int nfound = -1;
	int ret = 0;
    nfound = epoll_wait(ep_fd_close,events_close,20,0);
	if(nfound > 0)
	{
		for(int i = 0;i < nfound;i ++)
		{
			ret = DelFdFromEpollInstance(events_close[i].data.fd);
			printf("close_fd = %d\n",events_close[i].data.fd);
			if(ret == -1)
			{
				perror("Delete closed fd error: ");
				break;
			}
			DeleteActiveUser(events_close[i].data.fd);
			ShowActiveUser();
		}
	}
	return ret;
}

/**********************************
 * 描述：监控客户端是否断开的线程入口函数
 * ******************************/
void *NewPthreadFunction(void *p)
{
	int ret = 0;
	while(1)
	{
		if(pthread_flag == 1)
			break;
		pthread_mutex_lock(&mutex);
		ret = SettleReadyCloseFd();
		pthread_mutex_unlock(&mutex);
		if(ret < 0)
		{
			printf("Settle closed fd error!\n");
			pthread_flag = 1;
			break;
		}
	}
	return NULL;
}
/**********************************
 * 描述：发送一般信息，按照字符串处理
 * ******************************/
int SendCommonData(char *src,char *to_name,int fd)
{
	int data_len = PACK_HEAD_LEN + strlen(src);
	//COMMON_DATA *send_data = (COMMON_DATA*)malloc(data_len);
	COMMON_DATA *send_data = (COMMON_DATA*)malloc(data_len + 1);
	if(send_data == NULL)
	{
		perror("Malloc space error:");
		return -1;
	}

	memset(send_data,'\0',data_len + 1);
	send_data->head.type = 3;
	send_data->head.version = 1;
	send_data->head.len = strlen(src);
	strcpy(send_data->head.to_name,to_name);
	strcpy(send_data->head.from_name,current_user.name);
	strcpy(send_data->data,src);
	int ret = write(fd,send_data,data_len);
	if(ret < 0)
	{
			perror("Send data error:");
	}
	free(send_data);
	send_data = NULL;
	return ret;
}
/**********************************
 * 描述：发送注册成功的信息
 * ******************************/
int SendStructData(USER* user,char *to_name,int fd)
{
	int data_len = 0;
	 data_len = sizeof(STRUCT_DATA);
	STRUCT_DATA *send_data = (STRUCT_DATA*)malloc(data_len);
	if(send_data == NULL)
	{
		perror("Molloc space error:");
		return -1;
	}
	memset(send_data,'\0',data_len);
	send_data->head.type = 1;
	send_data->head.version = 1;
	send_data->head.len = USER_LEN;
	strcpy(send_data->head.to_name,to_name);
	strcpy(send_data->head.from_name,current_user.name);
	memcpy(&send_data->user,user,USER_LEN);
	int ret = write(fd,send_data,data_len);
	if(ret < 0)
	{
		perror("Send data error:");
	}
	free(send_data);
	send_data = NULL;
	return ret;
}
/**********************************
 * 描述：在客户段同步在线用户，发送在线用户信息
 * ******************************/
int SendActiveUserData(ACTIVE_USER* user,int fd,int ope)
{
	int data_len = 0;
	 data_len = ACTIVE_USER_DATA_LEN;
	ACTIVE_USER_DATA *send_data = (ACTIVE_USER_DATA*)malloc(data_len);
	if(send_data == NULL)
	{
		perror("Molloc space error:");
		return -1;
	}
	memset(send_data,'\0',data_len);
	send_data->head.type = 2;
	send_data->head.version = 1;
	send_data->head.len = ACTIVE_USER_LEN;
	send_data->head.act_state = ope;
	//strcpy(send_data->head.to_name,to_name);
	strcpy(send_data->head.from_name,current_user.name);
	memcpy(&send_data->act_user,user,ACTIVE_USER_LEN);
	int ret = write(fd,send_data,data_len);
	if(ret < 0)
	{
		perror("Send data error:");
	}
	free(send_data);
	send_data = NULL;
	return ret;
}

/**********************************
 * 描述：读取客户端注册或者登陆时发送的用户信息
 * ******************************/
static int ReadStructDataFromOtherUsers(int fd,int type)
{
		read_user = (USER*)malloc(USER_LEN);
		if(read_user == NULL)
		{
			perror("Molloc space error:");
			return -1;
		}
		
		memset(read_user,'\0',USER_LEN);
		int ret = read(fd,read_user,USER_LEN);
		if(ret < 0)
		{
			perror("Read data error: ");
			return ret;
		}
		if(ret > 0)
		{
			//客户端添加到在线链表
			//服务端添加到注册列表或者在线列表
			if(type == 1)
			{
				read_user->num = CreatNewAccountNum();
				read_user->pNext = NULL;
				AddNewAccount(read_user);
				ShowRegisterUser();
				while(1)
				{
					ret = WriteRegisterUserToFile();
					if(ret == 1)
						break;
				}
			}
			if(type == 2)
			{
				CheckLogin(read_user);
			}
			ret = SendStructData(read_user,read_user->name,fd);
			if(ret < 0)
			{
				perror("write data error:");
			}
			return ret;
		}
}
/**********************************
 * 描述：读取在线用户的信息;客户端一般不会发送此类信息
 * ******************************/
 static int ReadActiveUserDataFromHost(int fd)
{
		ACTIVE_USER *online_user = (ACTIVE_USER*)malloc(ACTIVE_USER_LEN);
		if(online_user == NULL)
		{
			perror("Malloc space error:");
			return -1;
		}
		memset(online_user,'\0',ACTIVE_USER_LEN);
		int ret = read(fd,online_user,ACTIVE_USER_LEN);
		if(ret < 0)
		{
			perror("Read data error: ");
			return ret;
		}
		if(ret > 0)
		{
			printf("online_user_info : num : %d    name : %s    password:   %s ip: %s   port : %d \n",online_user->user.num,online_user->user.name,online_user->user.password,
					inet_ntoa(online_user->addr.sin_addr),ntohs(online_user->addr.sin_port));
			return ret;
		}
}
/**********************************
 * 描述：读取数据并解码，处理客户端连接 
 * ******************************/
int ReadDataFromOtherUser(int fd)
{
	PACK_HEAD data_head;
	memset(&data_head,'\0',PACK_HEAD_LEN);
	int ret = read(fd,&data_head,PACK_HEAD_LEN);
	if(ret < 0)
	{
		perror("Read pack_head error:  ");
		return ret;
	}
	if(data_head.type == 3)
	{
		char *data = (char*)malloc(data_head.len + 1);
		if(data == NULL)
		{
			perror("Malloc space error:");
			return -1;
		}
		memset(data,'\0',data_head.len + 1);
		ret = read(fd,data,data_head.len);
		if(ret < 0)
		{
			perror("Read data error: ");
			return ret;
		}
		if(ret > 0)
		{
			printf("%s   :    %s\n",data_head.from_name,data);
			free(data);
			data = NULL;
			return ret;
		}
	}
	if(data_head.type == 1 || data_head.type == 2)
	{
		ret =  ReadStructDataFromOtherUsers(fd,data_head.type);
		if(ret < 0)
			return ret;
		if(data_head.type == 2)
		{
			if(read_user->num == 0)
				return ret;
			else
			{
				ACTIVE_USER *act_user = (ACTIVE_USER*)malloc(ACTIVE_USER_LEN);
				act_user->pNext = NULL;
				act_user->user = *read_user;
				IP_INFO ip;
				memset(&ip,'\0',sizeof(ip));
				ip.ip_info.sin_family = fd;
				FindUserIpInfo(&ip);
				act_user->addr = ip.ip_info;
				act_user->socket_fd = fd;
				act_user->addr.sin_family = AF_INET;
				AddActiveUser(act_user);
				ShowActiveUser();
				SendAllActiveUser(fd);
				SendNewJoinUser(act_user);
			}
		}
	}
	/*
	if(data_head.type == 2)
	{
		return ReadActiveUserDataFromHost(fd);
	}
	*/
}
/********** operation on register user *******************/
/**********************************
 * 描述：添加新用户至注册用户链表 
 * ******************************/
void AddNewAccount(USER *user)
{
	USER *p = reg_head;
	int ret = -1;
	while(1)
	{
		if(p->pNext == NULL)
			break;
		p = p->pNext;
	}
	p->pNext = user;
	//p->pNext = NULL;
	//printf("Succeed to add new register user !!!\n");
}
/**********************************
 * 描述：注销当前用户 
 * ******************************/
void DeleteRegisterUser(USER *user)
{
	USER *p = reg_head;
	USER *q = reg_head->pNext;
	int ret = -1;
	while(1)
	{
		if(q == NULL)
			break;
		if(q->num == user->num)
			break;
		p = p->pNext;
		q = q->pNext;
	}
	if(q == NULL)
	{
		printf("The user you wan to delete doesn't exist!!!\n");
	}
	else
	{
		p->pNext = q->pNext;
		free(q);
		printf("Succeed to delete user named %s !!!\n",user->name);
		free(user);
	}
	while(1)
	{
		ret = WriteRegisterUserToFile();
		if(ret == 1)
			break;
	}
}
/**********************************
 * 描述：保存注册用户 
 * ******************************/
int  WriteRegisterUserToFile()
{
	USER *p = reg_head->pNext;
	FILE *wfp_user = NULL;
	int ret = -1;
	while(1)
	{
		 wfp_user = fopen("user_info.txt","w");
		 if(wfp_user != NULL)
			 break;
	}
	while(1)
	{
		if(p == NULL)
			break;
		ret = fprintf(wfp_user,"%d\t%s\t\t%s\t\t\n",p->num,p->name,p->password);
		if(ret <= 0)
		{
			printf("Failed to write register user file!!!!\n");
			fclose(wfp_user);
			return -1;
		}
		p = p->pNext;
	}
	fclose(wfp_user);
	return 1;
}
/**********************************
 * 描述：系统初始化，从文件读取注册用户信息并加入注册用户链表，用于用户登录的检测
 * ******************************/
int ReadRegisterUserfromFile()
{
	FILE *rfp_user = NULL;
	int ret = -1;
	while(1)
	{
		//读取文件之前，必须建立文件，否则死循环
		 rfp_user = fopen("user_info.txt","r");
		 if(rfp_user != NULL)
			 break;
	}
	while(1)
	{
		USER *user = (USER*)malloc(USER_LEN);
		user->num = 0;
		ret = fscanf(rfp_user,"%d%s%s",&(user->num),user->name,user->password);
		if(user->num == 0)
		{
			free(user);
			user = NULL;
			break;
		}
		user->pNext = NULL;
		AddNewAccount(user);
	}
	fclose(rfp_user);
	return 1;
}

/**********************************
 * 描述：自动生成注册用户的帐号
 * ******************************/
uint32_t CreatNewAccountNum()
{
	srand(time(NULL));
	uint32_t num = rand();
	num = 1000 + 1000 * (num / (RAND_MAX + 1.0));
	return num;
}

/**********************************
 * 描述：显示已经注册账户
 * ******************************/
void ShowRegisterUser()
{
	USER *p = reg_head->pNext;
	while(1)
	{
		if(p == NULL)
			break;
		printf("user_info: %d\t%s\t\t%s\n",p->num,p->name,p->password);
		p = p->pNext;
	}
}
/**********************************
 * 描述：查找特定用户，用于登录时判断
 * ******************************/
void CheckLogin(USER *user)
{
	USER *p = reg_head->pNext;
	while(1)
	{
		if(p == NULL)
			break;
		if(!strcmp(p->name,user->name))
		{
			if(!strcmp(p->password,user->password))
			{
				*user = *p;
				break;
			}
		}
		p = p->pNext;
	}
}
/**********************************
 * 描述：回收除链表头以外的所有节点
 * ******************************/
void ClearRegisteruserList()
{
	USER *p = reg_head->pNext;
	if(p == NULL)
		return;
	reg_head->pNext = p->pNext;
	free(p);
	p = NULL;
	ClearRegisteruserList();
}
/********** operation on online user *******************/
/**********************************
 * 描述：添加新用户至注册用户链表 
 * ******************************/
void AddActiveUser(ACTIVE_USER *online_user)
{
	ACTIVE_USER *p = online_head;
	int ret = -1;
	while(1)
	{
		if(p->pNext == NULL)
			break;
		p = p->pNext;
	}
	p->pNext = online_user;
	//p->pNext = NULL;
	//printf("Succeed to add new register user !!!\n");
}
/**********************************
 * 描述：删除在线用户
 * ******************************/
void DeleteActiveUser(int fd)
{
	ACTIVE_USER *p = online_head;
	ACTIVE_USER *q = online_head->pNext;
	int ret = -1;
	while(1)
	{
		if(q == NULL)
			break;
		if(q->socket_fd == fd)
			break;
		p = p->pNext;
		q = q->pNext;
	}
	if(q == NULL)
	{
		printf("The online_user you wan to delete doesn't exist!!!\n");
	}
	else
	{
		printf("User nemed %s exit system!!!\n",q->user.name);
		p->pNext = q->pNext;
		SendExitUser(q);	
		free(q);
	}
}
/**********************************
 * 描述：显示在线用户
 * ******************************/
void ShowActiveUser()
{
	ACTIVE_USER *p = online_head->pNext;
	while(1)
	{
		if(p == NULL)
			break;
		printf("user_info: %d\t%s\t\t%s\t[%s]:[%d]\n",p->user.num,p->user.name,p->user.password,
				inet_ntoa(p->addr.sin_addr),ntohs(p->addr.sin_port));
		p = p->pNext;
	}
}
/********** operation on ipv4 addr *******************/
/**********************************
 * 描述：链接成功则加入链表
 * ******************************/
void AddConnectUserIp(IP_INFO *ip)
{
	IP_INFO *p = ip_head;
	int ret = -1;
	while(1)
	{
		if(p->pNext == NULL)
			break;
		p = p->pNext;
	}
	p->pNext = ip;
	//p->pNext = NULL;
	//printf("Succeed to add new register user !!!\n");
}
/**********************************
 * 描述：查找特定用户
 * ******************************/
void FindUserIpInfo(IP_INFO *ip_info)
{
	IP_INFO *p = ip_head->pNext;
	while(1)
	{
		if(p == NULL)
			break;
		if(p->ip_info.sin_family == ip_info->ip_info.sin_family)
		{
			(*ip_info).ip_info = p->ip_info;
			break;
		}
		p = p->pNext;
	}
}
/**********************************
 * 描述：在自己加入在线列表之后，向新上线的用户发送在线链表
 * ******************************/
int SendAllActiveUser(int fd)
{
	ACTIVE_USER *p = online_head->pNext;
	int ope = -1;
	int ret = -1;
	while(1)
	{
		if(p == NULL)
			break;
		if(p->pNext == NULL)
			ope = 0;
		else
		{
			ope = 1;
		}
		ret = SendActiveUserData(p,fd,ope);
		if(ret < 0)
		{
			printf("Send active user list error!!!\n");
			return -1;
		}
		p = p->pNext;
	}
}

/**********************************
 * 描述：当有新用户上线，向除了该用户的所有用户发送该用户上线消息
 * ******************************/
int SendNewJoinUser(ACTIVE_USER *user)
{
	ACTIVE_USER *p = online_head->pNext;
	int ret = -1;
	while(1)
	{
		if(p->pNext == NULL)
			break;
		ret = SendActiveUserData(user,p->socket_fd,2);
		if(ret < 0)
		{
			printf("Send active user list error!!!\n");
			return -1;
		}
		p = p->pNext;	
	}
}
/**********************************
 * 描述：当有用户退出，向所有在线用户的发送该消息
 * ******************************/
int SendExitUser(ACTIVE_USER *user)
{
	ACTIVE_USER *p = online_head->pNext;
	int ret = -1;
	while(1)
	{
		if(p == NULL)
			break;
		ret = SendActiveUserData(user,p->socket_fd,3);
		if(ret < 0)
		{
			printf("Send active user list error!!!\n");
			return -1;
		}
		p = p->pNext;	
	}
}















