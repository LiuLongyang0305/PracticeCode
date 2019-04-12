#ifndef _CLASS_GOODS_H_
#define _CLASS_GOODS_H_
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
using namespace std;

/******* class GOODS *******/
class GOODS
{
	public:
		GOODS(string name = "goods",int tracking_num = 0,string size = "size",string dest = "shanghai",string state = "norecord",string in_staff = "none",time_t in_time_sec = 0,string in_time_str = "norecord",string out_staff = "none",time_t out_time_sec = 0,string out_time_str = "norecord");
		void InputGoodsInfo();
		void WriteSingleGoodsInfo(ofstream&);
		void ReadSingleGoodsInfo(ifstream&);
		void PrintGoodsInfo();
		void WriteInStorageRecord();
		void WriteOutStorageRecord();

		string GetGoodsDestination();
		string GetGoodsState();
		string GetGoodsInStorageStaff();
		string GetGoodsOutStorageStaff();
		time_t GetGoodsInStorageTimeSec();
		time_t GetGoodsOutStorageTimeSec();
		string GetGoodsOutStorageTimeStr();
		string GetGoodsInStorageTimeStr();
		int GetGoodsTrackingNum();
		string GetGoodsName();
		string GetGoodsSize();
		string GetQRcodeContents();
		string GetQRcodeName();
		void CreatGoodsQRCode(string&);

		void SetGoodsInStorageTime();
		void SetGoodsOutStorageTime();
		void SetGoodsState(string);
		void SetGoodsOutStorageStaff(string&);
		void SetGoodsInStorageStaff(string&);
		void SetGoodsTrackingNum(int num);
	private:
		string name;
		int tracking_num;
		string size;
		string destination;
		string state;
		string in_storage_staff; 
		size_t in_storage_time_sec;
		//designed to used for super user to view
		string in_storage_time_str; 
		string out_storage_staff; 
		time_t out_storage_time_sec;
		//designed to used for super user to view
		string out_storage_time_str; 
};
/*********** class GOODS _SETS ***************/
class GOODS_SETS:public vector<GOODS*>
{
	public:
		~GOODS_SETS();
		void PrintAllGoodsInfo();
		void WriteAllGoodsInfo();
		void AddGoods(GOODS*);//by in_storage_staff 
		int  GetGoodsMaxTrackingNum();
		GOODS* FindGoodsByTrackingNum(int tar_num);
		void FindGoodsByInStorageStaffName();
		void FindGoodsByInStorageTime();
		bool DeleteGoodsByTrackingNum(int);
		void ReadAllGoodsInfoFromFile();
};
#endif
