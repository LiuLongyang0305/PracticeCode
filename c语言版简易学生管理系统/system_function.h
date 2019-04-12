#ifndef _SYS_FUNCTION_H_
#define _SYS_FUNCTION_H_
/******************************/
extern char grade_record_start_str[100];
extern char teacher_info_start_str[50];
extern char student_password_start_str[50];
extern char teacher_password_start_str[50];
extern char super_user_start_str[50];
extern char start_str[4][100];
extern char subject[4][15];
/*****************************/
void ReadGradeRecord();
void ReadTeacherInfo();
void ReadStudentPassword();
void ReadTeacherPassword();
void ReadSuperUserInfo();
void SystemInitiate();
/*****************************/
#endif
