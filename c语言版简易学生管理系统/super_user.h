#ifndef _SUPER_USER_H_
#define _SUPER_USER_H_
/************************/
typedef struct superuser
{
	int student_num;
	int teacher_num;
	char name[20];
	char password[20];
}SUPER_USER;

/***********************/
extern SUPER_USER super_user;
extern void (*super_user_function[14])();
/********************************/
int  CheckLogin();
void WriteSuperUserInfo();
void ViewMemberNumber();
#endif
