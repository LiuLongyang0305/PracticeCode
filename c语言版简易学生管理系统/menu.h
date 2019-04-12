#ifndef _MENU_H_
#define _MENU_H_
extern int (*menu_function[9])();
/***************************/
void MyPrintf(char str[]);
int ChooseMemberLogin();
int ChooseOperationSuperUser();
int ChooseOperationTeacher();
int ChooseLoginWay();
int ChooseOperationStudent();
int ChooseOperationWay();
int SuperUserModifyTeacherInfo();
int SuperUserModifyStudentGradeRecord();
int SuperUserViewStudentGradeRecordBySubject();
/*******************************/
#endif
