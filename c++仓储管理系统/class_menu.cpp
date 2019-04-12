#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "class.h"
#include "derived_class.h"
#include "class_template.h"
#include "class_menu.h"
using namespace std;

MENU :: MENU(string start_str  ,string body_str ,int max_num,int min_num )
{
	this->menu_str_start = start_str;
	this->menu_str_body = body_str;
	this->max_operation_number = max_num;
	this->min_operation_number = min_num;
}
void MENU :: MyPrint(string str)
{
	cout << str << endl;
}
void MENU :: MyPrintStar()
{
	cout << "**************************************" << endl;
}
int MENU :: StringIntoInt(string str)
{
	int str_len = str.length();
	int rel = 0;
	if(str_len >= 2)
	{
		rel = (str[str_len - 2] - '0') * 10 + str[str_len - 1] - '0';
	}
	else
	{
		rel = str[0] - '0';
	}
	return rel;
}
int MENU :: CheckOperationNum(int num)
{
	return (num >= min_operation_number && num <= max_operation_number) ? num : -1;
}

int MENU :: GetOperationNumber()
{
	int result = 0;
	string tmp;
	while(1)
	{
		MyPrint("Please input the number:");
		cin >> tmp;
		result = CheckOperationNum(StringIntoInt(tmp));
		if(result != -1)
		{
			break;
		}
		else
		{
			MyPrint("You input the wrong number!");
		}
	}
	return result;
}
void MENU :: ShowMenu()
{
	MyPrint(menu_str_start);
	MyPrintStar();
	MyPrint(menu_str_body);
	MyPrintStar();
}
/************** derived class ******************/
LOGIN ::LOGIN():MENU(login_start_str,login_body_str,login_max_num ,login_min_num)
{
	;
}
SUPER_USER_OPERATION::SUPER_USER_OPERATION():MENU(super_user_operation_start_str,super_user_operation_body_str,super_user_operation_max_num ,super_user_operation_min_num)
{
	;
}
SUPER_USER_OPERATION_STAFF ::SUPER_USER_OPERATION_STAFF():MENU(super_user_operation_staff_start_str,super_user_operation_staff_body_str,super_user_operation_staff_max_num ,super_user_operation_staff_min_num)
{
	;
}
SUPER_USER_OPERATION_WAY::SUPER_USER_OPERATION_WAY():MENU(super_user_operation_way_start_str,super_user_operation_way_body_str,super_user_operation_way_max_num ,super_user_operation_way_min_num)
{
	;
}
SUPER_USER_OPERATION_GOODS ::SUPER_USER_OPERATION_GOODS():MENU(super_user_operation_goods_start_str,super_user_operation_goods_body_str,super_user_operation_goods_max_num ,super_user_operation_goods_min_num)
{
	;
}
COMON_STAFF_OPERATION ::COMON_STAFF_OPERATION():MENU(common_staff_operation_start_str,common_staff_operation_body_str,common_staff_operation_max_num ,common_staff_operation_min_num)
{
	;
}
