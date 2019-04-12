#ifndef _CLASS_SYSTEM_H_
#define _CLASS_SYSTEM_H_
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "class.h"
#include "derived_class.h"
#include "class_template.h"
#include "class_goods.h"
#include <ctime>
extern int template_login_flag;
using namespace std;
//this class is used for system initialization && exit && member login 
class SYSTEM
{
	public:
		SYSTEM();
		//constructor
		//creat three kinds of staff_sets object,read info from file
		//creat super_user object,use the pointers to three kinds of staff_sets object initialization
		//creat relative menu object
		void SystemCheckLogin();
		//check login according to initialization info
		//after login,the user control the system
		void SystemExit();
		//delete space
		void SetCurrentInStaff(IN_STORAGE_STAFF*&);
		void SetCurrentOutStaff(OUT_STORAGE_STAFF*&);
		void SetCurrentGenaralStaff(GENERAL_STAFF*&);
		void SetCheckLoginFlag(int);
		void CheckLoginInStorageStaff();
		void CheckLoginOutStorageStaff();
		void CheckLoginGeneralStaff();
		void CheckLoginSuperUser();
		void SystemRun();
		void WriteSucceedLoginRecord(MEMBER*);
		void WriteFailedLoginRecord(MEMBER*);
		//void ViewLoginRecord();
	private:
		STAFF_SETS<IN_STORAGE_STAFF>* in_staff;
		STAFF_SETS<OUT_STORAGE_STAFF>* out_staff;
		STAFF_SETS<GENERAL_STAFF>*  general_staff;
		SUPER_USER* super_user;
		IN_STORAGE_STAFF* current_in_staff;
		OUT_STORAGE_STAFF* current_out_staff;
		GENERAL_STAFF* current_general_staff;
		int check_login_flag;
		GOODS_SETS* goods_sets;
		//add goods;
};
#endif
