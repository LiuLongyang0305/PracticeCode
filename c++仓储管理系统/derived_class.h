#ifndef _DERIVED_CLASS_H_
#define _DERIVED_CLASS_H_
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "class.h"
#include "class_template.h"
#include "class_menu.h"
#include "class_goods.h"
extern int max_member_number;
using namespace std;
//four classes were derived from class MEMBER
//the mamber data is the same
//member function is different
/********************************/
class IN_STORAGE_STAFF:public MEMBER
{
public:
	void OperationOnGoods();
    int ChooseOperation();
	//void GetOperationNumOnStaff(int);
	//void GetOperationNumOnSelf(int);
	//pure virtual function	
	//add goods in to storage,and creat QRcode
};
class OUT_STORAGE_STAFF:public MEMBER
{
public:
	void OperationOnGoods();
	int ChooseOperation();
	//pure virtual function
	//delete goods,delete QRcode
		
};
class GENERAL_STAFF:public MEMBER
{
public:
	void OperationOnGoods();
	int ChooseOperation();
	//view goods by tracking number
};
typedef void (STAFF_SETS<IN_STORAGE_STAFF>::*in_mem_fun_ptr)();
typedef void (STAFF_SETS<OUT_STORAGE_STAFF>::*out_mem_fun_ptr)();
typedef void (STAFF_SETS<GENERAL_STAFF>::*general_mem_fun_ptr)();
class SUPER_USER:public MEMBER
{
public:
	void SetSuperUser(STAFF_SETS<IN_STORAGE_STAFF>* pf1 = NULL,STAFF_SETS<OUT_STORAGE_STAFF>* pf2 = NULL,STAFF_SETS<GENERAL_STAFF>* pf3 = NULL);
	int ChooseOperation();
	void OperationOnGoods();
	void ChooseOperationOnStaffs(int);
	void ChooseOperationOnSelf();
	void ViewAllGoodsInfo();
	void ViewGoodsInfoByMember();
	void ViewGoodsInfoByTime();
	void GetMaxMemberNum();
	void ShowLastTimeSucceedLoginTime();
	void ViewLoginRecord();
	void ViewGoodsInStorageRecord();
	void ViewGoodsOutStorageRecord();
	//view goods info
	//add other function
private:
	STAFF_SETS<IN_STORAGE_STAFF>* in_staff_objec_pointer;
	STAFF_SETS<OUT_STORAGE_STAFF>* out_staff_objec_pointer;
	STAFF_SETS<GENERAL_STAFF>* general_staff_objec_pointer;
};
#endif
