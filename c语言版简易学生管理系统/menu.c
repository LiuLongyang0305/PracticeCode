#include <stdio.h>
#include "menu.h"
#include <string.h>
int (*menu_function[9])()={ChooseMemberLogin,ChooseOperationSuperUser,ChooseOperationStudent,
	ChooseOperationTeacher,ChooseLoginWay,ChooseOperationWay,SuperUserModifyTeacherInfo,
	SuperUserModifyStudentGradeRecord,SuperUserViewStudentGradeRecordBySubject};
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
void MyPrintf(char str[])
{
	printf("%s\n",str);
}
static void MyPrintfStar()
{
	printf("%s\n","**************************************");
}
//select whom to login 
int ChooseMemberLogin()
{
	//int num;
	int rel = 0;
	char name[20];
	MyPrintf("          ChooseMemberLogin           ");
	MyPrintfStar();
	MyPrintf("******       1.super user       ******");
	MyPrintf("******       2.student          ******");
	MyPrintf("******       3.teacher          ******");
	MyPrintf("******       0.exit system      ******");
	MyPrintfStar();
	while(1)
	{
		MyPrintf("Please input the num:");
		scanf("%s",name);
		rel = CheckOperation(ChangeStringToInt(name),0,3);
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
//when super user login, choose his operation 
int ChooseOperationSuperUser()
{
	//int num;
	int rel = 0;
	char name[20];
	MyPrintf("      SuperUserOperation               ");
	MyPrintfStar();
	MyPrintf("***   1.add student                ***");
	MyPrintf("***   2.delete student             ***");
	MyPrintf("***   3.modify student score       ***");	
	MyPrintf("***   4.view all student info      ***");
	MyPrintf("***   5.view single student info   ***");
	MyPrintf("***   6.view student by subject    ***");
	MyPrintf("***   7.view student by class      ***");
	MyPrintf("***   8.modify student password    ***");
	MyPrintf("***   9.add teacher                ***");
	MyPrintf("***   10.delete teacher            ***");
	MyPrintf("***   11.modify teacher info       ***");
	MyPrintf("***   12.view all teacher info     ***");
	MyPrintf("***   13.view single teacher  info ***");
	MyPrintf("***   14.modify teacher password   ***");
	MyPrintf("***   0.exit  to login menu        ***");
	MyPrintfStar();
	while(1)
	{
		MyPrintf("Please input the num:");
		scanf("%s",name);
		rel = CheckOperation(ChangeStringToInt(name),0,14);
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
//when teacher login, choose his operation
int ChooseOperationTeacher()
{
	//int num;
	int rel = 0;
	char name[20];
	MyPrintf("      TeacherOperation                ");
	MyPrintfStar();
	MyPrintf("***   1.view student grade record  ***");
	MyPrintf("***   2.view single student info   ***");
	MyPrintf("***   3.modify student grade       ***");
	MyPrintf("***   4.modify password(self)      ***");
	MyPrintf("***   5.view information(self      ***");
	MyPrintf("***   0.exit to login menu         ***");
	MyPrintfStar();
	while(1)
	{
		MyPrintf("Please input the num:");
		scanf("%s",name);
		rel = CheckOperation(ChangeStringToInt(name),0,5);
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
//when student login, choose his operation
int ChooseOperationStudent()
{
	//int num;
	int rel = 0;
	char name[20];            
	MyPrintf("     StudentOperation                 ");
	MyPrintfStar();
	MyPrintf("***  1.view information(self)      ***");
	MyPrintf("***  2.modify password(self)       ***");
	MyPrintf("***  0.exit to login menu          ***");
	MyPrintfStar();
	while(1)
	{
		MyPrintf("Please input the num:");
		scanf("%s",name);
		rel = CheckOperation(ChangeStringToInt(name),0,2);
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
// whoever you choose to login,choose login way:name or num 
int ChooseLoginWay()
{
	//int num;
	int rel = 0;
	char name[20];
	MyPrintf("            ChooseLoginWay            ");
	MyPrintfStar();
	MyPrintf("******      1.login by name    *******");
	MyPrintf("******      2.login by num     *******");
	MyPrintfStar();
	while(1)
	{
		MyPrintf("Please input the num:");
		scanf("%s",name);
		rel = CheckOperation(ChangeStringToInt(name),1,2);
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
//similar to the latter one
int ChooseOperationWay()
{
	//int num;
	int rel = 0;
	char name[20];
	//	int scanf_return_value= 0;
	MyPrintf("          ChooseOperationWay          ");
	MyPrintfStar();
	MyPrintf("***      1.operation by name       ***");
	MyPrintf("***      2.operation by num        ***");
	MyPrintfStar();
	while(1)
	{
		MyPrintf("Please input the num:");
		scanf("%s",name);
		rel = CheckOperation(ChangeStringToInt(name),1,2);
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
//super user choose modify item of the teacher
int SuperUserModifyTeacherInfo()
{
	//int num;
	int rel = 0;
	char name[20];
	MyPrintf("           ChooseModifyItem          ");
	MyPrintfStar();
	MyPrintf("******    1.modify subject     ******");
	MyPrintf("******    2.modify class       ******");
	MyPrintf("******    3.above all          ******");
	MyPrintf("******    0.exit to admin menu ******");
	MyPrintfStar();
	while(1)
	{
		MyPrintf("Please input the num:");
		scanf("%s",name);
		rel = CheckOperation(ChangeStringToInt(name),0,3);
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
////super user choose modify item of the student
int SuperUserModifyStudentGradeRecord()
{
	//int num;
	int rel = 0;
	char name[20];
	MyPrintf("           ChooseModifyItem           ");
	MyPrintfStar();
	MyPrintf("******     1.Chinese score      ******");
	MyPrintf("******     2.math score         ******");
	MyPrintf("******     3.English score      ******");
	MyPrintf("******     4.C score            ******");
	MyPrintf("******     5.above all          ******");
	MyPrintf("******     0.exit to admin menu ******");
	MyPrintfStar();
	while(1)
	{
		MyPrintf("Please input the num:");
		scanf("%s",name);
		rel = CheckOperation(ChangeStringToInt(name),0,5);
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
// view students according to subject
int SuperUserViewStudentGradeRecordBySubject()
{
	//int num;
	int rel = 0;
	char name[20];
	MyPrintf("           ChooseModifyItem           ");
	MyPrintfStar();
	MyPrintf("******     1.Chinese score      ******");
	MyPrintf("******     2.math score         ******");
	MyPrintf("******     3.English score      ******");
	MyPrintf("******     4.C score            ******");
	MyPrintf("******     0.exit to admin menu ******");
	MyPrintfStar();
	while(1)
	{
		MyPrintf("Please input the num:");
		scanf("%s",name);
		rel = CheckOperation(ChangeStringToInt(name),0,4);
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
