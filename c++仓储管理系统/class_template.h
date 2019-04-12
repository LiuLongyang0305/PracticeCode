#ifndef _CLASS_TEMPLATE_H_
#define _CLASS_TEMPLATE_H_
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "class.h"
#include "derived_class.h"
#include <typeinfo>
#include "class_goods.h"
extern int max_member_number;
extern int template_login_flag;
using namespace std;
/******* define class_template ********/
template <class class_name>
class STAFF_SETS:public vector<class_name*>
{
	private:
		typedef void (STAFF_SETS<class_name>::*tem_class_mem_fun_ptr)();
		tem_class_mem_fun_ptr pfunction[9];
		//for super_user to operation staffs
	public:
		STAFF_SETS()
		{
			pfunction[0] = &STAFF_SETS::ReadAllMemberIntoFromFile;
			pfunction[1] = &STAFF_SETS::AddMember;//add
			pfunction[2] = &STAFF_SETS::DeleteMemberByNum;//delete
			pfunction[3] = &STAFF_SETS::ModifyMemberInfo;//modify
			pfunction[4] = &STAFF_SETS::ViewAllMemberInfo;//view
			pfunction[5] = &STAFF_SETS::DeleteAllMember;
			//delete department member
			pfunction[6] = &STAFF_SETS::FindMemberByJobKind;
			pfunction[7] = &STAFF_SETS::DeleteMemberByName;
			pfunction[8] = &STAFF_SETS::WriteAllMemberInfoIntoFile;
		}
		tem_class_mem_fun_ptr* GetMemberFunctionPointer()
		{
			return pfunction;
		}
		~STAFF_SETS();
		void ViewAllMemberInfo();
		void AddMember();
		bool FindMemberByNum(int);
		bool FindMemberByName(string);
		//find by job_kind,namely view all member info
		void FindMemberByJobKind();
		void DeleteMemberByNum();
		void DeleteMemberByName();
		void DeleteAllMember();
		void WriteAllMemberInfoIntoFile();
		void ReadAllMemberIntoFromFile();
		void ModifyMemberInfo();
		void GetCurrentMaxMemberNum();
		class_name* CheckLoginByMemberNum();
		string GetFileName();
		//three kinds of staff were written into different files
};


/************** member function resolve **************/
template <class class_name>
STAFF_SETS<class_name> :: ~STAFF_SETS()
{
	cout << "Deleting staff sets now!" << endl;
	//vector<class_name*>::iterator it;
	typename vector<class_name*>::iterator it;
	it = this->begin();
	while(it != this->end())
	{
		class_name* tmp = NULL;
		tmp = *it;
		*it = NULL;
		delete tmp;
		it ++;
	}
}
template <class class_name>
void STAFF_SETS<class_name> :: ViewAllMemberInfo()
{
	//vector<class_name*> :: iterator it;
	typename vector<class_name*>::iterator it;
	it = this->begin();
	while(it != this->end())
	{
		(*it)->ViewMemberInfo();
		it ++;
	}
}
template <class class_name>
void STAFF_SETS<class_name> ::GetCurrentMaxMemberNum()
{
	//vector<class_name*> :: iterator it;
	typename vector<class_name*>::iterator it;
	it = this->begin();
	while(it != this->end())
	{
		int max_num = (*it)-> GetMemberNum();
		if(max_num > max_member_number)
		{
			max_member_number = max_num;
		}
		it ++;
	}
}
template <class class_name>
void STAFF_SETS<class_name> ::ModifyMemberInfo()
{
	int num; 
	bool modify_flag = false;
	//vector<class_name*> ::iterator it;
	typename vector<class_name*>::iterator it;
	it = this->begin();
	cout << "Please input the member's num you want to modify:" << endl;
	cin >> num;
	while(it != this->end())
	{
		if((*it)->GetMemberNum() == num)
		{
			(*it)->InputMemberInfo();
			modify_flag = true;
			cout << "Succeed to Modify No." << num << " member!" << endl;
			break;
		}
		else
		{
			it ++;
		}	
	}
	if(!modify_flag)
	{
		cout << "No." << num << " member dosen't exist in " << typeid(class_name).name() << "!"<< endl;
		cout << "Failed to modify No." << num << " member!" << endl;
	}
	else
	{
		this->WriteAllMemberInfoIntoFile();
	}
}
template <class class_name>
string STAFF_SETS<class_name> :: GetFileName()
{
	string file_name;
	file_name += typeid(class_name).name();
	file_name += ".txt";
	return file_name;
}

template <class class_name>
void STAFF_SETS<class_name> :: WriteAllMemberInfoIntoFile()
{
	string file_name = GetFileName();
	ofstream output_file(file_name.c_str(),fstream :: out);
	while(!output_file.is_open())
	{
		output_file.open(file_name.c_str(),fstream :: out);
	}
	//vector<class_name*> :: iterator it;
	typename vector<class_name*>::iterator it;
	it = this->begin();
	while(it != this->end())
	{
		(*it)->WriteSingleMemberInfoIntoFile(output_file);
		it ++;
	}
	output_file.close();
}
template <class class_name>
void STAFF_SETS<class_name>::AddMember()
{
	int num = 0;
	string current_job_kind;
	class_name* tmp = new class_name;
	//invoke default constructor
	tmp->InputMemberInfo();
	tmp->SetMemberNum(max_member_number + 1);
	//set job_kind
	string current_class_name(typeid(class_name).name());
	if(current_class_name.find("IN") != -1)
	{
		current_job_kind = "in";
	}
	if(current_class_name.find("OUT") != -1)
	{
		current_job_kind = "out";
	}
	if(current_class_name.find("GENERAL") != -1)
	{
		current_job_kind = "general";
	}
	tmp->SetMemberJobKind(current_job_kind);
	this->push_back(tmp);
	tmp->ChangeMemberNumber(true);
	this->WriteAllMemberInfoIntoFile();
} 

template <class class_name> 
void STAFF_SETS<class_name> ::FindMemberByJobKind()
{
	this->ViewAllMemberInfo();
}
template <class class_name> 
bool STAFF_SETS<class_name> :: FindMemberByNum(int num)
{
	//int num;
	bool find_flag = false;
	//vector<class_name*> :: iterator it;
	typename vector<class_name*>::iterator it;
	it = this->begin();
	cout << "Please input the member's num you want to find:" << endl;
	cin >> num;
	while(it != this->end())
	{
		if((*it)->GetMemberNum() == num )
		{
			cout << "Succeed to find No." << num << " member!" << endl;
			(*it)->ViewMemberInfo();
			find_flag = true;
			//if find,print member info
			break;
		}
		else
		{
			it ++;
		}
	}
	if(it == this->end())
	{
		;//cout << "No." << num << " member dosen't exist in " << typeid(class_name).name() << endl;
		//if not find,print info
	}
	return find_flag;
} 
template <class class_name> 
bool STAFF_SETS<class_name> :: FindMemberByName(string name)
{
	//string name;
	bool find_flag = false;
	//vector<class_name*> :: iterator it;
	typename vector<class_name*>::iterator it;
	it = this->begin();
	//cout << "Please input the member's name you want to find:" << endl;
	//cin >> name;
	while(it != this->end())
	{
		if((*it)->GetMemberName() == name )
		{
			cout << "Succeed to find Member named " << name << " !" << endl;
			(*it)->ViewMemberInfo();
			find_flag = true;
			//if find,print member info
			break;
		}
		else
		{
			it ++;
		}
	}
	return find_flag;
} 
template <class class_name> 
void STAFF_SETS<class_name> ::DeleteMemberByNum()
{
	int num; 
	bool delete_flag = false;
	//vector<class_name*> ::iterator it;
	typename vector<class_name*>::iterator it;
	it = this->begin();
	cout << "Please input the member's num you want to delete:" << endl;
	cin >> num;
	while(it != this->end())
	{
		if((*it)->GetMemberNum() == num)
		{
			class_name* tmp = *it;//used to delete space
			(*it)->ChangeMemberNumber(false);
			//write members in system
			this->erase(it);
			delete tmp;
			delete_flag = true;
			cout << "Succeed to delete No." << num << " member!" << endl;
			break;
		}
		else
		{
			it ++;
		}
	}
	if(!delete_flag)
	{
		cout << "No." << num << " member dosen't exist in " << typeid(class_name).name() << "!"<< endl;
		cout << "Failed to delete No." << num << " member!" << endl;
	}
}
template <class class_name> 
void STAFF_SETS<class_name> :: DeleteMemberByName()
{
	string name;
	bool delete_flag = false;
	//vector<class_name*> :: iterator it;
	typename vector<class_name*>::iterator it;
	it = this->begin();
	cout << "Please input the member's name you want to delete:" << endl;
	cin >> name;
	while(it != this->end())
	{
		if((*it)->GetMemberName() == name)
		{
			class_name* tmp = *it;//used to delete space
			(*it)->ChangeMemberNumber(false);
			//class_name :: member_number -= 1;
			this->erase(it);
			delete tmp;
			delete_flag = true;
			cout << "Succeed to delete member named " << name << " !" << endl;
			break;
		}
		else
		{
			it ++;
		}	
	}
	if(!delete_flag)
	{
		cout << "Member named " << name << " dosen't exist in " << typeid(class_name).name() << "!"<< endl;
		cout << "Failed to delete  member named " << name << " !" << endl;
	}
}
template <class class_name> 
void STAFF_SETS<class_name> :: DeleteAllMember()
{
	//vector<class_name*> ::iterator it;
	typename vector<class_name*>::iterator it;
	if(!this->empty())
	{
		it = this->begin();
		while(it != this->end())
		{
			class_name* tmp = *it;
			(*it)->ChangeMemberNumber(false);
			//class_name :: member_number -= 1;
			this->erase(it);
			delete tmp;
			it ++;
		}
		//class_name :: member_number = 0;
		//this->WriteAllMemberInfoIntoFile();
	}
	else
	{
		cout << typeid(class_name).name() << " has no members! " << endl;
	}
}
template <class class_name> 
void STAFF_SETS<class_name> ::ReadAllMemberIntoFromFile()
{
	string file_name = GetFileName();
	ifstream input_file(file_name.c_str(),ifstream :: in);
	while(!input_file.is_open())
	{
		input_file.open(file_name.c_str(),ifstream :: in);
	}
	while(!input_file.eof())
	{
		class_name* pClass = new class_name;
		pClass->ReadSingleMemberInfoFromFile(input_file);
		if(pClass->GetMemberNum() != 0)//if num == 0,finish reading file
		{
			//pClass->ChangeMemberNumber(true);
			this->push_back(pClass);
		}
		else
		{
			delete pClass;
			break;
		}
	}
	input_file.close();
}
template <class class_name> 
class_name* STAFF_SETS<class_name> ::CheckLoginByMemberNum()
{
	int num;
	string user_password;
	int login_error_count = 3;
	cout << "Please input your member num:" << endl;
	cin >> num;
	cout << "Please input your member password:" << endl;
	(*this->begin())->SetTerminalHide();
	cin >>user_password;
	(*this->begin())->SetTerminalShow();
	//vector<class_name*> ::iterator it;
	typename vector<class_name*>::iterator it;
	it = this->begin();
	while(it != this->end())
	{
		if((*it)->GetMemberNum() == num)
		{
			if(!(*it)->CheckLoginQualication())
			{
				cout << "You can't login now because you don't have the qualication!" << endl;

				return *(this->end());
			}
			string user_store_password = (*it)->GetMemberPassword();
			while(--login_error_count && !(user_password == user_store_password))
			{
				cout << "You input the wrong password!" << endl;
				cout << "you can try " << login_error_count << " times" << endl;
				cout << "Please input your member password:" << endl;
				(*this->begin())->SetTerminalHide();
				cin >>user_password;
				(*this->begin())->SetTerminalShow();
				//cin >>user_password; 
			}
			if(user_password == user_store_password)
			{
				cout << "No." << num << " member succeed to login in!" << endl;
				(*it)->ShowLastTimeSucceedLoginTime();
				(*it)->SetSucceedLoginTime();
				template_login_flag = 1;
				return *it;
			}
			else
			{
				cout << "No." << num << " member failed to login in!" << endl;
				cout << " You have input the wrong password three times! You don't have the qualication to login in three minutes!" << endl;
				(*it)->SetFailedLoginTime();
				template_login_flag = 2;
				return *it;
			}
		}
		it ++;
	}
	if(it == this->end())
	{
		cout << "No." << num << " member doesn't exist!" << endl;
	} 
	return *(this->end());
}
#endif
