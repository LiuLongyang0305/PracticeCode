#ifndef _CLASS_MENU_H_
#define _CLASS_MENU_H_
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "class.h"
#include "derived_class.h"
#include "class_template.h"
using namespace std;
//used define base class && menu derived class
class MENU
{
	public:
		MENU(string start_str = "\0",string body_str = "\0",int max_num = 0,int min_num = 0);
		void ShowMenu();
		int GetOperationNumber();
	private:
		string menu_str_start;
		string menu_str_body;
		int max_operation_number;
		int min_operation_number;
		int StringIntoInt(string);
		int CheckOperationNum(int);
		void MyPrintStar();
		void MyPrint(string);
};
class LOGIN :public MENU
{
	public:
		LOGIN();
};
//choose whom login
class SUPER_USER_OPERATION:public MENU
{
	public:
		SUPER_USER_OPERATION();
};
//choose operation by super_user
class SUPER_USER_OPERATION_STAFF:public MENU
{
	public:
		SUPER_USER_OPERATION_STAFF();
};
//choose operation object
class SUPER_USER_OPERATION_WAY:public MENU
{
	public:
		SUPER_USER_OPERATION_WAY();
};
//options:name,num,job_kind
class SUPER_USER_OPERATION_GOODS:public MENU
{
	public:
		SUPER_USER_OPERATION_GOODS();
};
//options:view all goods info,view goods info by in_storage time
class COMON_STAFF_OPERATION:public MENU
{
	public:
		COMON_STAFF_OPERATION();
};
//option: add good into storage;view self info,modify self info
//login menu contents
#define login_start_str      "          ChooseMemberLogin           "
#define login_body_str   "******     1.super user         ******\n"\
                         "******     2.in storage staff   ******\n"\
                         "******     3.out storage staff  ******\n"\
                         "******     4.general staff      ******\n"\
                         "******     0.exit system        ******"
#define login_max_num 4
#define login_min_num 0

//super_user operation 
#define super_user_operation_start_str     "      SuperUserChooseOperation        "
#define super_user_operation_body_str  "***   1.add member                 ***\n"\
                                       "***   2.delete single member       ***\n"\
                                       "***   3.modify member info         ***\n"\
                                       "***   4.view  member info          ***\n"\
                                       "***   5.delete department member   ***\n"\
                                       "***   6.find  member by department ***\n"\
                                       "***   7.find  member by name       ***\n"\
                                       "***   8.find  member by num        ***\n"\
                                       "***   9.operation self && goods    ***\n"\
                                       "***   10.view login record         ***\n"\
                                       "***   0.exit  to login menu        ***"
#define super_user_operation_max_num 10
#define super_user_operation_min_num 0

#define super_user_operation_staff_start_str     "          ChooseOperationObeject       "
#define super_user_operation_staff_body_str  "******     1.in storage staff   ******\n"\
                                             "******     2.out storage staff  ******\n"\
                                             "******     3.general staff      ******\n"\
                                             "******     0.exit               ******"

//exit to super user choose operation menu
#define super_user_operation_staff_max_num 3
#define super_user_operation_staff_min_num 0

#define super_user_operation_way_start_str     "          ChooseOperationWay          "
#define super_user_operation_way_body_str    "******  1.operation by name     ******\n"\
                                             "******  2.operation by num      ******\n"\
                                             "******  3.operation by job_kind ******\n"\
                                             "******  0.exit                  ******"
//exit to super user choose operation menu
#define super_user_operation_way_max_num 3
#define super_user_operation_way_min_num 0

#define super_user_operation_goods_start_str     "          ChooseOperationOnGoods      "
#define super_user_operation_goods_body_str  "***   1.view all goods info        ***\n"\
                                             "***   2.view goods info by member  ***\n"\
                                             "***   3.view goods info by time    ***\n"\
                                             "***   4.check in storage record    ***\n"\
                                             "***   5.check out storage record   ***\n"\
                                             "***   0.exit                       ***"
//exit to super user choose operation menu
#define super_user_operation_goods_max_num 5
#define super_user_operation_goods_min_num 0

#define common_staff_operation_start_str     "          CommonStaffOperation          "
#define common_staff_operation_body_str     "******  1.modify self info      ******\n"\
                                            "******  2.view self info        ******\n"\
                                            "******  3.operation on goods    ******\n"\
                                            "******  0.exit                  ******"
//exit to common staff choose operation menu
#define common_staff_operation_max_num 3
#define common_staff_operation_min_num 0
#endif
