#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <fstream>
#include "class_goods.h"
#include <cstdlib>
#include <cstdio>
/******** class GOODS member function ******/
GOODS :: GOODS(string name ,int tracking_num ,string size,string dest ,string state,string in_staff,time_t in_time_sec ,string in_time_str ,string out_staff ,time_t out_time_sec,string out_time_str )
{
	this->name = name;
	this->tracking_num = tracking_num;
	this->size = size;
	this->destination = dest;
	this->state = state;
	this->in_storage_staff = in_staff;
	this->in_storage_time_sec = in_time_sec;
	this->out_storage_time_str = in_time_str;
	this->out_storage_staff = out_staff;
	this->out_storage_time_sec = out_time_sec;
	this->out_storage_time_str = out_time_str;
}
void GOODS :: InputGoodsInfo()
{
	cout << "Please input goods' name:" << endl;
	cin >> this->name;
	while(1)
	{
		cout << "Please input goods' size:" << endl;
		cin >> this->size;
		if(this->size == "large" || this->size == "middle" || this->size == "small")
		{
			break;
		}
	}
	cout << "Please input goods' destination:" << endl;
	cin >> this->destination;
}
void GOODS :: WriteSingleGoodsInfo(ofstream& output_file)
{
	output_file << this->name << '\t';
	output_file << this->tracking_num << '\t';
	output_file << this->size << '\t';
	output_file << this->destination << '\t';
	output_file << this->state << '\t';
	output_file << this->in_storage_staff << '\t';
	output_file << this->in_storage_time_sec << "\t\t";
	output_file << this->in_storage_time_str << "\t\t\t";
	output_file << this->out_storage_staff << '\t';
	output_file << this->out_storage_time_sec << "\t\t";
	output_file << this->out_storage_time_str << "\t\t\t";
	output_file << endl;
	//attention when reading
}
void GOODS::  ReadSingleGoodsInfo(ifstream& input_file)
{
	input_file >> this->name;
	input_file >> this->tracking_num;
	input_file >> this->size;
	input_file >> this->destination;
	input_file >> this->state;
	input_file >> this->in_storage_staff;
	input_file >> this->in_storage_time_sec;
	input_file >> this->in_storage_time_str;
	input_file >> this->out_storage_staff;
	input_file >> this->out_storage_time_sec;
	input_file >> this->out_storage_time_str;
}
void GOODS :: PrintGoodsInfo()
{
	cout << this->name << '\t';
	cout << this->tracking_num << '\t';
	cout << this->size << '\t';
	cout << this->destination << '\t';
	cout << this->state << '\t';
	cout << this->in_storage_staff << '\t';
	cout << this->in_storage_time_str << "\t\t\t";
	cout << this->out_storage_staff << '\t';
	cout << this->out_storage_time_str << "\t\t\t";
	cout << endl;
}
void GOODS :: WriteInStorageRecord()
{
	ofstream output_file("goods_in_storage_record.txt",ios :: app);
	while(!output_file.is_open())
	{
		output_file.open("goods_in_storage_record.txt",ios :: app);
	}
	output_file << this->name << '\t';
	output_file << this->tracking_num << '\t';
	output_file << this->size << '\t';
	output_file << this->destination << '\t';
	output_file << this->state << '\t';
	output_file << this->in_storage_staff << '\t';
	output_file << this->in_storage_time_str << "\t\t\t";
	output_file << endl;
	output_file.close();
}
void GOODS :: WriteOutStorageRecord()
{
	ofstream output_file("goods_out_storage_record.txt",ios :: app);
	while(!output_file.is_open())
	{
		output_file.open("goods_out_storage_record.txt",ios :: app);
	}
	output_file << this->name << '\t';
	output_file << this->tracking_num << '\t';
	output_file << this->size << '\t';
	output_file << this->destination << '\t';
	output_file << this->state << '\t';
	output_file << this->out_storage_staff << '\t';
	output_file << this->out_storage_time_str << "\t\t\t";
	output_file << endl;
	output_file.close();
}
string GOODS :: GetGoodsDestination()
{
	return this->destination;
}
string GOODS ::  GetGoodsInStorageStaff()
{
	return this->in_storage_staff;
}
string  GOODS :: GetGoodsState()
{
	return this->state;
}
string  GOODS :: GetGoodsOutStorageStaff()
{
	return this->out_storage_staff;
}
time_t  GOODS :: GetGoodsInStorageTimeSec()
{
	return this->in_storage_time_sec;
}
time_t  GOODS :: GetGoodsOutStorageTimeSec()
{
	return this->out_storage_time_sec;
}
string  GOODS :: GetGoodsOutStorageTimeStr()
{
	return this->out_storage_time_str;
}
string  GOODS :: GetGoodsInStorageTimeStr()
{
	return this->in_storage_time_str;
}
int  GOODS :: GetGoodsTrackingNum()
{
	return this->tracking_num;
}
string GOODS :: GetGoodsName()
{
	return this->name;
}
string GOODS :: GetGoodsSize()
{
	return this->size;
}
string GOODS :: GetQRcodeContents()
{
	char buff[1024];
	string num="goods_num:";
	int trac_num = this-> GetGoodsTrackingNum();
	string name= "goods_name:";
	name += this->GetGoodsName();
	string goods_size = "goods_size:";
	goods_size += this->GetGoodsSize();
	string destination = "goods_destion:" ;
	destination += this->GetGoodsDestination();
	string storage_staff = "goods_instorage_staff:";
	storage_staff += this->GetGoodsInStorageStaff();
	string goods_in_time = "goods_in_storage_time:";
	goods_in_time += GetGoodsInStorageTimeStr();
	sprintf(buff,"%s%d%c%s%c%s%c%s%c%s%c%s%c",num.c_str(),trac_num,'\n',name.c_str(),'\n',goods_size.c_str(),'\n',destination.c_str(),'\n',storage_staff.c_str(),'\n',goods_in_time.c_str(),'\n');
	string goods_QR_cotents(buff);
	return goods_QR_cotents;
}
string GOODS :: GetQRcodeName()
{
	char buff[24];
	int QR_code_num = this-> GetGoodsTrackingNum();
	sprintf(buff,"%d",QR_code_num);
	string QR_code_name(buff);
	return QR_code_name;
}
void GOODS :: CreatGoodsQRCode(string& str)
{
	string str1;
	str1 += "\"";
	str1 += str;
	str1 += "\"";
	string QRcode_name = this->GetQRcodeName();
	QRcode_name += ".png";
	str1 = "qrencode " + str1 + " -o " + QRcode_name;
	system(str1.c_str());
}
void  GOODS :: SetGoodsInStorageTime()
{
	time_t current_time_sec = time(NULL);
	string current_time_str = ctime(&current_time_sec);
	current_time_str[3] = '-';
	current_time_str[7] = '-';
	current_time_str[10] = '-';
	current_time_str[19] = '-';
	//replace space with -
	this->in_storage_time_sec = current_time_sec;
	this->in_storage_time_str = current_time_str;
}
void  GOODS :: SetGoodsOutStorageTime()
{
	time_t current_time_sec = time(NULL);
	string current_time_str = ctime(&current_time_sec);
	current_time_str[3] = '-';
	current_time_str[7] = '-';
	current_time_str[10] = '-';
	current_time_str[19] = '-';
	//replace space with -
	this->out_storage_time_sec = current_time_sec;
	this->out_storage_time_str = current_time_str;
}
void  GOODS :: SetGoodsState(string state)
{
	this->state = state;// in or out
}
void GOODS :: SetGoodsInStorageStaff(string& in_staff_name)
{
	this->in_storage_staff = in_staff_name;
}
void GOODS :: SetGoodsOutStorageStaff(string& out_staff_name)
{
	this->out_storage_staff = out_staff_name;
}
void GOODS :: SetGoodsTrackingNum(int num)
{
	this->tracking_num = num;
}
/******************* GOODS_SETS member function *********/

GOODS_SETS::~GOODS_SETS()
{
	cout << "Deleting goods sets now!" << endl;
	vector<GOODS*> :: iterator it;
	it = this->begin();
	while(it != this->end())
	{
		GOODS* tmp = *it;
		*it = NULL;
		delete tmp;
		it ++;
	}
}
void GOODS_SETS ::PrintAllGoodsInfo()
{
	vector<GOODS*> :: iterator it;
	it = this->begin();
	while(it != this->end())
	{
		(*it)->PrintGoodsInfo();
		it ++;
	}
}

void GOODS_SETS :: WriteAllGoodsInfo()
{
	ofstream output_file("goods.txt",ios :: out);
	while(!output_file.is_open())
	{
		output_file.open("goods.txt",ios :: out);
	}
	vector<GOODS*> :: iterator it;
	it = this->begin();
	while(it != this->end())
	{
		(*it)->WriteSingleGoodsInfo(output_file);
		it ++;
	}
	output_file.close();
}
void GOODS_SETS :: AddGoods(GOODS* good)
{
	this->push_back(good);
}
int  GOODS_SETS :: GetGoodsMaxTrackingNum()
{
	int max_tracking_num = 0;
	vector<GOODS*> :: iterator it;
	it = this->begin();
	while(it != this->end())
	{
		int num = (*it)->GetGoodsTrackingNum();
		if( num > max_tracking_num)
		{
			max_tracking_num = num;
		}
		it ++;
	}
	return max_tracking_num;
}
GOODS* GOODS_SETS :: FindGoodsByTrackingNum(int target_tracking_num)
{
	vector<GOODS*> :: iterator it;
	it = this->begin();
	while(it != this->end())
	{
		int num = (*it)->GetGoodsTrackingNum();
		if( num == target_tracking_num)
		{
			cout << "Succeed to fine No." << target_tracking_num << " goods!its info is as  follows:" << endl;
			(*it)->PrintGoodsInfo();
			break;
		}
		it ++;
	}
	if(it == this->end())
	{
		cout << "Failed to find No." << target_tracking_num << " goods! It doesn't exist in this storage!" << endl;
	}
	return *it;
}

void GOODS_SETS :: FindGoodsByInStorageStaffName()
{
	string in_storage_staff_name;
	bool find_flag = false;
	cout << "please input the in_storage staff's name: " << endl;
	cin >> in_storage_staff_name;
	vector<GOODS*> :: iterator it;
	it = this->begin();
	while(it != this->end())
	{
		string staff_name = (*it)->GetGoodsInStorageStaff();
		if( staff_name == in_storage_staff_name)
		{
			find_flag = true;
			(*it)->PrintGoodsInfo();
		}
		it ++;
	}
	if(find_flag == false)
	{
		cout << "There is no goods which is put into this storage by in_storage staff named " << in_storage_staff_name << "!" << endl;
	}
}
void GOODS_SETS :: FindGoodsByInStorageTime()
{
	int time_span = 0;
	bool find_flag = false;
	cout << "Please input the time(minute) :" << endl;
	cin >> time_span;
	time_t current_time_sec = time(NULL);
	time_t start_time_sec = 0;
	start_time_sec = current_time_sec - 60*time_span;
	vector<GOODS*> :: iterator it;
	it = this->begin();
	while(it != this->end())
	{
		time_t in_storage_time_sec = (*it)->GetGoodsInStorageTimeSec();
		if(in_storage_time_sec >= start_time_sec)
		{
			find_flag = true;
			(*it)->PrintGoodsInfo();
		}
		it ++;
	}
	if(find_flag == false)
	{
		cout << "No goods was put into this storage in last " << time_span << " minutes!" << endl;
	}
}


bool GOODS_SETS :: DeleteGoodsByTrackingNum(int target_num)
{
	bool delete_flag = false;
	vector<GOODS*> :: iterator it;
	it = this->begin();
	while(it != this->end())
	{
		if((*it)->GetGoodsTrackingNum() == target_num)
		{
			delete_flag = true;
			GOODS* tmp = *it;
			this->erase(it);
			delete tmp;//
			cout << " No." << target_num << " goods is discharged from this storage!" << endl;
			break;
		}
		it ++;
	}
	if(delete_flag == false)
	{
		cout << "No." << target_num << "goods failed to discharge from this storage!" << endl;
	}
	return delete_flag;
}

void GOODS_SETS :: ReadAllGoodsInfoFromFile()
{
	ifstream input_file("goods.txt",ios ::in);
	while(!input_file.is_open())
	{
		input_file.open("goods.txt",ios ::in);
	}
	while(!input_file.eof())
	{
		GOODS* pGoods = new GOODS;
		pGoods->ReadSingleGoodsInfo(input_file);
		if(pGoods->GetGoodsTrackingNum() == 0)
		{
			delete pGoods;
			break;
		}
		else
		{
			this->push_back(pGoods);
		}
	}
	input_file.close();
}
