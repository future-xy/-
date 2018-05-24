//TEST

#include<iostream>
#include<fstream>
#include<string>
#include<map>
#include<set>
#include<cstdlib>

#include"Card.h"
#include"Campus_card.h"
#include"Deposit_card.h"
#include"Binding_card.h"

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::map;
using std::set;
using std::string;
using std::ifstream;
using std::ofstream;

int err_flag = 0;
map <unsigned, Campus_card> campus_card_list;
map <unsigned, Deposit_card> deposit_card_list;
map <unsigned, Binding_card> binding_card_list;
set <string> student;

int getOrder();
void showErr(int err_type);
bool read(unsigned &, unsigned &, map <unsigned, Campus_card>&, map <unsigned, Deposit_card>&,
	map<unsigned, Binding_card> &);
bool save(unsigned &, unsigned &, const map <unsigned, Campus_card>&, const map <unsigned, Deposit_card>&,
	const map<unsigned, Binding_card>&);

int main()
{

	unsigned campus_card_num = 170000;
	unsigned deposit_card_num = 170100;

	if (!read(campus_card_num, deposit_card_num, campus_card_list, deposit_card_list,binding_card_list))
	{
		cerr << "程序发生了严重错误，即将退出.\n";
		system("pause");

		exit(0);
	}
	while (1)
	{
		err_flag = 0;
		cout << "*欢迎使用校园卡管理系统！\n"
			<< "*请输入括号中的命令以执行相应操作！\n"
			<< "*警告：非正常退出可能导致卡片冻结！！！\n"
			<< "(0)退出系统\n"
			<< "(1)办理储蓄卡\n"
			<< "(2)办理校园卡\n"
			<< "(3)绑定储蓄卡\n"
			<< "(4)卡片业务\n";
		int order = getOrder();
		if (order == -1)
		{
			showErr(-1);
			system("pause");
			system("cls");
			continue;
		}
		if (order == 0)
		{
			if (save(campus_card_num, deposit_card_num, campus_card_list, deposit_card_list, binding_card_list))
			{
				cout << "Bye!\n";
				break;
			}
			else
			{
				cerr << "An error occurred. Please try again later.\n";
				continue;
			}
		}

		string name;
		string department;
		string passward;

		unsigned money;
		unsigned card_num = 0;
		unsigned student_num;
		unsigned deposit_num = 0;


		Campus_card newCampus_card;
		Deposit_card newDeposit_card;
		Binding_card newBinding_card;

		//Menu
		switch (order)
		{
		case 1:
			cout << "请输入开户人姓名：";
			cin >> name;
			cout << "\n请输入初始密码：";
			cin >> passward;
			cout << "\n请输入预存金额：";
			cin >> money;
			newDeposit_card.open_account(deposit_card_num,name,money,passward);
			deposit_card_list[deposit_card_num] = newDeposit_card;
			++deposit_card_num;
			cout << "\n开户成功。\n";
			cout << "你的储蓄卡号码是：" << newDeposit_card.getNum() << endl;
			break;
		case 2:
			cout << "请输入学生姓名：";
			cin >> name;
			if (student.count(name))
			{
				cout << "\n\n每个学生只能有一张校园卡！\n";
				break;
			}
			cout << "\n请输入初始密码：";
			cin >> passward;
			cout << "\n请输入学号：";
			cin >> student_num;
			cout << "\n请输入学院/系：";
			cin >> department;
			cout << "\n请输入预存金额：";
			cin >> money;
			newCampus_card.open_account(campus_card_num, name, student_num, department, money,passward);
			campus_card_list[campus_card_num] = newCampus_card;
			++campus_card_num;
			student.insert(name);
			cout << "\n办理成功。\n";
			cout << "你的校园卡号码是：" << newCampus_card.getNum() << endl;
			break;
		case 3:
			cout << "请输入校园卡号：";
			cin >> student_num;
			cin.clear();
			if (!campus_card_list.count(student_num))
			{
				cout << "\n此校园卡不存在，请先办理！\n";
				break;
			}
			newBinding_card.init(student_num);
			campus_card_list.erase(student_num);
			while (true)
			{
				cout << "\n输入-1结束";
				cout << "\n请输入要绑定的储蓄卡号码：";
				cin >> deposit_num;
				if (deposit_num == -1)
					break;
				if(deposit_card_list.count(deposit_num))
					newBinding_card.bind(deposit_num);
				else
					cout << "\n此储蓄卡不存在，请重新输入\n";
			}
			binding_card_list[student_num] = newBinding_card;
			cout << "\n绑定成功！\n";
			cout << "校园卡号不变，可以继续使用\n";
			break;
		case 4:
			while (true)
			{
				cout << "输入-1退出\n";
				cout << "请输入卡号：";
				cin >> card_num;
				if (card_num == -1)
					break;
				if (deposit_card_list.count(card_num))
					deposit_card_list[card_num].operation();
				else if (campus_card_list.count(card_num))
					campus_card_list[card_num].operation();
				else if (binding_card_list.count(card_num))
					binding_card_list[card_num].operation();
				else
					cout << "\n此卡片不存在，请重新输入\n";
			}
			break;
		default:
			cout << "\n非法指令\n";
			break;
		}
		cin.sync();
		cin.clear();
		system("pause");
		system("cls");
		fflush(stdin);
	}

	return 0;
}
bool read(unsigned &campus_card_num, unsigned &deposit_card_num, map <unsigned, Campus_card> &campus_card_list,
	map <unsigned, Deposit_card> &deposit_card_list, map<unsigned, Binding_card> &binding_card_list)
{
	ifstream myin("Bank.csv");
	if (!myin.is_open())
	{
		ofstream newfile("Bank.csv");
		myin.close();
		newfile.close();
		return true;
	}

	int n = 0;
	string temp;
	myin >> campus_card_num >> temp >> deposit_card_num;

	std::getline(myin, temp, ',');
	try { n = stoi(temp); }
	catch (...) { n = 0; }
	for (int j = 0; j < n; ++j)
	{
		Campus_card temp_cam;
		myin >> temp_cam;
		campus_card_list[temp_cam.getNum()] = temp_cam;
	}
	n = 0;
	std::getline(myin, temp, ',');
	try { n = stoi(temp); }
	catch (...) { n = 0; }
	for (int j = 0; j < n; ++j)
	{
		Deposit_card temp_dep;
		myin >> temp_dep;
		deposit_card_list[temp_dep.getNum()] = temp_dep;
	}
	n = 0;
	std::getline(myin, temp, ',');
	try { n = stoi(temp); }
	catch (...) { n = 0; }
	for (int j = 0; j < n; ++j)
	{
		Binding_card temp_bin;
		myin >> temp_bin;
		binding_card_list[temp_bin.getNum()] = temp_bin;
	}

	myin.close();
	return true;
}
bool save(unsigned &campus_card_num, unsigned &deposit_card_num, const map <unsigned, Campus_card> &campus_card_list, const map <unsigned, Deposit_card> &deposit_card_list,
	const map<unsigned, Binding_card> &binding_card_list)
{
	ofstream myout("BANK.csv");
	myout << campus_card_num << " , " << deposit_card_num << endl;
		myout << campus_card_list.size() << "," << endl;
		for (auto item = campus_card_list.begin(); item != campus_card_list.end(); ++item)
		{
			myout << item->second;
		}
		myout << deposit_card_list.size() << "," << endl;
		for (auto item = deposit_card_list.begin(); item != deposit_card_list.end(); ++item)
		{
			myout << item->second;
		}
		myout << binding_card_list.size() << "," << endl;
		for (auto item = binding_card_list.begin(); item != binding_card_list.end(); ++item)
		{
			myout << item->second;
		}
	myout.close();

	return true;
}
int getOrder()
{
	int Order = 0;
	cin >> Order;

	return Order;
}
void showErr(int err_type)
{
	if(err_type==-1)
		cout << "\n输入的命令非法，请检查输入\n";
}