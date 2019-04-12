#ifndef _TECHER_H_
#define _TECHER_H_

#include "student.h"

/*******************************/
typedef struct teacher_ 
{
	char name[20];
	int  num;
	char subject[20];
	char tech_class[10];
}TEACHER;

typedef struct teacherpassword
{
	int num;
	char name[20];
	char password[20];
}TEACHER_PASSWORD;

extern TEACHER teacher[20];
extern TEACHER_PASSWORD teacher_password[20];

extern TEACHER  system_current_teacher_user;
extern void (*teacher_function[5])();
//when teacher succeed to login ,save it
/**********************************/

void AddTeacher();
void DeleteTeacher();
void ModifyTecherInfoBySuperUser();
void ViewAllTeacherInfo();
void ViewSingleTeacherInfo();
void ModifyTeacherPasswordBySuperUser();
void ViewStudentGradeRecordByTeacher();
void ViewSingleStudentRecordByTeacher();
void ModifyTeacherPasswordByTeacher();
int  CheckTeacherModifyStudentGradeRecordQualification(TEACHER tea,STUDENT stu);
int  CalculateOffsetByTeacherSubject(TEACHER temp);
void ModifyStudentGradeRecordByTeacher();
void ViewTeacherInfoByTeacher();

/****************************************/
#endif
