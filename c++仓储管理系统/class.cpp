#include "class.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include "class_goods.h"
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#define ECHOFLAGS (ECHO | ECHOE | ECHOK | ECHONL)
using namespace std;
/******  class member_funtion ********/

MEMBER :: MEMBER(string name,int num,char sex,int age,string job_kind,string password,time_t time_succeed_login_sec,string time_succeed_login_str,time_t time_failed_login_sec,string time_failed_login_str)
{

	this->name = name;
	this->num = num;
	this->sex = sex;
	this->age = age;
	this->job_kind = job_kind;
	this->password = password;
	this->time_failed_login_str = time_failed_login_str;
	this->time_failed_login_sec = time_failed_login_sec;

	this->time_succeed_login_sec = time_succeed_login_sec;
	this->time_succeed_login_str = time_succeed_login_str;
}
int MEMBER :: member_number = 0;
void MEMBER ::InputMemberInfo()
{
	cout << "Please input the member name:" << endl;
	cin >> this->name;
	while(1)
	{
		cout << "Please input the member sex:" << endl;
		cin >> this->sex;
		if(sex == 'M' || sex == 'W')
			break;
		else
			continue;
	}
	while(1)
	{
		cout << "Please input the member age:" << endl;
		cin >> this->age;
		if(age >= 18 && age <= 60)
			break;
		else
			continue;
	}
	cout << "Please input the member password:" << endl;
	this->SetTerminalHide();
	cin >> this->password;
	this->SetTerminalShow();
}
bool MEMBER :: CheckLoginQualication()
{
	time_t failed_login_time = this->GetFailedLoginSec();
	if(failed_login_time == 0)
		return true;
	else
	{
		time_t current_time = time(NULL);
		if(current_time - failed_login_time >= 180)
			return true;
		else
			return false;
	}
}
void MEMBER :: WriteSingleMemberInfoIntoFile(ofstream& output_file)const
{
	output_file << this->name << '\t';
	output_file << this->num << '\t';
	output_file << this->sex << '\t';
	output_file << this->age << '\t';
	output_file << this->password << "\t\t";
	output_file << this->job_kind << '\t';
	output_file << this->time_succeed_login_sec << '\t';
	output_file << this->time_succeed_login_str << "\t\t\t";
	output_file << this->time_failed_login_sec << '\t';
	output_file << this->time_failed_login_str << "\t\t\t";
	output_file << MEMBER :: member_number << '\t';
	output_file << endl;
}
void MEMBER :: SetTerminalHide()
{
	struct termios ter_buf;
	while(tcgetattr(fileno(stdin),&ter_buf) != 0);
	ter_buf.c_lflag &= ~ECHOFLAGS;
	while(tcsetattr(fileno(stdin),TCSAFLUSH,&ter_buf) != 0);
}
void MEMBER :: SetTerminalShow()
{
	struct termios ter_buf;
	while(tcgetattr(fileno(stdin),&ter_buf) != 0);
	ter_buf.c_lflag |= ECHOFLAGS;
	while(tcsetattr(fileno(stdin),TCSAFLUSH,&ter_buf) != 0);
}
void MEMBER :: ModifyPasswordBySelf()
{
	string old_password;
	string new_password;
	string verify_password;
	int error_count = 3;
	char verify_ch = '\0';

	cout << "Please input your old password:" << endl;
	this->SetTerminalHide();
	cin >> old_password;
	this->SetTerminalShow();

	while((--error_count) && (old_password != this->password))
	{
		cout << "You input the wrong password!" << endl;
		cout << "You can try "  << error_count << " times!" << endl;
		cout << "Please input your old password:" << endl;;
		this->SetTerminalHide();
		cin >> old_password;
		this->SetTerminalShow();

	}
	if(old_password != this->password)
	{
		cout << "Failed to modify your password because the old password you input is wrong !" << endl;
	}
	else
	{
		cout << "Please input your new password:" << endl;
		this->SetTerminalHide();
		cin >> new_password;
		this->SetTerminalShow();
		// input new password
		cout << "Please input your new password one more time:" << endl;
		this->SetTerminalHide();
		cin >> verify_password;
		this->SetTerminalShow();
		//verify the new password
		if(new_password != verify_password)
		{
			cout << "You input the wrong password when you verified!" << endl;
			cout << "Failed to modify your password!" << endl;
			//when input different numbers,do this
		}
		else
		{
			cout << "Please select: Y or y for modifying your password, N or n for not modifying your password!" << endl;
			getchar();
			cin >> verify_ch;
			if(verify_ch == 'Y' || verify_ch == 'y')
			{
				this->password = new_password;
				cout << "Succeed to modify your password!" << endl;
			}
			if(verify_ch == 'N' || verify_ch == 'n')
			{
				cout << " You quit to modify your password!" << endl;
			}
		}
	}
}
void MEMBER :: ReadSingleMemberInfoFromFile(ifstream& input_file)
{
	input_file >> this->name ;
	input_file >> this->num ;
	input_file >> this->sex ;
	input_file >> this->age ;
	input_file >> this->password ;
	input_file >> this->job_kind ;
	input_file >> this->time_succeed_login_sec ;
	input_file >> this->time_succeed_login_str ;
	input_file >> this->time_failed_login_sec ;
	input_file >> this->time_failed_login_str ;
	input_file >> MEMBER :: member_number  ;
}
void MEMBER :: ViewMemberInfo()const
{
	//cout << MEMBER_COUT_STR << endl;
	cout << this->name << '\t';
	cout << this->num << '\t';
	cout << this->sex << "   ";
	cout << this->age << "  ";
	cout << this->password << "\t\t";
	cout << this->job_kind << '\t';
	cout << this->time_succeed_login_str << "\t\t\t";
	cout << this->time_failed_login_str << "\t\t\t";
	cout << MEMBER :: member_number << '\t';
	cout << endl;
}

void MEMBER :: ChangeMemberNumber(bool flag)
{
	if(flag) 
		member_number ++;
	else
		member_number --;
}

void MEMBER ::SetGoodsSets(GOODS_SETS* goods_sets)
{
	this->goods_sets = goods_sets;
}
void MEMBER :: SetMemberNum(int num)
{
	this->num = num;
}
void MEMBER :: SetMemberJobKind(string job_kind)
{
	this->job_kind = job_kind;
}
void MEMBER :: SetSucceedLoginTime()
{
	time_t current_time_sec = time(NULL);
	string current_time_str = ctime(&current_time_sec);
	current_time_str[3] = '-';
	current_time_str[7] = '-';
	current_time_str[10] = '-';
	current_time_str[19] = '-';
	this->time_succeed_login_sec = current_time_sec;
	this->time_succeed_login_str = current_time_str;
}
void MEMBER :: SetFailedLoginTime()
{
	time_t current_time_sec = time(NULL);
	string current_time_str = ctime(&current_time_sec);
	current_time_str[3] = '-';
	current_time_str[7] = '-';
	current_time_str[10] = '-';
	current_time_str[19] = '-';
	this->time_failed_login_sec = current_time_sec;
	this->time_failed_login_str = current_time_str;
}

int MEMBER :: GetMemberNumber()const
{
	return this->member_number;
}

int MEMBER :: GetMemberNum()const
{
	return this->num;
}
string MEMBER :: GetMemberName()
{
	return this->name;
}
string MEMBER::GetMemberPassword()
{
	return this->password;
}

size_t MEMBER :: GetSucceedLoginSec()
{
	return this->time_succeed_login_sec;
}
size_t MEMBER :: GetFailedLoginSec()
{
	return this->time_failed_login_sec;
}
string MEMBER :: GetSucceedLoginStr()
{
	return this->time_succeed_login_str;
}
string MEMBER :: GetFailedLoginstr()
{
	return this->time_failed_login_str;
}
GOODS_SETS* MEMBER :: GetGoodsSets()
{
	return goods_sets;
}
void MEMBER:: ShowLastTimeSucceedLoginTime()
{
	cout << "Warmly welcome No." << num << " member to login!" <<endl;
	if(this->time_succeed_login_str == "norecord")
	{
		cout << "It is the first time for you to login!" << endl;
	}
	else
	{
		cout << "The time when you succeed to login for the last time is " << time_succeed_login_str <<"!" << endl; 
	}
}
void MEMBER :: WriteLoginRecord(ofstream& output_file)
{
	output_file << this->name << '\t';
	output_file << this->num << '\t';
	output_file << this->job_kind << '\t';
}

