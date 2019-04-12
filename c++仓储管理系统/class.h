#ifndef _CLASS_H_
#define _CLASS_H_
#include <iostream>
#include <ctime>
#include <fstream>
#include <string>
#include <ctime>
#include "class_goods.h"
using namespace std;
#define MEMBER_COUT_STR "name\tnum\tsex age password\tjobkind\tsucceedlogin\t\t\tfaillogin\t\t\t"
class MEMBER
{
	public:
		MEMBER(string name = "noname",int num = 0,char sex = 'M',int age =18,string job_kind = "**",string password = "123456",time_t time_succeed_login_sec = 0,string time_succeed_login_str = "norecord",time_t time_failed_login_sec = 0,string time_failed_login_str = "norecord");
		void InputMemberInfo();
		//when adding member,change the default info
		void WriteSingleMemberInfoIntoFile(ofstream&)const;
		void ModifyPasswordBySelf();
		void ReadSingleMemberInfoFromFile(ifstream&);
		void ViewMemberInfo()const;
		bool CheckLoginQualication();
		void WriteLoginRecord(ofstream&);
		//void WriteLoginRecord();
		//view the info of self


		void ChangeMemberNumber(bool);
		void SetMemberNum(int);
		void SetMemberJobKind(string);
		void SetSucceedLoginTime();
		void SetFailedLoginTime();
		void SetGoodsSets(GOODS_SETS*);
		void SetTerminalHide();
		void SetTerminalShow();

		int    GetMemberNum()const;
		string GetMemberName();
		string GetMemberPassword();
		size_t GetSucceedLoginSec();
		size_t GetFailedLoginSec();
		string GetSucceedLoginStr();
		string GetFailedLoginstr();
		void ShowLastTimeSucceedLoginTime();
		int GetMemberNumber()const;
		GOODS_SETS* GetGoodsSets();
		//three kinds of member's operation on goods is different
		virtual void OperationOnGoods() = 0;
		//when loging in,three kinds of member need to access different menu
		virtual int ChooseOperation() = 0;
	private:
		string name;
		int num;
		char sex;
		int age;
		string job_kind;
		string password;
		string time_succeed_login_str;
		time_t time_succeed_login_sec;
		string time_failed_login_str;
		time_t time_failed_login_sec;
		static int member_number;
		GOODS_SETS* goods_sets;
};
#endif
