#include <iostream>
#include <fstream>
#include <string>
#include "derived_class.h"
#include <cstdlib>
#include <cmath>
int max_member_number = 0;
using namespace std;
void IN_STORAGE_STAFF ::OperationOnGoods()
{
	cout << "call in_storage_staff " << endl;
	//add good && creat QRcode
	GOODS* pGoods = new GOODS;
	int num;
	pGoods->InputGoodsInfo();
	pGoods->SetGoodsInStorageTime();
	pGoods->SetGoodsState("in");
	//pGoods->SetGoodsInStorageStaff(this->GetMemberName());
	string current_member_name = this->GetMemberName();
	pGoods->SetGoodsInStorageStaff(current_member_name);
	num = this->GetGoodsSets()->GetGoodsMaxTrackingNum();
	pGoods->SetGoodsTrackingNum(num + 1);
	pGoods->WriteInStorageRecord();
	this->GetGoodsSets()->push_back(pGoods);
	this->GetGoodsSets()->WriteAllGoodsInfo();
	//QRcode;
	string QR_code_str = pGoods->GetQRcodeContents();
	pGoods->CreatGoodsQRCode(QR_code_str);
}
int IN_STORAGE_STAFF :: ChooseOperation()
{
	COMON_STAFF_OPERATION common_staff_menu;
	while(1)
	{
		common_staff_menu.ShowMenu();
		int operation_num = common_staff_menu.GetOperationNumber();
		//system("cls");
		system("clear");
		if(operation_num == 1)
		{
			this->ModifyPasswordBySelf();
			getchar();
		}
		if(operation_num == 2)
		{
			this->ViewMemberInfo();
			getchar();
		}
		if(operation_num == 3)
		{
			this->OperationOnGoods();
			getchar();
		}
		if(operation_num == 0)
		{
			break;
		}
	}
	return 0;
}
void OUT_STORAGE_STAFF :: OperationOnGoods()
{
	cout << "call out_storage_staff " << endl;
	//simulate scanf QRcode
	string tar_tracking_num;
	cout << "please input the goods' tracking num you want to delete:" << endl;
	cin >> tar_tracking_num;
	int tracking_num = 0;
	int i = 10;
	int j = 0;
	char buf[10];
	while(tar_tracking_num[i] >= '0' && tar_tracking_num[i] <= '9')
	{
		buf[j] = tar_tracking_num[i];
		i ++;
		j ++;
	}
	buf[j] = '\0';
	string buf_info(buf);
	string QRcode_name("rm ");
	QRcode_name += buf_info ;
	QRcode_name += ".png";
	system(QRcode_name.c_str());
	i = 0;
	while(buf[i] != '\0')
	{
		tracking_num += (buf[i] - '0') *pow(10,j - 1 - i);
		i++;
	}
	GOODS* pGoods = this->GetGoodsSets()->FindGoodsByTrackingNum(tracking_num);
	if(pGoods != NULL)
	{
		//set relative info	
		pGoods->SetGoodsOutStorageTime();
		string current_member_name = this->GetMemberName();
		pGoods->SetGoodsOutStorageStaff(current_member_name);
		//pGoods->SetGoodsOutStorageStaff(this->GetMemberName());
		pGoods->SetGoodsState("out");
		pGoods->WriteOutStorageRecord();
		//delete QRcode
		this->GetGoodsSets()->DeleteGoodsByTrackingNum(tracking_num);
		this->GetGoodsSets()->WriteAllGoodsInfo();
	}
}
int OUT_STORAGE_STAFF :: ChooseOperation()
{
	COMON_STAFF_OPERATION common_staff_menu;
	while(1)
	{
		common_staff_menu.ShowMenu();
		int operation_num = common_staff_menu.GetOperationNumber();
		//system("cls");
		system("clear");
		if(operation_num == 1)
		{
			this->ModifyPasswordBySelf();
			getchar();
		}
		if(operation_num == 2)
		{
			this->ViewMemberInfo();
			getchar();
		}
		if(operation_num == 3)
		{
			this->OperationOnGoods();
			getchar();
		}
		if(operation_num == 0)
		{
			break;
		}
	}
	return 0;
}
void GENERAL_STAFF :: OperationOnGoods()
{
	cout << "call general_staff " << endl;
	int tar_tracking_num;
	cout << "Please input the goods' tracking num:" << endl;
	cin >> tar_tracking_num;
	this->GetGoodsSets()->FindGoodsByTrackingNum(tar_tracking_num);
}
int GENERAL_STAFF :: ChooseOperation()
{
	COMON_STAFF_OPERATION common_staff_menu;
	while(1)
	{
		common_staff_menu.ShowMenu();
		int operation_num = common_staff_menu.GetOperationNumber();
		//system("cls");
		system("clear");
		if(operation_num == 1)
		{
			this->ModifyPasswordBySelf();
			getchar();
		}
		if(operation_num == 2)
		{
			this->ViewMemberInfo();
			getchar();
		}
		if(operation_num == 3)
		{
			this->OperationOnGoods();
			getchar();
		}
		if(operation_num == 0)
		{
			break;
		}
	}
	return 0;
}
void SUPER_USER::SetSuperUser(STAFF_SETS<IN_STORAGE_STAFF>* pf1 ,STAFF_SETS<OUT_STORAGE_STAFF>* pf2 ,STAFF_SETS<GENERAL_STAFF>* pf3 )
{
	this->in_staff_objec_pointer = pf1;
	this->out_staff_objec_pointer = pf2;
	this->general_staff_objec_pointer = pf3;
}
void SUPER_USER :: GetMaxMemberNum()
{
	this->in_staff_objec_pointer->GetCurrentMaxMemberNum();
	this->out_staff_objec_pointer->GetCurrentMaxMemberNum();
	this->general_staff_objec_pointer->GetCurrentMaxMemberNum();
}
void SUPER_USER ::OperationOnGoods()
{
	cout << "call super_user" << endl;
	SUPER_USER_OPERATION_GOODS menu5;
	while(1)
	{
		menu5.ShowMenu();
		int menu5_num = menu5.GetOperationNumber();
		//system("cls");
		system("clear");
		if(menu5_num ==0)
		{
			break;
		}
		if(menu5_num == 1)
		{
			this->GetGoodsSets()->PrintAllGoodsInfo();
			getchar();
		}
		if(menu5_num == 2)
		{
			this->GetGoodsSets()->FindGoodsByInStorageStaffName();
			getchar();
		}

		if(menu5_num == 3)
		{
			this->GetGoodsSets()->FindGoodsByInStorageTime();
			getchar();
		}
		if(menu5_num == 4)
		{
			this->ViewGoodsInStorageRecord();
			getchar();
		}
		if(menu5_num == 5)
		{
			this->ViewGoodsOutStorageRecord();
			getchar();
		}
	}
}
void SUPER_USER :: ChooseOperationOnStaffs(int menu1_num)
{
	SUPER_USER_OPERATION_STAFF menu2;
	menu2.ShowMenu();
	int menu2_num = menu2.GetOperationNumber();
	//system("cls");
	system("clear");
	if(menu2_num == 1)
	{
		(in_staff_objec_pointer->*(*(in_staff_objec_pointer->GetMemberFunctionPointer()+ menu1_num)))();
		getchar();
	}
	if(menu2_num == 2)
	{
		(out_staff_objec_pointer->*(*(out_staff_objec_pointer->GetMemberFunctionPointer()+ menu1_num)))();
		getchar();
	}
	if(menu2_num == 3)
	{
		(general_staff_objec_pointer->*(*(general_staff_objec_pointer->GetMemberFunctionPointer()+ menu1_num)))();
		getchar();
	}
	(in_staff_objec_pointer->*(*(in_staff_objec_pointer->GetMemberFunctionPointer()+ 8)))();
	(out_staff_objec_pointer->*(*(out_staff_objec_pointer->GetMemberFunctionPointer()+ 8)))();
	(general_staff_objec_pointer->*(*(general_staff_objec_pointer->GetMemberFunctionPointer()+ 8)))();
}
//member function pointer
void SUPER_USER ::ChooseOperationOnSelf()
{
	COMON_STAFF_OPERATION  menu4;

	while(1)
	{
		menu4.ShowMenu();
		int menu4_num = menu4.GetOperationNumber();
		//system("cls");
		system("clear");
		if(menu4_num == 0 )
		{
			break;
		}
		if(menu4_num == 1)
		{
			this->ModifyPasswordBySelf();
			getchar();
		}
		if(menu4_num == 2)
		{
			this->ViewMemberInfo();
			getchar();
		}
		if(menu4_num == 3)
		{
			this->OperationOnGoods();
			getchar();
		}
	}
}
int SUPER_USER::ChooseOperation()
{
	SUPER_USER_OPERATION menu1;
	while(1)
	{
		menu1.ShowMenu();
		int menu1_num = menu1.GetOperationNumber();
		//system("cls");
		system("clear");
		if(menu1_num == 0)
		{
			break;
			//goto login menu;
		}
		if(menu1_num >= 1 && menu1_num <= 6)
		{
			this->GetMaxMemberNum();
			this->ChooseOperationOnStaffs(menu1_num);
			getchar();
		}
		if(menu1_num == 7)
		{
			int num = 0;
			cout << "Please input the member's num you want to find :" << endl;
			cin >> num;
			bool find_flag = false;
			find_flag = in_staff_objec_pointer->FindMemberByNum(num);
			if(!find_flag)
				find_flag = out_staff_objec_pointer->FindMemberByNum(num);
			if(!find_flag)
				find_flag = general_staff_objec_pointer->FindMemberByNum(num);
			if(!find_flag)
				cout << "No." << num << " doesn't exist in this system!" << endl;
			getchar();
		}
		if(menu1_num == 8)
		{
			string name ;
			cout << "Please input the member's name you want to find :" << endl;
			cin >> name;
			bool find_flag = false;
			find_flag = in_staff_objec_pointer->FindMemberByName(name);
			if(!find_flag)
				find_flag = out_staff_objec_pointer->FindMemberByName(name);
			if(!find_flag)
				find_flag = general_staff_objec_pointer->FindMemberByName(name);
			if(!find_flag)
				cout << "Member named " << name  << " doesn't exist in this system!" << endl;
			getchar();
		}
		if(menu1_num == 9)
		{
			this->ChooseOperationOnSelf();
			getchar();
		}
		if(menu1_num == 10)
		{
			this->ViewLoginRecord();
		}
	}
	return 0;
}
void SUPER_USER::ShowLastTimeSucceedLoginTime()
{
	cout << "Warmly welcome super user" << this->GetMemberName() << " to login!" <<endl;
	if(this->GetSucceedLoginStr() == "norecord")
	{
		cout << "It is the first time for you to login!" << endl;
	}
	else
	{
		cout << "The time when you succeed to login for the last time is " << this->GetSucceedLoginStr() <<" !" << endl; 
	}
}
void SUPER_USER :: ViewLoginRecord()
{
	ifstream input_file("login_record.txt",ifstream :: in);
	while(!input_file.is_open())
	{
		input_file.open("login_record.txt",ifstream :: in);
	}
	string name;
	int num;
	string job_kind;
	string login_time_str;
	string login_result;
	while(!input_file.eof())
	{
		name = "";
		input_file >> name;
		input_file >> num;
		input_file >> job_kind;
		input_file >> login_time_str;
		input_file >> login_result;
		if(name == "")
		{
			break;
		}
		cout << name << '\t';
		cout << num << '\t';
		cout << job_kind << '\t';
		cout << login_time_str << "\t\t\t";
		cout << login_result << "\t" << endl;
	}
	input_file.close();
}
void  SUPER_USER :: ViewGoodsInStorageRecord()
{
	ifstream input_file("goods_in_storage_record.txt",ifstream :: in);
	while(!input_file.is_open())
	{
		input_file.open("goods_in_storage_record.txt",ifstream :: in);
	}
	string goods_name;
	int goods_tracking_num;
	string goods_size;
	string goods_destination;
	string goods_state;
	string goods_in_storage_staff;
	string goods_in_storage_time_str;
	while(!input_file.eof())
	{
		goods_name = "";
		input_file >> goods_name;
		input_file >> goods_tracking_num;
		input_file >> goods_size;
		input_file >> goods_destination;
		input_file >> goods_state;
		input_file >> goods_in_storage_staff;
		input_file >> goods_in_storage_time_str;
		if(goods_name == "")
		{
			break;
		}
		cout << goods_name << '\t';
		cout << goods_tracking_num << '\t';
		cout << goods_size << '\t';
		cout << goods_destination << '\t';
		cout << goods_state << '\t';
		cout << goods_in_storage_staff << '\t';
		cout << goods_in_storage_time_str << "\t\t\t";
	}
	input_file.close();
	cout << endl;
}
void  SUPER_USER :: ViewGoodsOutStorageRecord()
{
	ifstream input_file("goods_out_storage_record.txt",ifstream :: in);
	while(!input_file.is_open())
	{
		input_file.open("goods_out_storage_record.txt",ifstream :: in);
	}
	string goods_name;
	int goods_tracking_num;
	string goods_size;
	string goods_destination;
	string goods_state;
	string goods_out_storage_staff;
	string goods_out_storage_time_str;
	while(!input_file.eof())
	{
		goods_name = "";
		input_file >> goods_name;
		input_file >> goods_tracking_num;
		input_file >> goods_size;
		input_file >> goods_destination;
		input_file >> goods_state;
		input_file >> goods_out_storage_staff;
		input_file >> goods_out_storage_time_str;
		if(goods_name == "")
		{
			break;
		}
		cout << goods_name << '\t';
		cout << goods_tracking_num << '\t';
		cout << goods_size << '\t';
		cout << goods_destination << '\t';
		cout << goods_state << '\t';
		cout << goods_out_storage_staff << '\t';
		cout << goods_out_storage_time_str << "\t\t\t";
	}
	cout << endl;
	input_file.close();
}
