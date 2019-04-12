#include "super_user.h"
#include "menu.h"
#include "student.h"
#include "teacher.h"
#include <string.h>
#include <stdio.h>
#include "system_function.h"
#include "terminal_set.h"
void (*super_user_function[14])() =
{
	AddStudent,DeleteStudent,ModifyStudentGradeRecordBySuperUser,
	ViewAllStudentInfo,ViewSingleStudentInfo,ViewStudentGradeRecordBySubject,
	ViewStudentGradeRecordByClass,ModifyStudentPasswordBySuperUser,AddTeacher,
	DeleteTeacher,ModifyTecherInfoBySuperUser,ViewAllTeacherInfo,
	ViewSingleTeacherInfo,ModifyTeacherPasswordBySuperUser
};
SUPER_USER super_user;
//save student number ,teacher number,super user's name and password
static int(*check_login_pointer[4])(void) = {NULL,NULL,NULL,NULL};
//save function pointer


//when super user login,try 3 times at most
static int CheckLoginSuperUser()
{
	char name[20];
	char password[20];
	int login_err_count = 3;
	int check_true_flag = -1;
	MyPrintf("Please input super user's name:");
	scanf("%s",name);
	MyPrintf("Please input the password:");
	set_terminal_hide();
	scanf("%s",password);
	set_terminal_show();
	while((--login_err_count) && !((strcmp(name,super_user.name) == 0) && (strcmp(password,super_user.password) == 0)))
	{
		//compare name and password every time
		MyPrintf("Super user failed to login!");
		printf("You can try %d times!\n",login_err_count);
		MyPrintf("Please input super user's name:");
		scanf("%s",name);
		MyPrintf("Please input the password:");
		set_terminal_hide();
		scanf("%s",password);
		set_terminal_show();

	}	
	if((strcmp(name,super_user.name) == 0) && (strcmp(password,super_user.password) == 0))
	{
		MyPrintf("Super user succeed to login!");
		check_true_flag = 1;
	}
	else 
	{
		MyPrintf("Super user failed to login!");
		check_true_flag = 0;
	}

	return check_true_flag;
}
// printf the information : if the member login successfully
static void PrintfLoginInfo(  int check_true_flag,char str[10])
{
	if(check_true_flag == 0)
	{
		printf("%s",str);
		MyPrintf(" failed to login!");
	}
	if(check_true_flag == 1)
	{
		printf("%s",str);
		MyPrintf(" succeed to login!");
	}
	if(check_true_flag == -1)
	{
		printf("Failed to find the %s !\n",str);
	}
}
// teacher login by name
static int CheckLoginTeacherByName(void)
{
	char name[20];
	char password[20];
	int login_err_count = 3;
	int check_true_flag = -1;
	int  i = 0;
	MyPrintf("Please input teacher's name:");
	scanf("%s",name);
	MyPrintf("Please input teacher's password:");
	set_terminal_hide();
	scanf("%s",password);
	set_terminal_show();
	for(i = 0; i < super_user.teacher_num;i++)
	{
		if(strcmp(name,teacher_password[i].name) == 0)
		{
			while( !(strcmp(password,teacher_password[i].password) == 0) && (--login_err_count))
			{
				MyPrintf(" You input the wrong password!");
				printf(" You can try %d times!\n",login_err_count);
				MyPrintf("Please input teacher's password:");
				set_terminal_hide();
				scanf("%s",password);
				set_terminal_show();
			}
			//if the name exits,try 3 times at most
			if(strcmp(password,teacher_password[i].password) == 0)
			{
				check_true_flag = 1;
				// name and password matched
				system_current_teacher_user = teacher[i];
				break;
			}
			else
			{
				check_true_flag = 0;
				break;
			}

		}
	}
	PrintfLoginInfo(check_true_flag,"teacher");
	return check_true_flag;
}
// teacher login by num
static int CheckLoginTeacherByNum(void)
{
	int num;
	char password[20];
	int login_err_count = 3;
	int check_true_flag = -1;
	int  i = 0;
	MyPrintf("Please input teacher's num:");
	scanf("%d",&num);
	MyPrintf("Please input teacher's password:");
	set_terminal_hide();
	scanf("%s",password);
	set_terminal_show();
	for(i = 0; i < super_user.teacher_num;i++)
	{
		if(num - teacher_password[i].num == 0)
		{
			while( !(strcmp(password,teacher_password[i].password) == 0) && (--login_err_count))
			{
				MyPrintf(" You input the wrong password!");
				printf(" You can try %d times!\n",login_err_count);
				MyPrintf("Please input teacher's password:");
				set_terminal_hide();
				scanf("%s",password);
				set_terminal_show();
			}
			if(strcmp(password,teacher_password[i].password) == 0)
			{
				check_true_flag = 1;
				// name and password matched
				system_current_teacher_user = teacher[i];
				break;
			}
			else
			{
				check_true_flag = 0;
				break;
			}
		}
	}
	PrintfLoginInfo(  check_true_flag,"teacher");
	return check_true_flag;
}
// student login by name
static int CheckLoginStudentByName(void)
{
	char name[20];
	char password[20];
	int login_err_count = 3;
	int check_true_flag = -1;
	int  i = 0;
	MyPrintf("Please input student's name:");
	scanf("%s",name);
	MyPrintf("Please input student's password:");
	set_terminal_hide();
	scanf("%s",password);
	set_terminal_show();
	for(i = 0; i < super_user.student_num;i++)
	{
		if(strcmp(name,student_password[i].name) == 0)
		{
			while( !(strcmp(password,student_password[i].password) == 0) && (--login_err_count))
			{
				MyPrintf(" You input the wrong password!");
				printf(" You can try %d times!\n",login_err_count);
				MyPrintf("Please input student's password:");
				set_terminal_hide();
				scanf("%s",password);
				set_terminal_show();
			}
			if(strcmp(password,student_password[i].password) == 0)
			{				
				check_true_flag = 1;
				system_current_student_user = student[i];
				break;

			}
			else
			{
				check_true_flag = 0;
				break;
			}
		}
	}
	PrintfLoginInfo(check_true_flag,"student");
	return check_true_flag;
}
// student login by num
static int CheckLoginStudentByNum(void)
{
	int num;
	char password[20];
	int login_err_count = 3;
	int check_true_flag = -1;
	int  i = 0;
	MyPrintf("Please input student's num:");
	scanf("%d",&num);
	MyPrintf("Please input student's password:");
	set_terminal_hide();
	scanf("%s",password);
	set_terminal_show();
	for(i = 0; i < super_user.student_num;i++)
	{
		if(num - student_password[i].school_number == 0)
		{
			while( !(strcmp(password,student_password[i].password) == 0) && (--login_err_count))
			{
				MyPrintf(" You input the wrong password!");
				printf(" You can try %d times!\n",login_err_count);
				MyPrintf("Please input student's password:");
				set_terminal_hide();
				scanf("%s",password);
				set_terminal_show();
			}
			if(strcmp(password,student_password[i].password) == 0)
			{				
				check_true_flag = 1;
				system_current_student_user = student[i];
				break;

			}
			else
			{
				check_true_flag = 0;
				break;
			}
		}
	}
	PrintfLoginInfo(check_true_flag,"student");
	return check_true_flag;
}
int CheckLogin()
{
	int member = ChooseMemberLogin();
	int way = 0;
	int check_login_result = 0;

	int login_result = 0;
	check_login_pointer[0] = CheckLoginTeacherByName;
	check_login_pointer[1] = CheckLoginTeacherByNum;
	check_login_pointer[2] = CheckLoginStudentByName;
	check_login_pointer[3] = CheckLoginStudentByNum;

	//initialize the function pointer arrary
	if(member == 1)
	{
		check_login_result = CheckLoginSuperUser();
		if(check_login_result == 1)		
		{
			login_result = 1;
		}
		else
		{
			login_result = 0;
		}
	}
	if(member == 2)
	{
		way = ChooseLoginWay();
		check_login_result = check_login_pointer[way +1]();
		if(check_login_result == 1)
		{
			login_result = 2;
		}
		else
		{
			login_result = 0;
		}
	}
	if(member == 3)
	{
		way = ChooseLoginWay();
		check_login_result = check_login_pointer[way - 1]();
		if(check_login_result == 1)
		{
			login_result = 3;
		}
		else
		{
			login_result = 0;
		}
	}
	if(member == 0)
	{
		login_result = 4;
	}
	//printf("   %d \n",login_result);
	return login_result;
}
//save info relative to super user
void WriteSuperUserInfo()
{
	FILE  *wfp_super_user_info = fopen("super_user_info.txt","w");
	while(wfp_super_user_info == NULL)
	{
		wfp_super_user_info = fopen("super_user_info.txt","w");
	}
	fputs(super_user_start_str,wfp_super_user_info);
	fputc('\n',wfp_super_user_info);
	fprintf(wfp_super_user_info,"%d\t%d\t%s\t%s",super_user.teacher_num,super_user.student_num,super_user.name,super_user.password);
	fclose(wfp_super_user_info);
	MyPrintf("Succeed to write super_user_info.txt!");
}
//view the number of student or teacher in the syatem
void ViewMemberNumber()
{
	printf("%s:  %d    %s: %d\n","teacher",super_user.teacher_num,"student",super_user.student_num);
}
