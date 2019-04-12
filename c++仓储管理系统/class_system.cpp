#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "class.h"
#include "derived_class.h"
#include "class_template.h"
#include "class_system.h"
#include "class_menu.h"
#include <cstdlib>
#include "class_goods.h"
int template_login_flag = 0;
SYSTEM :: SYSTEM()
{
	this->in_staff = new STAFF_SETS<IN_STORAGE_STAFF>; 
	this->out_staff	= new STAFF_SETS<OUT_STORAGE_STAFF> ;
	this-> general_staff = new STAFF_SETS<GENERAL_STAFF> ;
	this->super_user = new SUPER_USER ;
	this->goods_sets = new GOODS_SETS;
	this->goods_sets ->ReadAllGoodsInfoFromFile();
	this->super_user->SetGoodsSets(goods_sets);
	ifstream input_file("super_user.txt",ifstream :: in);
	(this->super_user)->ReadSingleMemberInfoFromFile(input_file);
	input_file.close();
	//read super_user info
	(this->in_staff)->ReadAllMemberIntoFromFile();
	(this->out_staff)->ReadAllMemberIntoFromFile();
	(this->general_staff)->ReadAllMemberIntoFromFile();
	//read three kind of members info
	super_user->SetSuperUser(this->in_staff,this->out_staff,this->general_staff);
	//set super_use in order to manage members
	this->check_login_flag = 0;
}
void SYSTEM :: SetCurrentInStaff(IN_STORAGE_STAFF*& pInStaff)
{
	this->current_in_staff = pInStaff;
}
void SYSTEM :: SetCurrentOutStaff(OUT_STORAGE_STAFF*& pOutStaff)
{
	this->current_out_staff = pOutStaff;
}
void SYSTEM :: SetCurrentGenaralStaff(GENERAL_STAFF*& pGeneralStaff)
{
	this->current_general_staff = pGeneralStaff;
}
void SYSTEM ::SetCheckLoginFlag(int flag)
{
	this->check_login_flag = flag;
}
void  SYSTEM :: WriteSucceedLoginRecord(MEMBER* member)
{
	ofstream output_file("login_record.txt",ofstream :: app);
	while(!output_file.is_open())
	{
		output_file.open("login_record.txt",fstream :: app);
	}
	member->WriteLoginRecord(output_file);
	output_file << member->GetSucceedLoginStr()<< "\t\t\t";
	output_file << "succeed" << "\t";
	output_file << endl;
}
void SYSTEM :: WriteFailedLoginRecord(MEMBER* member)
{
	ofstream output_file("login_record.txt",ofstream :: app);
	while(!output_file.is_open())
	{
		output_file.open("login_record.txt",fstream :: app);
	}
	member->WriteLoginRecord(output_file);
	output_file << member->GetFailedLoginstr()<< "\t\t\t";
	output_file << "failed" << "\t";
	output_file << endl;
}
void SYSTEM :: SystemRun()
{
	int operation_return_value ;
	while(check_login_flag == 0)
	{
		this->SystemCheckLogin();
		if(check_login_flag == -1)
		{
			this->SystemExit();
			break;
		}

		while(check_login_flag == 1)
		{
			operation_return_value = this->super_user->ChooseOperation();
			if(operation_return_value == 0)
			{
				this->SetCheckLoginFlag(0);
			}
		}
		while(check_login_flag == 2)
		{
			operation_return_value = this->current_in_staff->ChooseOperation();
			if(operation_return_value == 0)
			{
				this->SetCheckLoginFlag(0);
			}
		}
		while(check_login_flag == 3)
		{			
			operation_return_value = this->current_out_staff->ChooseOperation();
			if(operation_return_value == 0)
			{
				this->SetCheckLoginFlag(0);
			}
		}			
		while(check_login_flag == 4)
		{			
			operation_return_value = this->current_general_staff->ChooseOperation();
			if(operation_return_value == 0)
			{
				this->SetCheckLoginFlag(0);
			}
		}
	}			
}
void SYSTEM ::CheckLoginSuperUser()
{
	if(!(this->super_user->CheckLoginQualication()))
	{
		cout << "You can't login now because you don't have the qualication!" << endl;
		return ;
	}
	string name;
	string password;
	string super_user_name = this->super_user->GetMemberName();
	string super_user_password = this->super_user->GetMemberPassword();
	int login_error_count = 3;
	cout << "Please input super user's name:" << endl;
	cin >> name;
	cout << "Please input super user's password:" << endl;
	this->super_user->SetTerminalHide();
	cin >> password;
	this->super_user->SetTerminalShow();
	while(--login_error_count && (!(name == super_user_name) || !(password == super_user_password)))
	{
		cout << "Super user failed to login in!" << endl;
		cout << "You can try " << login_error_count << " times!" << endl;
		cout << "Please input super user's name:" << endl;
		cin >> name;
		cout << "Please input super user's password:" << endl;
		this->super_user->SetTerminalHide();
		cin >> password;
		this->super_user->SetTerminalShow();
	}
	if((name == super_user_name) && (password == super_user_password))
	{
		cout << "Super user succeed to login!" << endl;
		this->super_user->ShowLastTimeSucceedLoginTime();
		this->super_user->SetSucceedLoginTime();
		this->WriteSucceedLoginRecord(this->super_user);
		this->SetCheckLoginFlag(1);
	}
	else
	{
		cout << "Super user failed to login!" << endl;
		cout << " You have input the wrong password three times! You don't have the qualication to login in three minutes!" << endl;
		this->super_user->SetFailedLoginTime();
		this->WriteFailedLoginRecord(this->super_user);
	}
	ofstream output_file("super_user.txt",fstream :: out);
	this->super_user->WriteSingleMemberInfoIntoFile(output_file);
	output_file.close();
}
void SYSTEM :: CheckLoginInStorageStaff() 
{
	IN_STORAGE_STAFF* tmp = this->in_staff->CheckLoginByMemberNum();
	if(tmp != NULL)
	{
		this->SetCurrentInStaff(tmp);
		if(template_login_flag == 1)
		{
			//this->SetCurrentInStaff(tmp);
			this->current_in_staff->SetGoodsSets(goods_sets);
			this->WriteSucceedLoginRecord(this->current_in_staff);
			this->SetCheckLoginFlag(2);
		}
		if(template_login_flag == 2)
		{
			this->WriteFailedLoginRecord(this->current_in_staff);
		}
		this->in_staff->WriteAllMemberInfoIntoFile();
	}		
	//this->in_staff->WriteAllMemberInfoIntoFile();
}
void SYSTEM :: CheckLoginOutStorageStaff()
{
	OUT_STORAGE_STAFF* tmp = this->out_staff->CheckLoginByMemberNum();
	if(tmp != NULL)
	{
		this->SetCurrentOutStaff(tmp);
		if(template_login_flag == 1)
		{
			//	this->SetCurrentOutStaff(tmp);
			this->current_out_staff->SetGoodsSets(goods_sets);
			this->WriteSucceedLoginRecord(this->current_out_staff);
			this->SetCheckLoginFlag(3);
		}
		if(template_login_flag == 2)
		{
			this->WriteFailedLoginRecord(this->current_out_staff);
		}
	}
	this->out_staff->WriteAllMemberInfoIntoFile();
}
void SYSTEM :: CheckLoginGeneralStaff()
{
	GENERAL_STAFF* tmp = this->general_staff->CheckLoginByMemberNum();
	if(tmp != NULL)
	{
		this->SetCurrentGenaralStaff(tmp);
		if(template_login_flag == 1)
		{
			//this->SetCurrentGenaralStaff(tmp);
			this->current_general_staff->SetGoodsSets(goods_sets);
			this->WriteSucceedLoginRecord(this->current_general_staff);
			this->SetCheckLoginFlag(4);
		}
		if(template_login_flag == 2)
		{
			this->WriteFailedLoginRecord(this->current_general_staff);
		}
	}
	this->general_staff->WriteAllMemberInfoIntoFile();
}
void SYSTEM::SystemCheckLogin()
{
	LOGIN login_menu;
	//while(1)
	//{
	login_menu.ShowMenu();
	int menu_num = login_menu.GetOperationNumber();
	//system("cls");
	system("clear");
	if(menu_num == 0)
	{
		this->SetCheckLoginFlag(-1);
		//break;
	}
	if(menu_num == 1)
	{
		this->CheckLoginSuperUser();
	}
	if(menu_num == 2)
	{
		this->CheckLoginInStorageStaff();
	}
	if(menu_num == 3)
	{
		this->CheckLoginOutStorageStaff();
	}
	if(menu_num == 4)
	{
		this->CheckLoginGeneralStaff();
	}
	//}
}
void SYSTEM::SystemExit()
{
	cout << "Exit system!" << endl;
	cout << "Deleting space now!" << endl;
	delete (this->super_user);
	delete (this->in_staff);
	delete (this->out_staff);
	delete (this->general_staff);
	delete (this->goods_sets);
}

