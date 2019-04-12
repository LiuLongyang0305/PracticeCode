#ifndef _STUDENT_H_
#define _STUDENT_H_

/***************************/
typedef struct student_
{
	int school_number;
	char name[20];
	char sex[6];
	int age;
	float Chinese_score;
	float math_score;
	float English_score;
	float clanguage_score;
	float sum_score;
	int rank;
	char stu_class[5];
}STUDENT;

typedef struct studentpassword
{
	int school_number;
	char name[20];
	char password[20];
}STUDENT_PASSWORD;
/*******************************/
extern STUDENT_PASSWORD student_password[100];
extern STUDENT student[100];

extern void(*select_sort_by_subject[4])();
void (*student_function[2])();

extern STUDENT  system_current_student_user;
// student succeed to login,save the student's info
/************************************/
void SelectSortByChinese();
void SelectSortByMath();
void SelectSortByEnglish();
void SelectSortByClanguage();
void SelectSort();
void AddStudent();
void DeleteStudent();
void ModifyStudentGradeRecordBySuperUser();
void ViewAllStudentInfo();
void ViewSingleStudentInfo();
void ViewStudentGradeRecordBySubject();
void ViewStudentGradeRecordByClass();
void ModifyStudentPasswordBySuperUser();
void WriteStudentPassword();
void WriteStudent();
int FindStudentByNum( int num );
int FindStudentByName( char name[]);
void ViewStudentGradeRecordByStudent();
void ModifyStudentPasswordByStudent();

/*************************************/
#endif
