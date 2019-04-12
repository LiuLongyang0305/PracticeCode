#include "cli.h"
int main(int argc,char *argv[])
{
	int ret = -1;
	/*
	   current_user.num = 1000;
	   strcpy(current_user.name,"lly");
	   strcpy(current_user.password,"12345");
	   */ 
	if(argc < 2)
	{
		printf("You don't input the IP!!!\n");
		return -1;
	}

	pthread_t read_pthread_id = -1;
	memset(&current_user,'\0',USER_LEN);
	ep_fd_read = epoll_create(256);
	if(ep_fd_read == -1)
	{
		perror("Failed to creatr epoll_instance:");
		return -1;
	}
	printf("ep_fd_read = %d\n",ep_fd_read);


	cli_socket_fd = socket(AF_INET,SOCK_STREAM,0);
	if(cli_socket_fd == -1)
	{
		perror("Create client scoket fd error:");
		return -1;
	}
	ret = ConnectServerHost(argv[1]);
	if(ret == -1)
	{
		printf("Failed to connect the server!!!\n ");
		close(cli_socket_fd);
		return -1;
	}
	ret = pthread_mutex_init(&mutex_active_user,NULL);
	if(ret != 0)
	{
		perror("Failed initialize the mutex:");
		close(cli_socket_fd);
		return -1;
	}
	ret = pthread_mutex_init(&mutex_message_list,NULL);
	if(ret != 0)
	{
		perror("Failed initialize the mutex:");
		close(cli_socket_fd);
		return -1;
	}
	online_head = (ACTIVE_USER*)malloc(ACTIVE_USER_LEN);
	if(online_head == NULL)
	{
		printf("Malloc space error!!!\n");
		close(cli_socket_fd);
		return -1;
	}
	memset(online_head,'\0',ACTIVE_USER_LEN);
	online_head->pNext = NULL;
	pFun fun[2] = {CreateNewAccount,UserLogin};
	int ope = -1;
	memset(&cli_udp_addr,'\0',ipv4_addr_len);
	for(int i = 0;i < 50;i ++)
	{
		cli_message_head[i] = NULL;
	}

	/******* *******************************************/
	while(pthread_flag == 0)
	{
		while(login_flag == -1)
		{
			SetBlockFife(cli_socket_fd);
			ope = ChooseOperation();
			system("clear");
			if(ope == 0)
			{
				pthread_flag = 1;
				break;
			}
			ret = fun[ope - 1](cli_socket_fd);
			{
				if(ret == 1 && ope == 2)
				{
					login_flag = 1;
					new_message_head = (MESSAGE*)malloc(MESSAGE_LEN);
					if(new_message_head == NULL)
					{
						perror("Malloc space for new message error:");
						return -1;
					}
					new_message_head->message_num = 100;
					new_message_head->pNext = NULL;
					while(1)
					{
						ret = ReadDataFromOtherUser(cli_socket_fd);
						if(ret == 0)
							break;
						
						if(ret < 0)
						{
							close(cli_socket_fd);
							return -1;
						}
					}
					//读取当前在线列表
					ret =  AddFdToEpollInstance(cli_socket_fd);
					if(ret == -1)
					{
						printf("Failed to add socket fd to epoll_instance!\n");
						close(cli_socket_fd);
						return -1;
					}	
					//调用绑定UDP端口的程序
					ret = CreateUDPCommucation();
					if(ret < 0)
					{
						printf("Failed to bind UDP port!!!\n");
						close(cli_socket_fd);
						return -1;
					}
					ret =  AddFdToEpollInstance(cli_udp_fd);
					if(ret == -1)
					{
						printf("Failed to add udp fd to epoll_instance!\n");
						close(cli_socket_fd);
						close(cli_udp_fd);
						return -1;
					}
					//开线程,以下若出错则要清pthread_flag;
					ret = pthread_create(&read_pthread_id,NULL,ReadPthreadFunction,NULL);
					if(ret != 0)
					{
						perror("Failed to create pthread:");
						close(cli_socket_fd);
						close(cli_udp_fd);
						return -1;
					}
				}
			}
		}
		while(login_flag == 1)
		{
			ret = ChooseOperationAfterLogin();
			system("clear");
			if(ret == 0)
			{
				pthread_flag = 1;
				break;
			}
			if(ret == 1)
			{
				ret = ShowActiveUser();
				system("clear");
				ShowMessageList(cli_message_head[ret]);
				char info[128];
				memset(info,'\0',128);
				printf("Please input message:\n");
				getchar();
				fgets(info,sizeof(info),stdin);
				info[strlen(info) - 1] = '\0';
				ret = SendCommonData(info,ret);
				if(ret < 0)
				{
					printf("Failed to send messages!!!\n");
					pthread_flag = 1;
					break;
				}
			}
			if(ret == 2)
			{	
				system("clear");
				char info[128];
				memset(info,'\0',128);
				printf("Please input message:\n");
				getchar();
				fgets(info,sizeof(info),stdin);
				info[strlen(info) - 1] = '\0';
				//ret = ShowActiveUser();
				ret = SendMessageToAllClient(info);
				if(ret < 0)
				{
					printf("Failed to send messages!!!\n");
					pthread_flag = 1;
					break;
				}
			}
			if(ret == 3)
			{
				system("clear");
				ShowMessageList(new_message_head);
				ClearNewMessageList();
			}
		}

	}
	pthread_flag = 1;
	return 0;
}
