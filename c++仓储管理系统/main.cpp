#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "class.h"
#include "derived_class.h"
#include "class_template.h"
#include "class_menu.h"
#include "class_system.h"
#include "class_goods.h"
#include <typeinfo>
using namespace std;
int main()
{
	SYSTEM current_system;
	current_system.SystemRun();
	return 0;	
}
