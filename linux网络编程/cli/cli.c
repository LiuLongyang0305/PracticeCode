#include "cli.h"
int active_user_num = 0;
int cli_socket_fd = -1;
USER current_user;
int login_flag = -1;
ACTIVE_USER *online_user;
//保存当前用户，调试时main（）函数中给定，最终版本为成功的用户
USER read_user;
ACTIVE_USER *online_head;
int ep_fd_read = -1;
epoll_event events_read[20];
pthread_mutex_t mutex_active_user;;
pthread_mutex_t mutex_message_list;
ipv4_addr   cli_udp_addr;
int cli_udp_fd = -1;
int pthread_flag = 0;
MESSAGE *new_message_head = NULL;
MESSAGE *cli_message_head[50];
/*********************************
 * 描述：以下几个函数用于显示菜单并且选定操作
 * ******************************/
void MyPrintf(char str[])
{
	printf("%s\n",str);
}
static void MyPrintfStar()
{
	printf("%s\n","**************************************");
}
static int ChangeStringToInt(char str[])
{
	int str_len = strlen(str);
	int rel = 0;
	if(str_len >= 2)
	{
		rel = (*(str + str_len -2) - '0') * 10 + (*(str + str_len - 1) - '0');
	}
	else
	{
		rel = *str - '0';
	}
	return rel;
}
static int CheckOperation( int num,int left,int right)
{
	return (num >= left && num <=right) ? num : -1;
}

/*********************************
 * 描述：菜单显示 
 * ******************************/
int ChooseOperation()
{
	int rel = 0;
	char operation[20];
	MyPrintf("          ChooseOperation             ");
	MyPrintfStar();
	MyPrintf("***    1.new user to register      ***");
	MyPrintf("***    2.old user to login         ***");
	MyPrintf("***    0.exit system               ***");
	MyPrintfStar();
	while(1)
	{
		MyPrintf("Please input the num:");
		scanf("%s",operation);
		rel = CheckOperation(ChangeStringToInt(operation),0,2);
		if(rel != -1)
		{
			break;
		}
		else
		{
			MyPrintf("You input the wrong number!");
		}
	}
	return rel;
}
/*********************************
 * 描述：登录成功后菜单 
 * ******************************/
int ChooseOperationAfterLogin()
{
	int rel = 0;
	char operation[20];
	MyPrintf("          ChooseOperation             ");
	MyPrintfStar();
	MyPrintf("***    1.chat with someone         ***");
	MyPrintf("***    2.send message to all       ***");
	MyPrintf("***    3.view new messages         ***");
	MyPrintf("***    0.exit system               ***");
	MyPrintfStar();
	while(1)
	{
		MyPrintf("Please input the num:");
		scanf("%s",operation);
		rel = CheckOperation(ChangeStringToInt(operation),0,3);
		if(rel != -1)
		{
			break;
		}
		else
		{
			MyPrintf("You input the wrong number!");
		}
	}
	return rel;
}
/**********************************
 * 描述：将cli_socket_fd && cli_udp_fd加入监控 
 * ******************************/


int  AddFdToEpollInstance(int fd)
{
	epoll_event ev;
	memset(&ev,'\0',sizeof(ev));
	int ret = -1;
	ev.data.fd = fd;
	ev.events = EPOLLIN | EPOLLET; 
	ret = epoll_ctl(ep_fd_read,EPOLL_CTL_ADD,fd,&ev);
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
	return ret;
}
/**********************************
 * 描述：处理触发读事件的fd
 *       1.对应fd确实有数据写入
 *       2.服务端更新在线用户列表
 * ******************************/

int  SettleReadyToReadFd()
{
	int ret = -1;
	int nfound = epoll_wait(ep_fd_read,events_read,20,0);
	if(nfound > 0)
	{
		for(int i = 0;i < nfound;i ++)
		{
			if(events_read[i].data.fd == cli_socket_fd)
			{
				ret = ReadDataFromOtherUser(cli_socket_fd);
				if(ret < 0)
				{
					printf("Read from server error!!!!\n");
					return ret;
				}
			}
			else
			{
				ret = ReadCommonDataFromOtherClients(cli_udp_fd);
				if(ret < 0)
				{
					printf("Read from other clients error!!!\n");
					return ret;
				}
			}
		}
	}
}
/*********************************
 * 描述：读数据线程入口函数
 * ******************************/
void *ReadPthreadFunction(void *p)
{
	int ret = 0;
	while(1)
	{
		if(pthread_flag == 1)
			break;
		//pthread_mutex_lock(&mutex);
		ret = SettleReadyToReadFd();
		//pthread_mutex_unlock(&mutex);
		if(ret < 0)
		{
			printf("Settle ready to read  fd error!\n");
			pthread_flag = 1;
			break;
		}
	}
	return NULL;
}
/*********************************
 * 描述：设置文件属性为非阻塞 
 * ******************************/
void SetNonBlockFife(int fd)
{
	int old_option = fcntl(fd,F_GETFL);
	int new_option = old_option | O_NONBLOCK;
	fcntl(fd,F_SETFL,new_option);
}
/*********************************
 * 描述：设置文件属性为阻塞 
 * ******************************/
void SetBlockFife(int fd)
{
	int old_option = fcntl(fd,F_GETFL);
	int new_option = old_option &( ~O_NONBLOCK);
	fcntl(fd,F_SETFL,new_option);
}
/*********************************
 * 描述：创建新新用户 
 * ******************************/
int CreateNewAccount(int fd)
{
	//客户端的网络套接字
	USER new_user;
	memset(&new_user,'\0',USER_LEN);
	char ver_password[20];
	int ret = -1;
	MyPrintf("Please input the nickname(less than 19 characters):");
	scanf("%s",new_user.name);
	while(1)
	{
		MyPrintf("Please input the password(less than 19 characters):");
		scanf("%s",new_user.password);
		MyPrintf("Please input the password again:");
		scanf("%s",ver_password);
		if(!strcmp(ver_password,new_user.password))
			break;
		else
		{
			MyPrintf("You input the different password!!");
		}
	}
	new_user.num = 0;//注册成功会返回用户帐号
	ret = SendStructData(&new_user,"host",fd,1);
	if(ret < 0)
	{
		MyPrintf("Failed to send new user info to host!");
		return -1;
	}
	ret = ReadDataFromOtherUser(fd);
	//printf("user_info : num : %d    name : %s    password:   %s\n",read_user.num,read_user.name,read_user.password);
	if(read_user.num == 0)
	{
		printf("Failed to create new account!\n");
		return -1;
	}
	else
	{
		printf("Succeed to create a new account!\n");
		return 1;
	}
}
/*********************************
 * 描述：用户登录
 * ******************************/
int UserLogin(int fd)
{
	USER old_user;
	memset(&old_user,'\0',USER_LEN);
	int ret = -1;
	MyPrintf("Please input the nickname(less than 19 characters):");
	scanf("%s",old_user.name);
	MyPrintf("Please input the password(less than 19 characters):");
	scanf("%s",old_user.password);
	old_user.num = 0;//登录成功返回帐号，用以区分是否登录成功
	ret = SendStructData(&old_user,"host",fd,2);
	if(ret < 0)
	{
		MyPrintf("Failed to send new user info to host!");
		return -1;
	}
	memset(&old_user,'\0',USER_LEN);
	ret = ReadDataFromOtherUser(fd);
	if(read_user.num == 0)
	{
		printf("Failed to login!\n");
		return -1;
	}
	else
	{
		printf(" User named %s succeed to login!\n",read_user.name);
		memset(&current_user,'\0',USER_LEN);
		current_user = read_user;
		return 1;
	}
}

/*********************************
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
/*********************************
 * 描述：链接服务端，输入有参数为服务端IP 
 *       由main（）函数命令行参数提供
 * ******************************/
int  ConnectServerHost(char ip[])
{
	
	char *host = (char *)malloc(NI_MAXHOST);
	memset(host,'\0',NI_MAXHOST);
	int rel = -1;
	/*
	int rel = GetAddress(&host);
	if(rel == EXIT_FAILURE)
	{
		printf("Failed to get adress!\n");
		return -1;
	}
	*/
	strcpy(host,ip);
	ipv4_addr ser_addr;
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons((uint16_t)SER_PORT);
	inet_pton(AF_INET,host,&ser_addr.sin_addr.s_addr);
	rel = connect(cli_socket_fd,(com_addr*)&ser_addr,ipv4_addr_len);
	if(rel == -1)
	{
		perror("Connect error: ");
	}
	return rel;
}
/*********************************
 * 描述：向其他客户端发送数据，所有数据均视为字符串
 * ******************************/
int SendCommonData(char *src,int other_cli_fd)
{
	//int data_len = COMMON_DATA_LEN;
	COMMON_DATA *send_data = (COMMON_DATA*)malloc(COMMON_DATA_LEN);
	if( send_data == NULL)
	{
		perror("Malloc space error:");
		return -1;
	}
	memset(send_data,'\0',COMMON_DATA_LEN);
	send_data->head.type = 3;
	send_data->head.version = 1;
	send_data->head.len = 128;
	ACTIVE_USER act_user;
	memset(&act_user,'\0',ACTIVE_USER_LEN);
	FindOtherClientAddressBySocketFd(&act_user,other_cli_fd);
	if(act_user.socket_fd == other_cli_fd)
	{
		strcpy(send_data->head.to_name,act_user.user.name);
	}
	strcpy(send_data->head.from_name,current_user.name);
	strcpy(send_data->data,src);
	int ret = sendto(cli_udp_fd,send_data,COMMON_DATA_LEN,0,(com_addr*)(&act_user.addr),ipv4_addr_len);
	if(ret < 0)
	{
		perror("Send data error:");
		return ret;
	}
	MESSAGE *message = (MESSAGE*)malloc(MESSAGE_LEN);
	if(message == NULL)
	{
		perror("Malloc space for message error:");
		return -1;
	}
	memset(message,'\0',MESSAGE_LEN);
	message->pNext = NULL;
	strcpy(message->to_name,send_data->head.to_name);
	strcpy(message->from_name,current_user.name);
	strcpy(message->data,src);
	GetCurrentTime(message->time);
	AddMessageToClientMessageList(cli_message_head[act_user.socket_fd],message);
	free(send_data);
	send_data = NULL;
	return ret;
}
/*********************************
 * 描述：注册用户或登录时发送客户端信息
 * ******************************/
int SendStructData(USER* user,char *to_name,int fd,int type)
{
	int data_len = sizeof(STRUCT_DATA);
	STRUCT_DATA *send_data = (STRUCT_DATA*)malloc(data_len);
	memset(send_data,'\0',data_len);
	send_data->head.type = type;
	send_data->head.version = 1;
	send_data->head.len = USER_LEN;
	strcpy(send_data->head.to_name,to_name);
	strcpy(send_data->head.from_name,current_user.name);
	send_data->user = *user;
	int ret = write(fd,send_data,data_len);
	if(ret < 0)
	{
		perror("Send data error:");
	}
	free(send_data);
	send_data = NULL;
	return ret;
}
/*********************************
 * 描述：读取一般结构体数据
 * ******************************/
static int ReadStructDataFromOtherUsers(int fd)
{
	memset(&read_user,'\0',USER_LEN);
	read_user.num = 0;
	int ret = read(fd,&read_user,USER_LEN);
	if(ret < 0)
	{
		perror("Read data error: ");
		return ret;
	}
	if(ret > 0)
	{
		//客户端添加到在线链表
		//服务端添加到注册列表或者在线列表
		printf("user_info : num : %d    name : %s    password:   %s\n",read_user.num,read_user.name,read_user.password);
		return ret;
	}
}
/*********************************
 * 描述：读取UDP数据
 * ******************************/
int ReadCommonDataFromOtherClients(int fd)
{
	COMMON_DATA *data = (COMMON_DATA*)malloc(COMMON_DATA_LEN);
	if(data == NULL)
	{
		perror("Mclloc space error:");
		return -1;
	}
	memset(data,'\0',COMMON_DATA_LEN);
	ipv4_addr udp_client_addr;
	memset(&udp_client_addr,'\0',ipv4_addr_len);
	int len = ipv4_addr_len;
	int ret = recvfrom(fd,data,COMMON_DATA_LEN,0,(com_addr*)(&udp_client_addr),&len);
	if(ret < 0)
	{
		perror("Read UDP data error:");
		free(data);
		return ret;
	}
	ACTIVE_USER act_user;
	memset(&act_user,'\0',ACTIVE_USER_LEN);
	FindOtherClientAddressByIpAddress(&act_user,&udp_client_addr);
	MESSAGE *message = (MESSAGE*)malloc(MESSAGE_LEN);
	if(message == NULL)
	{
		perror("Malloc space for message error:");
		return -1;
	}
	memset(message,'\0',MESSAGE_LEN);
	//message->pNext = NULL;
	strcpy(message->to_name,data->head.to_name);
	strcpy(message->from_name,data->head.from_name);
	strcpy(message->data,data->data);
	GetCurrentTime(message->time);
	message->pNext = NULL;
	AddMessageToClientMessageList(cli_message_head[act_user.socket_fd],message);
	MESSAGE *new_message = (MESSAGE*)malloc(MESSAGE_LEN);
	if(new_message == NULL)
	{
		perror("Malloc space for message error:");
		return -1;
	}
	*new_message = *message;
	AddMessageToNewMessageList(new_message);
	free(data);
	//printf("info: %s\n",data->data);
}
/*********************************
 * 描述：读取服务端同步在线用户时的数据 
 * ******************************/
static int ReadActiveUserDataFromHost(int fd,int ope)
{
	online_user = (ACTIVE_USER*)malloc(ACTIVE_USER_LEN);
	if(online_user == NULL)
	{
		printf("Malloc space error!!!\n");
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
		if(ope == 1 || ope == 0 || ope == 2 )
		{
			online_user->pNext = NULL;
			AddActiveUser(online_user);
			active_user_num ++;
			if(ope == 0)
			{
				cli_udp_addr = online_user->addr;
			}
			cli_message_head[online_user->socket_fd] = (MESSAGE*)malloc(MESSAGE_LEN);
			if(cli_message_head[online_user->socket_fd] == NULL)
			{
				perror("Malloc space  to store message error:");
				return -1;
			}
			cli_message_head[online_user->socket_fd]->pNext = NULL;
		}
		if(ope == 3)
		{
			DeleteActiveUser(online_user->socket_fd);
			active_user_num --;
			free(online_user);
		}
		//ShowActiveUser();
		online_user = NULL;
		return ope;
	}
}

/*********************************
 * 描述：读取数据并解码 
 * ******************************/
int ReadDataFromOtherUser(int fd)
{
	PACK_HEAD data_head;
	memset(&data_head,'\0',PACK_HEAD_LEN);
	int ret = read(fd,&data_head,PACK_HEAD_LEN);
	if(ret < 0)
	{
		//perror("Read pack_head error:  ");
		return ret;
	}
	if(data_head.type == 3)
	{
		char *data = (char*)malloc(data_head.len + 1);
		if(data == NULL)
		{
			perror("Molloc space error:");
			return -1;
		}
		memset(data,'\0',data_head.len + 1);
		ret = read(fd,data,data_head.len);
		if(ret < 0)
		{
			perror("Read dgta error: ");
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
	if(data_head.type == 1 )
	{
		return ReadStructDataFromOtherUsers(fd);
	}	
	if(data_head.type == 2)
	{
		return  ReadActiveUserDataFromHost(fd,data_head.act_ope);
	}	
}


/**********************************
 * 描述：获取在线用户并创建链表
 * ******************************/
void AddActiveUser(ACTIVE_USER *online_user)
{
	ACTIVE_USER *p = online_head;
	int ret = -1;
	//pthread_mutex_lock(&mutex_active_user);
	while(1)
	{
		if(p->pNext == NULL)
			break;
		p = p->pNext;
	}
	p->pNext = online_user;
	//pthread_mutex_unlock(&mutex_active_user);
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
	//pthread_mutex_lock(&mutex_active_user);
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
		free(q);
	}
	//pthread_mutex_unlock(&mutex_active_user);
}
/**********************************
 * 描述：显示在线用户
 * ******************************/
int ShowActiveUser()
{
	ACTIVE_USER *p = online_head->pNext;
	int i = 1;
	int rel = -1;
	char operation[20];
	int fd[20]= {0};
	memset(operation,'\0',20);
	MyPrintf("          CurrentActiveUser             ");
	MyPrintfStar();
	//pthread_mutex_lock(&mutex_active_user);
	while(1)
	{
		if(p == NULL)
			break;
		/*
		   printf("user_info: %d\t%s\t\t%s\t[%s]:[%d]\n",p->user.num,p->user.name,p->user.password,
		   inet_ntoa(p->addr.sin_addr),ntohs(p->addr.sin_port));
		   */
		else
		{
			printf("\t %d.\t%s\t\t\n",i,p->user.name);
			fd[i] = p->socket_fd;
			i ++;
			p = p->pNext;
		}
	}
	MyPrintfStar();

	//pthread_mutex_unlock(&mutex_active_user);
	while(1)
	{
		MyPrintf("Please input the num:");
		scanf("%s",operation);
		rel = CheckOperation(ChangeStringToInt(operation),1,active_user_num );
		if(rel != -1)
		{
			break;
		}
		else
		{
			MyPrintf("You input the wrong number!");
		}
	}
	return fd[rel];
}


/**********************************
 * 描述：绑定UDP通信端口
 * ******************************/
int CreateUDPCommucation()
{
	int ret = -1;
	int opt = 1;
	cli_udp_fd = socket(AF_INET,SOCK_DGRAM,0);
	if(cli_udp_fd  < 0)
	{
		perror("Create UDP fd error!");
		return cli_udp_fd;
	}
	printf("cli_udp_fd = %d \n",cli_udp_fd);
	ret = setsockopt(cli_udp_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	if(ret  < 0)
	{
		perror("Set UDP fd error!");
		close(cli_udp_fd);
		return ret;
	}
	ret = bind(cli_udp_fd,(com_addr*)(&cli_udp_addr),ipv4_addr_len);
	if(ret < 0)
	{
		perror("Bind UDP fd error!!!");
		close(cli_udp_fd);
		return ret;
	}
}

/**********************************
 * 描述：获取当前时间
 * ******************************/
void GetCurrentTime(char cur_time[])
{
	//printf("call gettime()\n");
	time_t now = time(NULL);
	//char now_str[30];
	//memset(now_str,'\0',30);
	//strcpy(now_str,ctime(&now));
	strcpy(cur_time,ctime(&now));
	/*
	now_str[3] = '-';
	now_str[7] = '-';
	now_str[10] = '-';
	now_str[19] = '-';
	*/
//	strcpy(cur_time,now_str);
}

/**********************************
 * 描述：通过fd查找聊天对象
 * ******************************/
void  FindOtherClientAddressBySocketFd(ACTIVE_USER* act_user,int fd)
{
	ACTIVE_USER *p = online_head->pNext;

	//pthread_mutex_lock(&mutex_active_user);
	while(1)
	{
		if(p == NULL)
			break;
		if(p->socket_fd == fd)
			break;
		p = p->pNext;
	}
	if(p == NULL)
	{
		act_user = NULL;
	}
	else
	{
		*act_user = *p;
	}
	//pthread_mutex_unlock(&mutex_active_user);
}

/**********************************
 * 描述：发送消息给所有人
 *       中间走了弯路;
 * ******************************/
int SendMessageToAllClient(char data[])
{
	ACTIVE_USER *p = online_head->pNext;
	int ret = -1;
	//pthread_mutex_lock(&mutex_active_user);
	while(1)
	{
		if(p == NULL)
			break;
		if(p->socket_fd != cli_socket_fd)
		{
			ret = SendCommonData(data,p->socket_fd);
			if(ret < 0)
			{
				pthread_mutex_unlock(&mutex_active_user);
				return ret;
			}
		}
		p = p->pNext;
	}
	//pthread_mutex_unlock(&mutex_active_user);
	return ret;
}

/**********************************
 * 描述：添加消息记录至好友消息链表
 * ******************************/
void AddMessageToClientMessageList(MESSAGE* head,MESSAGE* message)
{

	//pthread_mutex_lock(&mutex_message_list);
	MESSAGE *p = NULL;
	
		if(head->message_num == 10)
		{
			p = head->pNext;
			head->pNext = p->pNext;
			free(p);
			p = NULL;
			head->message_num --;
		}
	p = head;
	while(1)
	{
		if(p->pNext == NULL)
			break;
		else
		{
			p = p->pNext;
		}
	}
	p->pNext = message;
	head->message_num ++;
	//pthread_mutex_unlock(&mutex_message_list);
}
/**********************************
 * 描述：添加消息记录至新消息链表
 * ******************************/
void AddMessageToNewMessageList(MESSAGE* message)
{
	//pthread_mutex_lock(&mutex_message_list);
	MESSAGE *p = NULL;
	p = new_message_head;
	while(1)
	{
		if(p->pNext == NULL)
			break;
		else
		{
			p = p->pNext;
		}
	}
	p->pNext = message;
	//pthread_mutex_unlock(&mutex_message_list);
}


/**********************************
 * 描述：通过IP信息查找聊天对象
 * ******************************/
void  FindOtherClientAddressByIpAddress(ACTIVE_USER* act_user,ipv4_addr *addr)
{
	ACTIVE_USER *p = online_head->pNext;
	//pthread_mutex_lock(&mutex_active_user);
	while(1)
	{
		if(p == NULL)
			break;
		if((p->addr.sin_addr.s_addr == addr->sin_addr.s_addr) && (p->addr.sin_port == addr->sin_port))
			break;
		p = p->pNext;
	}
	if(p == NULL)
	{
		act_user = NULL;
	}
	else
	{
		*act_user = *p;
	}
	//pthread_mutex_unlock(&mutex_active_user);
}

/**********************************
 * 描述：打印聊天消息
 * ******************************/
void ShowMessageList(MESSAGE *head)
{
	MESSAGE *p = head->pNext;
	char buf[256];
	memset(buf,'\0',256);
	if(p == NULL)
	{
		printf("Message list is empty!!\n");
	}
	else
	{
		//pthread_mutex_lock(&mutex_message_list);
		while(1)
		{
			if(p == NULL)
			{
				break;
			}
			else
			{
				sprintf(buf,"%s   %s   :   %s",p->time,p->from_name,p->data);
				//printf("%s\t%s\n",p->from_name,p->time);
				printf("%s\n",buf);
				p = p->pNext;
			}
		}
		//pthread_mutex_unlock(&mutex_message_list);
	}
}

/**********************************
 * 描述：新消息打印完成清空链表
 * ******************************/
void ClearNewMessageList()
{
	MESSAGE *p = NULL;
	//pthread_mutex_lock(&mutex_message_list);
	while(1)
	{
		if(new_message_head->pNext == NULL)
			break;
		p = new_message_head->pNext;
		new_message_head->pNext = p->pNext;
		free(p);
		p = NULL;
	}
	//pthread_mutex_unlock(&mutex_message_list);
}






