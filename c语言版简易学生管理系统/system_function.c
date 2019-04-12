#include <stdio.h>
#include "student.h"
#include "system_function.h"
#include <string.h>
#include "teacher.h"
#include "super_user.h"
/******************************************/
char grade_record_start_str[100] = "number\tname\tsex\tage\tChinese\tmath\tEnglish\tC_lang\tsum\trank\tclass";
char teacher_info_start_str[50] = "number\tname\tsubject\tclass";
char student_password_start_str[50] = "name\tnumber\tpassword";
char teacher_password_start_str[50] = "name\tnumber\tpassword";
char super_user_start_str[50] = "teacher\tstudent\tname\t\tpassword";

char subject[4][15] = {"Chinese","math","English","C_lang"};
char start_str[4][100]={"number\tname\tsex\tage\tChinese\trank\tcalss","number\tname\tsex\tage\tmath\trank\tcalss","number\tname\tsex\tage\tEnglish\trank\tcalss","number\tname\tsex\tage\tC_lang\trank\tcalss"};

// system initialize
void SystemInitiate()
{
	ReadGradeRecord();
	ReadTeacherInfo();
	ReadStudentPassword();
	ReadTeacherPassword();
	ReadSuperUserInfo();
}
//read info of student grade
void ReadGradeRecord()
{
	char student_start_str[100];
	FILE *rfp_grade_record = NULL;
	int i = 0;
	int num = 0;
	rfp_grade_record = fopen("student_grade.txt","r");
	while(rfp_grade_record == NULL)
	{
		rfp_grade_record = fopen("student_grade.txt","r");	
	}
	fgets(student_start_str,strlen(grade_record_start_str)+1,rfp_grade_record);
	while(fscanf(rfp_grade_record,"%d%s%s%d%f%f%f%f%f%d%s",&student[i].school_number,student[i].name,student[i].sex,&student[i].age,&student[i].Chinese_score,&student[i].math_score,&student[i].English_score,&student[i].clanguage_score,&student[i].sum_score,&student[i].rank,student[i].stu_class) != EOF)
	{
		i++;
	}
	fclose(rfp_grade_record);
	num = i;
	puts(grade_record_start_str);
	for(i = 0; i < num; i ++)
	{
		fprintf(stdout,"%d\t%s\t%s\t%3d\t%4.1f\t%4.1f\t%4.1f\t%4.1f\t%4.1f\t%d\t%s\n",student[i].school_number,student[i].name,student[i].sex,student[i].age,student[i].Chinese_score,student[i].math_score,student[i].English_score,student[i].clanguage_score,student[i].sum_score,student[i].rank,student[i].stu_class);
	}
}
//read info of teacher
void ReadTeacherInfo()
{
	FILE *rfp_teacher_info = NULL;
	int i = 0;
	int num = 0;  
	char teacher_info_str[50];
	rfp_teacher_info = fopen("teacher_info.txt","r");
	while(rfp_teacher_info == NULL)
	{

		rfp_teacher_info = fopen("teacher_info.txt","r");
	}

	fgets(teacher_info_str,strlen(teacher_info_start_str)+1,rfp_teacher_info);
	while(fscanf(rfp_teacher_info,"%s%d%s%s",teacher[i].name,&teacher[i].num,teacher[i].subject,teacher[i].tech_class)!=EOF)
	{
		i++;
	}
	fclose(rfp_teacher_info);

	num = i;	
	puts(teacher_info_str);
	for(i = 0; i < num; i ++)
	{
		fprintf(stdout,"%d\t%s\t%s\t%s\n",teacher[i].num,teacher[i].name,teacher[i].subject,teacher[i].tech_class);
	}
}
//read info of student password
void ReadStudentPassword()
{
	FILE *rfp_student_password = NULL;
	int i = 0;
	int num = 0;  
	char student_password_str[50];
	rfp_student_password = fopen("student_password.txt","r");
	while(rfp_student_password == NULL)
	{
		rfp_student_password = fopen("student_password.txt","r");	
	}

	fgets(student_password_str,strlen(student_password_start_str)+1,rfp_student_password);
	while(fscanf(rfp_student_password,"%s%d%s",student_password[i].name,&student_password[i].school_number,student_password[i].password) != EOF)
	{
		i ++;
	}
	fclose(rfp_student_password);
	num = i;
	puts(student_password_str);
	for(i = 0;i < num; i ++)
	{
		fprintf(stdout,"%s\t%d\t%s\n",student_password[i].name,student_password[i].school_number,student_password[i].password);
	}
}
//read info of teacher password
void ReadTeacherPassword()
{
	FILE *rfp_teacher_password = NULL;
	int i = 0;
	int num = 0;  
	char teacher_password_str[50];
	rfp_teacher_password = fopen("teacher_password.txt","r");
	while(rfp_teacher_password == NULL)
	{
		rfp_teacher_password = fopen("teacher_password.txt","r");

	}

	fgets(teacher_password_str,strlen(teacher_password_start_str)+1,rfp_teacher_password);
	while(fscanf(rfp_teacher_password,"%s%d%s",teacher_password[i].name,&teacher_password[i].num,teacher_password[i].password) != EOF)
	{
		i ++;
	}
	fclose(rfp_teacher_password);
	num = i;
	puts(teacher_password_str);
	for(i = 0;i < num; i ++)
	{
		fprintf(stdout,"%s\t%d\t%s\n",teacher_password[i].name,teacher_password[i].num,teacher_password[i].password);
	}
}
//read information of super user
void ReadSuperUserInfo()
{
	char super_user_str[50];
	FILE *rfp_super_user_info = NULL;

	rfp_super_user_info = fopen("super_user_info.txt","r");
	while(rfp_super_user_info == NULL)
	{
		rfp_super_user_info = fopen("super_user_info.txt","w");
	}

	fgets(super_user_str,strlen(super_user_start_str)+1,rfp_super_user_info);
	fscanf(rfp_super_user_info,"%d%d%s%s",&super_user.teacher_num,&super_user.student_num,super_user.name,super_user.password);
	fclose(rfp_super_user_info);

	fputs(super_user_str,stdout);
	putchar('\n');
	fprintf(stdout,"%d\t%d\t%s\t%s\n",super_user.teacher_num,super_user.student_num,super_user.name,super_user.password);
}
