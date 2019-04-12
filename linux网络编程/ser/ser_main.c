#include "ser.h"
int main()
{
	current_user.num = 0;
	strcpy(current_user.name,"host");
	strcpy(current_user.password,"12345");


	pthread_t pthread_id = -1;
	char *host_ip = (char *)malloc(NI_MAXHOST);
	int rel = GetAddress(&host_ip);
	if(rel == EXIT_FAILURE)
	{
		printf("Failed to get host address!\n");
	}
	else
	{
		printf("host ip : %s\n",host_ip);
	}

	int listen_fd = -1;
	ipv4_addr ser_addr;
	memset(&ser_addr,'\0',ipv4_addr_len);
	ep_fd_read = epoll_create(256);
	if(ep_fd_read == -1)
	{
		perror("Failed to creatr epoll_instance:");
		return -1;
	}
	printf("ep_fd_read = %d\n",ep_fd_read);
	ep_fd_close = epoll_create(256);
	if(ep_fd_close == -1)
	{
		perror("Failed to creatr epoll_instance:");
		return -1;
	}
	printf("ep_fd_close = %d\n",ep_fd_close);
	listen_fd = socket(AF_INET,SOCK_STREAM,0);
	if(listen_fd == -1)
	{
		perror("Failed create sccket:");
		return -1;
	}
	printf("listen_fd = %d\n",listen_fd);
	rel =  AddFdToEpollInstance(listen_fd,READ);
	//设置监听
	if(rel == -1)
	{
		printf("Failed to add fd to epoll_instance!\n");
		return -1;
	}


	ser_addr.sin_family = AF_INET;
	//ser_addr.sin_addr.s_addr = INADDR_ANY;
	ser_addr.sin_addr.s_addr = inet_addr(host_ip);
	ser_addr.sin_port = htons((uint16_t)SER_PORT);
	rel = bind(listen_fd,(com_addr*)&ser_addr,ipv4_addr_len);
	if(rel < 0)
	{
		perror("Failed to bind:");
		return -1;
	}
	rel = listen(listen_fd,20);
	if(rel == -1)
	{
		perror("Failed to listen:");
		return -1;
	}
	// 创建链表头
	reg_head = (USER*)malloc(USER_LEN);
	if(reg_head == NULL)
	{
		printf("Malloc space for register user error!!\n");
		return -1;
	}
	reg_head->pNext = NULL;
	online_head = (ACTIVE_USER*)malloc(ACTIVE_USER_LEN);
	if(online_head == NULL)
	{
		printf("Malloc space for online user error!!\n");
		return -1;
	}
	online_head->pNext = NULL;
	ip_head = (IP_INFO*)malloc(sizeof(IP_INFO));
	if(ip_head == NULL)
	{
		printf("Malloc space for ip error!!\n");
		return -1;
	}
	ip_head->pNext = NULL;
	while(1)
	{
		rel = ReadRegisterUserfromFile();
		if(rel == 1)
			break;
	}
	rel = pthread_mutex_init(&mutex,NULL);
	if(rel != 0)
	{
		perror("Failed initialize the mutex:");
		return -1;
	}
	//开线程
	rel = pthread_create(&pthread_id,NULL,NewPthreadFunction,NULL);
	if(rel != 0)
	{
		perror("Failed to create pthread:");
		return -1;
	}
	rel = pthread_mutex_init(&mutex,NULL);
	if(rel != 0)
	{
		perror("Failed initialize the mutex:");
		return -1;
	}

	while(1)
	{
		pthread_mutex_lock(&mutex);
		rel = SettleReadyToReadFd(listen_fd);
		pthread_mutex_unlock(&mutex);
		if(rel == -1)
		{
			printf("Settle read_fd error!\n");
			return -1;
		}
		if(pthread_flag == 1)
			return -1;
	}
	pthread_flag = 1;
	return 0;
}
