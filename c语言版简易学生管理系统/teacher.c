#include "teacher.h"
#include "menu.h"
#include <stdio.h>
#include "super_user.h"
#include "system_function.h"
#include <string.h>
#include "student.h"
#include "terminal_set.h"
TEACHER teacher[20];
TEACHER_PASSWORD teacher_password[20];
TEACHER  system_current_teacher_user;

void (*teacher_function[5])() = {ViewStudentGradeRecordByTeacher,ViewSingleStudentRecordByTeacher,ModifyStudentGradeRecordByTeacher,
	ModifyTeacherPasswordByTeacher,ViewTeacherInfoByTeacher};
//save info when info changes
static void WriteTeacher()
{
	int i;
	FILE *wfp_teacher_info = fopen("teacher_info.txt","w");
	while(wfp_teacher_info == NULL)
	{
		wfp_teacher_info = fopen("teacher_info.txt","w");
	}
	fputs(teacher_info_start_str,wfp_teacher_info);
	fputc('\n',wfp_teacher_info);
	for(i = 0;i < super_user.teacher_num; i ++)
	{
		fprintf(wfp_teacher_info,"%s\t%d\t%s\t%s\n",teacher[i].name,teacher[i].num,teacher[i].subject,teacher[i].tech_class);
	}
	fclose(wfp_teacher_info);
}
//save info when info changes
static void WriteTeacherPassword()
{
	int i;
	FILE *wfp_teacher_password = fopen("teacher_password.txt","w");
	while(wfp_teacher_password == NULL)
	{
		wfp_teacher_password = fopen("teacher_password.txt","w");
	}
	fputs(teacher_password_start_str,wfp_teacher_password);
	fputc('\n',wfp_teacher_password);
	for(i = 0; i< super_user.teacher_num;i ++)
	{	
		fprintf(wfp_teacher_password,"%s\t%d\t%s\n",teacher_password[i].name,teacher_password[i].num,teacher_password[i].password);
	}
	fclose(wfp_teacher_password);
}
//find the teacher by name
static int FindTeacherByName( char name[])
{
	int i = 0;
	for(i = 0;i < super_user.teacher_num;i ++)
	{
		if(strcmp(name,teacher[i].name) == 0)
		{
			break;
		}
		else
		{
			continue;
		}
	}
	return i;
} 
//find the teacher by num
static int FindTeacherByNum( int num)
{
	int i = 0;
	for(i = 0;i < super_user.teacher_num;i ++)
	{
		if((num - teacher[i].num) == 0)
		{
			break;
		}
		else
		{
			continue;
		}
	}
	return i;
}

// super user add teacher
void AddTeacher()
{
	int duplication_check_result = 0;
	MyPrintf("Please input the teacher's name:");
	scanf("%s",teacher[super_user.teacher_num].name);
	duplication_check_result = FindTeacherByName(teacher[super_user.teacher_num].name);
	while(duplication_check_result < super_user.teacher_num)
	{
		MyPrintf("The name you input already exists!");
		MyPrintf("Please input the teacher's name:");
		scanf("%s",teacher[super_user.teacher_num].name);
		duplication_check_result = FindTeacherByName(teacher[super_user.teacher_num].name);
	}
	// ensure the name doesn't exist
	MyPrintf("Please input the teacher's num:");
	scanf("%d",&teacher[super_user.teacher_num].num);
	duplication_check_result = FindTeacherByNum(teacher[super_user.teacher_num].num);
	while(duplication_check_result < super_user.teacher_num)
	{
		MyPrintf("The num you input already exists!");
		MyPrintf("Please input the teacher's num:");
		scanf("%d",&teacher[super_user.teacher_num].num);
		duplication_check_result = FindTeacherByNum(teacher[super_user.teacher_num].num);
	}
	// ensure the num doesn't exist
	MyPrintf("Please input the teacher's subject:");
	scanf("%s",teacher[super_user.teacher_num].subject);

	MyPrintf("Please input the teacher's class:");
	scanf("%s",teacher[super_user.teacher_num].tech_class);

	teacher_password[super_user.teacher_num].num  = teacher[super_user.teacher_num].num;
	strcpy(teacher_password[super_user.teacher_num].name,teacher[super_user.teacher_num].name);
	MyPrintf("Please input the teacher's password:");
	set_terminal_hide();
	scanf("%s",teacher_password[super_user.teacher_num].password);
	set_terminal_show();

	//add teacher password when adding teacher
	MyPrintf("Succeed to add a teacher!");

	super_user.teacher_num ++;
	//number of teacher changes
	WriteTeacher();
	WriteTeacherPassword();
	WriteSuperUserInfo(); 
}
//delete teacher by name or num
void DeleteTeacher()
{

	int i;
	char name[20];
	int num;
	int  find_teacher_result = 0;
	int way = ChooseOperationWay();
	if( way == 1)
	{
		MyPrintf("Please input the teacher's name :");
		scanf("%s",name);
		find_teacher_result = FindTeacherByName(name);
	}
	if(way == 2)
	{
		MyPrintf("Please input the teacher's num :");
		scanf("%d",&num);
		find_teacher_result = FindTeacherByNum(num);
	}

	if( find_teacher_result == super_user.teacher_num)
	{
		MyPrintf("Failed to find the teacher!");
		MyPrintf("Failed to delete the teacher!");
	}
	else
	{
		for(i = find_teacher_result;i < super_user.teacher_num-1;i ++)
		{
			teacher[i] = teacher[i + 1];
			teacher_password[i] = teacher_password[i + 1];
			//delete the password when deleting the teacher
		}
		MyPrintf("Succeed to delete the teacher and his/her password information!");

		super_user.teacher_num --;

		WriteTeacher();
		WriteTeacherPassword();
		WriteSuperUserInfo();
	}
}
// super user modify the teacher info
void ModifyTecherInfoBySuperUser()
{
	int num;
	char name[20];
	int mod_item = 0;
	char subject[20];
	char tech_class[10];
	int operation_way = ChooseOperationWay();
	int find_result = 0;
	if(operation_way == 1)
	{
		MyPrintf("Please input the teacher's name:");
		scanf("%s",name);
		find_result = FindTeacherByName(name);
	}
	else
	{
		MyPrintf("Please input the teacher's num:");
		scanf("%d",&num);
		find_result = FindTeacherByNum(num);
	}
	if(find_result == super_user.teacher_num)
	{
		MyPrintf("Failed to find the teacher!");
		MyPrintf("Failed to modify the teacher!");
	}
	else
	{
		mod_item = SuperUserModifyTeacherInfo();
		if(mod_item == 1)
		{
			MyPrintf("You choose to modify the teacher's subject!");
			MyPrintf("please to input the modified subject:");
			scanf("%s",teacher[find_result].subject);
		}
		if(mod_item == 2)
		{
			MyPrintf("You choose to modify the teacher's class!");
			MyPrintf("please to input the modified class:");
			scanf("%s",teacher[find_result].tech_class);
		}
		if(mod_item == 3)
		{
			MyPrintf("You choose to modify the teacher's subject and class!");
			MyPrintf("please to input the modified subject:");
			scanf("%s",teacher[find_result].subject);
			getchar();
			MyPrintf("please to input the modified class:");
			scanf("%s",teacher[find_result].tech_class);
		}
		MyPrintf("Succeed to modify the teacher's information!");
		WriteTeacher();
	}
}
//view all teacher info
void ViewAllTeacherInfo()
{
	ReadTeacherInfo();
	ReadTeacherPassword();
}
//view single teacher info
void ViewSingleTeacherInfo()
{
	char name[20];
	int num ;
	int way = ChooseOperationWay();
	int find_teacher_result = 0;

	if(way == 1)
	{
		MyPrintf("Please input the teacher's name:");
		scanf("%s",name);
		find_teacher_result = FindTeacherByName(name);
	} 
	if(way == 2)
	{
		MyPrintf("Please input the teacher's num:");
		scanf("%d",&num);
		find_teacher_result = FindTeacherByNum(num);
	}
	if(find_teacher_result == super_user.teacher_num)
	{
		MyPrintf("Failed to find the teacher!");
		MyPrintf("Failed to view the teacher!");
	}
	else
	{
		puts(teacher_info_start_str);
		fprintf(stdout,"%d\t%s\t%s\t%s\n",teacher[find_teacher_result].num,teacher[find_teacher_result].name,teacher[find_teacher_result].subject,teacher[find_teacher_result].tech_class);
		puts(teacher_password_start_str);
		fprintf(stdout,"%s\t%d\t%s\n",teacher_password[find_teacher_result].name,teacher_password[find_teacher_result].num,teacher_password[find_teacher_result].password);
	}
}
//modify the teacher password violently :666666
void ModifyTeacherPasswordBySuperUser()
{
	char name[20];
	int num ;
	int find_teacher_result = 0;
	char verify_ch = '\0';
	int way = ChooseOperationWay();

	if(way == 1)
	{
		MyPrintf("Please input the teacher's name:");
		scanf("%s",name);
		find_teacher_result = FindTeacherByName(name);
	} 
	if(way == 2)
	{
		MyPrintf("Please input the teacher's num:");
		scanf("%d",&num);
		find_teacher_result = FindTeacherByNum(num);
	}
	if(find_teacher_result == super_user.teacher_num)
	{
		MyPrintf("Failed to find the teacher!");
		MyPrintf("Failed to modify the teacher's password!");
	}
	else
	{
		MyPrintf("Please select: Y or y for modifying the teacher's password , N or n for not modifying the teacher's password!");
		getchar();
		scanf("%c",&verify_ch);
		if(verify_ch == 'Y' || verify_ch == 'y')
		{
			strcpy(teacher_password[find_teacher_result].password,"666666");
			MyPrintf("Succeed to modify the teacher's password violently!");
			WriteTeacherPassword();
		}
		if(verify_ch == 'N' || verify_ch == 'n')
		{
			MyPrintf(" You quit to modify your password!");
		}
	}
}
//pointer initialize to student[i].Chinese
int  CalculateOffsetByTeacherSubject(TEACHER temp)
{
	int offset = 0;
	if(strcmp("Chinese",temp.subject) == 0)
	{
		offset = 0;
	}
	if(strcmp("math",temp.subject) == 0)
	{
		offset = 1;
	}
	if(strcmp("English",temp.subject) == 0)
	{
		offset = 2;
	}
	if(strcmp("C_lang",temp.subject) == 0)
	{
		offset = 3;
	}
	return offset;
}

//teacher view student's info
//based on teacher login success
//view the class the teacher taught
//view the grade of the subject the teacher taught
void ViewStudentGradeRecordByTeacher()
{
	int  offset = 0;
	int i;
	float *poiter_score = NULL;
	/*
	   system_current_teacher_user = teacher[1];
	 */
	/****************************************/
	offset = CalculateOffsetByTeacherSubject(system_current_teacher_user);
	select_sort_by_subject[offset]();
	puts(start_str[offset]);
	for(i = 0; i < super_user.student_num;i ++)
	{
		if(strcmp(system_current_teacher_user.tech_class,student[i].stu_class) == 0)
		{
			poiter_score = &(student[i].Chinese_score);
			fprintf(stdout,"%d\t%s\t%s\t%3d\t%4.1f\t%d\t%s\n",student[i].school_number,student[i].name,student[i].sex,student[i].age,*(poiter_score + offset),student[i].rank,student[i].stu_class);
		}
	}
	SelectSort();
}

//view single student's grade
void ViewSingleStudentRecordByTeacher()
{
	int  offset = 0;
	char name[20];
	int num;
	int find_student_result = 0;
	float *poiter_score = NULL;
	int way = 0;

	/*
	   system_current_teacher_user = teacher[1];
	 */
	/**************************************/
	offset = CalculateOffsetByTeacherSubject(system_current_teacher_user);

	select_sort_by_subject[offset]();

	way = ChooseOperationWay();
	if(way == 1)
	{
		MyPrintf("Please input the student's name :");
		scanf("%s",name);
		find_student_result = FindStudentByName(name);
	}
	if( way == 2)
	{
		MyPrintf("Please input the student's num :");
		scanf("%d",&num);
		find_student_result = FindStudentByNum(num);
	}

	if( find_student_result == super_user.student_num)
	{
		MyPrintf("Failed to find the student!");
		MyPrintf("Failed to delete the student!");
	}
	else
	{
		if(strcmp(system_current_teacher_user.tech_class,student[find_student_result].stu_class) == 0)
		{
			poiter_score = &(student[find_student_result].Chinese_score);
			puts(start_str[offset]);
			fprintf(stdout,"%d\t%s\t%s\t%3d\t%4.1f\t%d\t%s\n",student[find_student_result].school_number,student[find_student_result].name,student[find_student_result].sex,student[find_student_result].age,*(poiter_score + offset),student[find_student_result].rank,student[find_student_result].stu_class);
		}
		else
		{
			MyPrintf("Failed to view the student's grade record because you are not his or her teacher!");
		}
	}
	SelectSort();
}

// teacher modify his or her password 
void ModifyTeacherPasswordByTeacher()
{
	char old_password[20];
	char new_password[20];
	char verify_password[20];
	int error_count = 3;
	int find_teacher_result = 0;
	char verify_ch = '\0';
	/*
	   system_current_teacher_user = teacher[1];
	 */
	/******************************************/
	find_teacher_result = FindTeacherByNum(system_current_teacher_user.num);
	MyPrintf("Please input your old password:");
	set_terminal_hide();
	scanf("%s",old_password);
	set_terminal_show();

	while((--error_count) && (strcmp(old_password,teacher_password[find_teacher_result].password) != 0))
	{
		MyPrintf("You input the wrong password!");
		printf("You can try %d times!\n",error_count);
		MyPrintf("Please input your old password:");
		set_terminal_hide();
		scanf("%s",old_password);
		set_terminal_show();

	}
	if(strcmp(old_password,teacher_password[find_teacher_result].password) != 0)
	{
		MyPrintf("Failed to modify your password because the old password you input is wrong!");
	}
	else
	{
		MyPrintf("Please input your new password:");
		set_terminal_hide();
		scanf("%s",new_password);
		set_terminal_show();


		MyPrintf("Please input your new password one more time:");

		set_terminal_hide();
		scanf("%s",verify_password);
		set_terminal_show();

		if(strcmp(new_password,verify_password) != 0)
		{
			MyPrintf("You input the wrong password when you verified!");
			MyPrintf("Failed to modify your password!");
		}
		else
		{
			MyPrintf("Please select: Y or y for modifying your password, N or n for not modifying your password!");
			getchar();
			scanf("%c",&verify_ch);
			if(verify_ch == 'Y' || verify_ch == 'y')
			{
				strcpy(teacher_password[find_teacher_result].password,new_password);
				MyPrintf("Succeed to modify your password!");
				WriteTeacherPassword();
			}
			if(verify_ch == 'N' || verify_ch == 'n')
			{
				MyPrintf(" You quit to modify your password!");
			}
		}
	}
}
//view the info by teacher himself
void ViewTeacherInfoByTeacher()
{
	int find_teacher_result = 0;
	/*
	   system_current_teacher_user = teacher[3];
	 */
	/**************************************/
	find_teacher_result = FindTeacherByNum(system_current_teacher_user.num);

	puts(teacher_info_start_str);
	fprintf(stdout,"%d\t%s\t%s\t%s\n",teacher[find_teacher_result].num,teacher[find_teacher_result].name,teacher[find_teacher_result].subject,teacher[find_teacher_result].tech_class);

}

//check the qualification of the teacher to modify the student's grade
//the  class and subject he teachers
int  CheckTeacherModifyStudentGradeRecordQualification(TEACHER tea,STUDENT stu)
{
	int check_result;
	int mod_item = SuperUserModifyStudentGradeRecord(); 
	if(strcmp(subject[mod_item -1],tea.subject) == 0)
	{
		if(strcmp(tea.tech_class,stu.stu_class) == 0)
		{
			check_result = 1;
		}
		else
		{
			check_result = 0;
		}
	}
	else
	{
		check_result = -1;
	}
	return check_result;
}

// modify the student's grade
void ModifyStudentGradeRecordByTeacher()
{
	char name[20];
	int num;
	int way = ChooseOperationWay();
	int find_student_result = 0;
	int check_result = 0;
	float new_score = 0;
	float verify_score = 0;
	char  verify_ch = '0';
	float *pointer_score = NULL;
	int offset = 0;
	system_current_teacher_user = teacher[0];
	/***********************************/
	if(way == 1)
	{
		MyPrintf("Please input the student's name :");
		scanf("%s",name);
		find_student_result = FindStudentByName(name);
	}
	if( way == 2)
	{
		MyPrintf("Please input the student's num :");
		scanf("%d",&num);
		find_student_result = FindStudentByNum(num);
	}
	if( find_student_result == super_user.student_num)
	{
		MyPrintf("Failed to find the student!");
		MyPrintf("Failed to modify the student's grade record!");
	}
	else
	{
		check_result = CheckTeacherModifyStudentGradeRecordQualification(system_current_teacher_user,student[find_student_result]);
		if(check_result == 0)
		{
			MyPrintf("You can't modify this student's grade because you are not his or her teacher! ");
			MyPrintf("Failed to modify this student's grade!");
		}
		else if(check_result == -1)
		{
			MyPrintf("You can't modify this student's grade because you don't teach this subject ! ");
			MyPrintf("Failed to modify this student's grade!");
		}
		else
		{
			MyPrintf("Please input this student's score:");
			scanf("%f",&new_score);

			MyPrintf("Please input this student's score one more time:");
			scanf("%f",&verify_score);
			if(new_score != verify_score)
			{
				MyPrintf("Failed to modify this student's grade because the scores you input is not equal! ");
			}
			else
			{
				MyPrintf("Please select: Y for modify your password or N or don't modify your password!");
				getchar();
				scanf("%c",&verify_ch);
				if(verify_ch == 'Y')
				{
					pointer_score = &student[find_student_result].Chinese_score;
					offset = CalculateOffsetByTeacherSubject(system_current_teacher_user);
					*(pointer_score + offset ) = new_score;
					student[find_student_result].sum_score = student[find_student_result].Chinese_score + student[find_student_result].math_score + student[find_student_result].English_score + student[find_student_result].clanguage_score ; 
					SelectSort();
					WriteStudent();
					//grade changes,sort and save it
				}
				if(verify_ch == 'N')
				{
					MyPrintf(" You quit to modify the student's grade!");
				}
			}
		}
	}

}
