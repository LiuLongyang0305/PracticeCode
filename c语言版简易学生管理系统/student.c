#include "student.h"
#include "menu.h"
#include <stdio.h>
#include "super_user.h"
#include "system_function.h"
#include <string.h>
#include "terminal_set.h"
STUDENT student[100];
STUDENT_PASSWORD student_password[100];
STUDENT  system_current_student_user;

void (*student_function[2])() = {ViewStudentGradeRecordByStudent,ModifyStudentPasswordByStudent};
void(*select_sort_by_subject[4])() = {SelectSortByChinese,SelectSortByMath,SelectSortByEnglish,SelectSortByClanguage};

//function: write the info of student into the student_grade.txt

void WriteStudent()
{
	int i;
	FILE *wfp_student_grade = fopen("student_grade.txt","w");
	while(wfp_student_grade == NULL)
	{
		wfp_student_grade = fopen("teacher_info.txt","w");
	}
	fputs(grade_record_start_str,wfp_student_grade);
	fputc('\n',wfp_student_grade);
	for(i = 0;i < super_user.student_num; i ++)
	{
		fprintf(wfp_student_grade,"%d\t%s\t%s\t%3d\t%4.1f\t%4.1f\t%4.1f\t%4.1f\t%4.1f\t%d\t%s\n",student[i].school_number,student[i].name,student[i].sex,student[i].age,student[i].Chinese_score,student[i].math_score,student[i].English_score,student[i].clanguage_score,student[i].sum_score,student[i].rank,student[i].stu_class);
	}
	fclose(wfp_student_grade);
}
// write the info of student to login into student_password.txt
void WriteStudentPassword()
{
	int i;
	FILE *wfp_student_password = fopen("student_password.txt","w");
	while(wfp_student_password == NULL)
	{
		wfp_student_password = fopen("teacher_password.txt","w");
	}
	fputs(student_password_start_str,wfp_student_password);
	fputc('\n',wfp_student_password);
	for(i = 0; i< super_user.student_num;i ++)
	{	
		fprintf(wfp_student_password,"%s\t%d\t%s\n",student_password[i].name,student_password[i].school_number,student_password[i].password);
	}
	fclose(wfp_student_password);
	MyPrintf("Succeed to store all student's password!");
}
//select sort according to sum_score
void SelectSort()
{
	int i;
	int j;
	float max;
	int max_index; 

	STUDENT tmp;
	STUDENT_PASSWORD temp;
	for(j = 0;j < super_user.student_num -1;j ++)
	{
		max = student[j].sum_score;
		max_index = j; 
		for( i = j+1; i <super_user.student_num;i ++)
		{
			if( student[i].sum_score > max)
			{
				max = student[i].sum_score;
				max_index = i;
			}
		}

		temp = student_password[j];
		student_password[j] = student_password[max_index];
		student_password[max_index] = temp;
		//to ensure the one to one correspondence between student[] and student_password[]	
		tmp = student[j];
		student[j] = student[max_index];
		student[max_index] = tmp;		
	}
	for(i = 0;i < super_user.student_num;i ++)
	{
		student[i].rank = i + 1;//
	}

}
//select sort according to Chinese score
void SelectSortByChinese()
{
	int i;
	int j;
	float max;
	int max_index; 

	STUDENT tmp;
	STUDENT_PASSWORD temp;
	for(j = 0;j < super_user.student_num -1;j ++)
	{
		max = student[j].Chinese_score;
		max_index = j; 
		for( i = j+1; i <super_user.student_num;i ++)
		{
			if( student[i].Chinese_score > max)
			{
				max = student[i].Chinese_score;
				max_index = i;
			}
		}
		tmp = student[j];
		student[j] = student[max_index];
		student[max_index] = tmp;

		temp = student_password[j];
		student_password[j] = student_password[max_index];
		student_password[max_index] = temp;
	}
	for(i = 0;i < super_user.student_num;i ++)
	{
		student[i].rank = i + 1;
	}	
}
//select sort according to math score
void SelectSortByMath()
{
	int i;
	int j;
	float max;
	int max_index; 

	STUDENT tmp;
	STUDENT_PASSWORD temp;
	for(j = 0;j < super_user.student_num -1;j ++)
	{
		max = student[j].math_score;
		max_index = j; 
		for( i = j+1; i <super_user.student_num;i ++)
		{
			if( student[i].math_score > max)
			{
				max = student[i].math_score;
				max_index = i;
			}
		}
		tmp = student[j];
		student[j] = student[max_index];
		student[max_index] = tmp;

		temp = student_password[j];
		student_password[j] = student_password[max_index];
		student_password[max_index] = temp;
	}
	for(i = 0;i < super_user.student_num;i ++)
	{
		student[i].rank = i + 1;
	}	
}
//select sort according to English score
void SelectSortByEnglish()
{
	int i;
	int j;
	float max;
	int max_index; 

	STUDENT tmp;
	STUDENT_PASSWORD temp;
	for(j = 0;j < super_user.student_num -1;j ++)
	{
		max = student[j].English_score;
		max_index = j; 
		for( i = j+1; i <super_user.student_num;i ++)
		{
			if( student[i].English_score> max)
			{
				max = student[i].English_score;
				max_index = i;
			}
		}
		tmp = student[j];
		student[j] = student[max_index];
		student[max_index] = tmp;		
	}

	temp = student_password[j];
	student_password[j] = student_password[max_index];
	student_password[max_index] = temp;
	for(i = 0;i < super_user.student_num;i ++)
	{
		student[i].rank = i + 1;
	}	
}
//select sort according to c language score
void SelectSortByClanguage()
{
	int i;
	int j;
	float max;
	int max_index; 

	STUDENT tmp;
	STUDENT_PASSWORD temp;
	for(j = 0;j < super_user.student_num -1;j ++)
	{
		max = student[j].clanguage_score;
		max_index = j; 
		for( i = j+1; i <super_user.student_num;i ++)
		{
			if( student[i].clanguage_score > max)
			{
				max = student[i].clanguage_score;
				max_index = i;
			}
		}
		tmp = student[j];
		student[j] = student[max_index];
		student[max_index] = tmp;	

		temp = student_password[j];
		student_password[j] = student_password[max_index];
		student_password[max_index] = temp;
	}
	for(i = 0;i < super_user.student_num;i ++)
	{
		student[i].rank = i + 1;
	}	
}
//find the student's position by his name
int FindStudentByName( char name[])
{
	int i = 0;
	for(i = 0;i < super_user.student_num;i ++)
	{
		if(strcmp(name,student[i].name) == 0)
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
//find the student's position by his school number
int FindStudentByNum( int num )
{
	int i = 0;
	for(i = 0;i < super_user.student_num;i ++)
	{
		if((num - student[i].school_number) == 0)
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
// add one student to the system
void AddStudent()
{
	int duplication_check_result = 0;
	MyPrintf("Please input student's school_number:");
	scanf("%d",&student[super_user.student_num].school_number);
	duplication_check_result = FindStudentByNum(student[super_user.student_num].school_number);
	while(duplication_check_result < super_user.student_num)
	{
		MyPrintf("The school number you input already exists!");
		MyPrintf("Please input student's school_number:");
		scanf("%d",&student[super_user.student_num].school_number);
		duplication_check_result = FindStudentByNum(student[super_user.student_num].school_number);
	}
	//to ensure the school number is not exist

	MyPrintf("Please input student's name:");
	scanf("%s",student[super_user.student_num].name);
	duplication_check_result = FindStudentByName(student[super_user.student_num].name);
	while(duplication_check_result < super_user.student_num)
	{
		MyPrintf("The name you input already exists!");
		MyPrintf("Please input student's name:");
		scanf("%s",student[super_user.student_num].name);		
		duplication_check_result = FindStudentByName(student[super_user.student_num].name);
	}
	//to ensure the name doesn't exist


	MyPrintf("Please input student's sex:");
	scanf("%s",student[super_user.student_num].sex);


	MyPrintf("Please input student's age:");
	scanf("%d",&student[super_user.student_num].age);

	MyPrintf("Please input student's Chinese score:");
	scanf("%f",&student[super_user.student_num].Chinese_score);

	MyPrintf("Please input student's math score:");
	scanf("%f",&student[super_user.student_num].math_score);

	MyPrintf("Please input student's English score:");
	scanf("%f",&student[super_user.student_num].English_score);

	MyPrintf("Please input student's C language score:");
	scanf("%f",&student[super_user.student_num].clanguage_score);

	MyPrintf("Please input student's class:");
	scanf("%s",student[super_user.student_num].stu_class);

	student[super_user.student_num].sum_score = student[super_user.student_num].Chinese_score + student[super_user.student_num].math_score + student[super_user.student_num].English_score +student[super_user.student_num].clanguage_score ; 
	student[super_user.student_num].rank = 0;
	// calculate the sum-score and initialize the member rank

	strcpy(student_password[super_user.student_num].name,student[super_user.student_num].name);
	student_password[super_user.student_num].school_number = student[super_user.student_num].school_number;

	MyPrintf("Please input student's password:");
	set_terminal_hide();
	scanf("%s",student_password[super_user.student_num].password);
	set_terminal_show();

	//when adding student,add student password information,too

	super_user.student_num ++;
	//student number changes

	SelectSort();
	WriteStudent();
	WriteStudentPassword();
	WriteSuperUserInfo(); 
	//sort,and write relative information
}
//delete one student's information from system
void DeleteStudent()
{
	int i;
	char name[20];
	int num;
	int  find_student_result = 0;
	int way = ChooseOperationWay();
	//choose the way to find the student you want to delete :name or num
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
	// the student is not find
	else
	{
		for(i = find_student_result;i < super_user.student_num-1;i ++)
		{
			student[i] = student[i + 1];
			student_password[i] = student_password[i + 1];
			// delete student's info and his login info
		}
		MyPrintf("Succeed to delete the student and his/her password information!");
		super_user.student_num --;
		//student number changes
		SelectSort();
		WriteStudent();
		WriteStudentPassword();
		WriteSuperUserInfo();
		// write relative info
	}
}

//super user modify the student's grade record
void ModifyStudentGradeRecordBySuperUser()
{
	char name[20];
	int num;
	int way = ChooseOperationWay();
	int find_student_result = 0;
	int mod_item = 0;

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
		mod_item = SuperUserModifyStudentGradeRecord();
		if(mod_item == 1)
		{
			MyPrintf("You choose to modify the student's Chinese score!");
			MyPrintf("Please input Chinese score:");
			scanf("%f",&student[find_student_result].Chinese_score);
		}
		if(mod_item == 2)
		{
			MyPrintf("You choose to modify the student's math score!");
			MyPrintf("Please input math score:");
			scanf("%f",&student[find_student_result].math_score);
		}
		if(mod_item == 3)
		{
			MyPrintf("You choose to modify the student's English score!");
			MyPrintf("Please input English score:");
			scanf("%f",&student[find_student_result].English_score);
		}
		if(mod_item == 4)
		{
			MyPrintf("You choose to modify the student's C language score!");
			MyPrintf("Please input C language score:");
			scanf("%f",&student[find_student_result].clanguage_score);
		}
		if(mod_item == 5)
		{
			MyPrintf("You choose to modify the student's all subject score!");
			MyPrintf("Please input Chinese score:");
			scanf("%f",&student[find_student_result].Chinese_score);
			MyPrintf("Please input math score:");
			scanf("%f",&student[find_student_result].math_score);
			MyPrintf("Please input English score:");
			scanf("%f",&student[find_student_result].English_score);
			MyPrintf("Please input C language score:");
			scanf("%f",&student[find_student_result].clanguage_score);
		}
		student[find_student_result].sum_score = student[find_student_result].Chinese_score + student[find_student_result].math_score + student[find_student_result].English_score + student[find_student_result].clanguage_score ; 
	}
	SelectSort();
	WriteStudent();
	WriteStudentPassword();
	// grade changes,so sort and write into txt
}

//view all students' info : grade and password 
void ViewAllStudentInfo()
{
	ReadGradeRecord();
	ReadStudentPassword();
}

//view single student's info
void ViewSingleStudentInfo()
{
	int num = 0;
	char name[20];
	int way = ChooseOperationWay();
	int find_student_result = 0;
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
		puts(grade_record_start_str);
		fprintf(stdout,"%d\t%s\t%s\t%3d\t%4.1f\t%4.1f\t%4.1f\t%4.1f\t%4.1f\t%d\t%s\n",student[find_student_result].school_number,student[find_student_result].name,student[find_student_result].sex,student[find_student_result].age,student[find_student_result].Chinese_score,student[find_student_result].math_score,student[find_student_result].English_score,student[find_student_result].clanguage_score,student[find_student_result].sum_score,student[find_student_result].rank,student[find_student_result].stu_class);
		puts(student_password_start_str);
		fprintf(stdout,"%s\t%d\t%s\n",student_password[find_student_result].name,student_password[find_student_result].school_number,student_password[find_student_result].password);
	}
}
//view grade record according to single subject's grade

void ViewStudentGradeRecordBySubject()
{
	int i;
	int sort_way = SuperUserViewStudentGradeRecordBySubject();
	if(sort_way != 0)
	{
		select_sort_by_subject[sort_way - 1]();
		//pointer to sort function 
		puts(grade_record_start_str);
		for(i = 0; i < super_user.student_num; i ++)
		{
			fprintf(stdout,"%d\t%s\t%s\t%3d\t%4.1f\t%4.1f\t%4.1f\t%4.1f\t%4.1f\t%d\t%s\n",student[i].school_number,student[i].name,student[i].sex,student[i].age,student[i].Chinese_score,student[i].math_score,student[i].English_score,student[i].clanguage_score,student[i].sum_score,student[i].rank,student[i].stu_class);
		}
		//show the result to stdout
		SelectSort();
	}
}

// View student grade record according to class
void ViewStudentGradeRecordByClass()
{
	char student_class[5];
	int i;
	MyPrintf("Please input the class you want to view:");
	scanf("%s",student_class);

	puts(grade_record_start_str);
	for(i = 0;i < super_user.student_num;i ++)
	{
		if(strcmp(student_class,student[i].stu_class) == 0)
		{
			fprintf(stdout,"%d\t%s\t%s\t%3d\t%4.1f\t%4.1f\t%4.1f\t%4.1f\t%4.1f\t%d\t%s\n",student[i].school_number,student[i].name,student[i].sex,student[i].age,student[i].Chinese_score,student[i].math_score,student[i].English_score,student[i].clanguage_score,student[i].sum_score,student[i].rank,student[i].stu_class);
		}
		//check the student's class
	}

}
// super user change the student's password to :666666;
//when you forget the password,use this function
void ModifyStudentPasswordBySuperUser()
{
	char name[20];
	int num ;
	int find_student_result = 0;
	char verify_ch = '\0';
	int way = ChooseOperationWay();

	if(way == 1)
	{
		MyPrintf("Please input the student's name:");
		scanf("%s",name);
		find_student_result = FindStudentByName(name);
	} 
	if(way == 2)
	{
		MyPrintf("Please input the student's num:");
		scanf("%d",&num);
		find_student_result = FindStudentByNum(num);
	}
	if(find_student_result == super_user.student_num)
	{
		MyPrintf("Failed to find the student!");
		MyPrintf("Failed to modify the student's password!");
	}
	else
	{
		MyPrintf("Please select: Y or y for modifying the teacher's password , N or n for not modifying the teacher's password!");
		getchar();
		scanf("%c",&verify_ch);
		//void to modify the password easily
		if(verify_ch == 'Y' || verify_ch == 'y')
		{
			strcpy(student_password[find_student_result].password,"666666");
			MyPrintf("Succeed to modify the student's password violently!");
			WriteStudentPassword();
		}
		if(verify_ch == 'N' || verify_ch == 'n')
		{
			MyPrintf(" You quit to modify the student's password!");
		}
	}
}
// view info by student self

void  ViewStudentGradeRecordByStudent()
{
	int find_student_result = 0;
	/*	
		system_current_student_user = student[0];
	 */
	//when student login ,this variable will be initialized
	//only for debug here
	find_student_result = FindStudentByName(system_current_student_user.name);
	//when login successfully,the student surely exists
	puts(grade_record_start_str);
	fprintf(stdout,"%d\t%s\t%s\t%3d\t%4.1f\t%4.1f\t%4.1f\t%4.1f\t%4.1f\t%d\t%s\n",student[find_student_result].school_number,student[find_student_result].name,student[find_student_result].sex,student[find_student_result].age,student[find_student_result].Chinese_score,student[find_student_result].math_score,student[find_student_result].English_score,student[find_student_result].clanguage_score,student[find_student_result].sum_score,student[find_student_result].rank,student[find_student_result].stu_class);
}
// modify the password of the student who login by his or her self
void ModifyStudentPasswordByStudent()
{
	char old_password[20];
	char new_password[20];
	char verify_password[20];
	int error_count = 3;
	int find_student_result = 0;
	char verify_ch = '\0';

	/*
	   system_current_student_user = student[3];
	 */
	//when student login ,this variable will be initialized
	//only for debug here

	find_student_result = FindStudentByNum(system_current_student_user.school_number);
	MyPrintf("Please input your old password:");
	set_terminal_hide();
	scanf("%s",old_password);
	set_terminal_show();

	while((--error_count) && (strcmp(old_password,student_password[find_student_result].password) != 0))
	{
		MyPrintf("You input the wrong password!");
		printf("You can try %d times!\n",error_count);
		MyPrintf("Please input your old password:");
		set_terminal_hide();
		scanf("%s",old_password);
		set_terminal_show();
	}
	//you can try three times at most
	if(strcmp(old_password,student_password[find_student_result].password) != 0)
	{
		MyPrintf("Failed to modify your password because the old password you input is wrong !");
	}
	else
	{
		MyPrintf("Please input your new password:");
		set_terminal_hide();
		scanf("%s",new_password);
		set_terminal_show();

		// input new password
		MyPrintf("Please input your new password one more time:");
		set_terminal_hide();
		scanf("%s",verify_password);
		set_terminal_show();
		//verify the new password
		if(strcmp(new_password,verify_password) != 0)
		{
			MyPrintf("You input the wrong password when you verified!");
			MyPrintf("Failed to modify your password!");
			//when input different numbers,do this
		}
		else
		{
			MyPrintf("Please select: Y or y for modifying your password, N or n for not modifying your password!");
			getchar();
			scanf("%c",&verify_ch);
			if(verify_ch == 'Y' || verify_ch == 'y')
			{
				strcpy(student_password[find_student_result].password,new_password);
				MyPrintf("Succeed to modify your password!");
				WriteStudentPassword();
				// sure to modify the password,write into txt
			}
			if(verify_ch == 'N' || verify_ch == 'n')
			{
				MyPrintf(" You quit to modify your password!");
			}
		}
	}
}
