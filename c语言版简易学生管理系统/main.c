#include <stdio.h>
#include "system_function.h"
#include "super_user.h"
#include "menu.h"
#include "teacher.h"
#include "student.h"
int main()
{
	int login_result = 0;
	int super_user_operation = 0;
	int teacher_operation = 0;
	int student_operation = 0;

	SystemInitiate();

	//CheckLogin();
	//AddTeacher();
	//AddTeacher();
	//WriteSuperUserInfo();
	// DeleteTeacher();
	//DeleteTeacher();
	//ModifyTecherInfoBySuperUser();
	//ViewAllTeacherInfo();
	//ViewSingleTeacherInfo();
	// AddStudent();
	//AddStudent();
	//DeleteStudent();
	//DeleteStudent();
	//ModifyStudentGradeRecordBySuperUser();
	//ViewAllStudentInfo();
	//ViewSingleStudentInfo();
	// ViewSingleStudentInfo();
	//ViewStudentGradeRecordBySubject();
	//ViewAllStudentInfo();
	//ViewStudentGradeRecordByClass();
	//ViewStudentGradeRecordByTeacher();
	// ViewSingleStudentRecordByTeacher();
	//ModifyTeacherPasswordByTeacher();
	//ModifyStudentGradeRecordByTeacher();
	//ViewStudentGradeRecordByStudent();
	//ModifyStudentPasswordByStudent();
	//ViewTeacherInfoByTeacher();
	//ViewMemberNumber();
	// ModifyTeacherPasswordBySuperUser();
	//ModifyStudentPasswordBySuperUser();
	//login_result = CheckLogin();
	while(1)
	{   
		login_result = CheckLogin();
		if(login_result == 4)
		{
			break;
		}
		/*
		   while(login_result == 0)
		   {
		   login_result = CheckLogin();
		   }
		 */
		while(login_result == 1)
		{
			super_user_operation = menu_function[1]();
			if(super_user_operation != 0)
			{
				super_user_function[super_user_operation - 1]();
			}
			else
			{
				login_result = 0;
				break;
			}	
		}
		while(login_result == 2)
		{
			student_operation = menu_function[2]();
			if(student_operation != 0)
			{
				student_function[student_operation - 1]();
			}
			else
			{
				login_result = 0;
				break;
			}
		}
		while(login_result == 3)
		{
			teacher_operation = menu_function[3]();
			if(teacher_operation  != 0)
			{
				teacher_function[teacher_operation - 1]();
			}
			else
			{
				login_result = 0;
				break;
			}
		}
	}
	return 0;
}
