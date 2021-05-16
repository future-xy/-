#include<iostream>
#include<map>

#include "Binding_card.h"

using std::cin;
using std::cout;
using std::endl;
using std::map;
using std::string;
using std::to_string;

extern int err_flag;
extern map <unsigned, Campus_card> campus_card_list;
extern map <unsigned, Deposit_card> deposit_card_list;
extern map <unsigned, Binding_card> binding_card_list;
extern int getOrder();

std::ifstream& operator>>(std::ifstream &is, Binding_card& card)
{
	string temp;
	int num;
	is >> temp >> card.num;
	is >> temp >> card.name;
	is >> temp >> card.student_num;
	is >> temp >> card.department;
	is >> temp >> card.balance;
	is >> temp >> card.date;
	is >> temp >> card.passward;
	is >> temp >> num;

	for (int i = 0; i < num; ++i)
	{
		unsigned tempnum;
		is >> tempnum;
		card.binded_card.push_back(tempnum);
	}
	is >> num;
	for (int i = 0; i < num; ++i)
	{
		is >> temp;
		card.account.push_back(temp);
	}

	return is;
}
std::ofstream& operator<<(std::ofstream &os, Binding_card card)
{
	os << "#卡号：, " << card.num << endl
		<< "#姓名：, " << card.name << endl
		<< "#学号：, " << card.student_num << endl
		<< "#院系：, " << card.department << endl
		<< "#余额：, " << card.balance << endl
		<< "#发卡日期：, " << card.date << endl
		<< "#密码：, " << card.passward << endl;
	os << "#绑定卡号：, " << card.binded_card.size() << endl;
	for (auto item = card.binded_card.begin(); item != card.binded_card.end(); ++item)
		os << (*item) << endl;
	os << card.account.size() << endl;
	for (auto item = card.account.begin(); item != card.account.end(); ++item)
	{
		os << (*item) << endl;
	}

	return os;
}

bool Binding_card::operation()
{
	string the_passward = "000000";
	int i = 3;
	while (i > 0)
	{

		cout << "请输入密码：";
		cin >> the_passward;
		if (the_passward == passward)
		{
			cout << "\n密码正确\n";
			break;
		}
		else
			cout << "\n密码错误，请重新输入\n"
			<< "还有" << i << "次机会\n";
		--i;
	}
	if (the_passward == passward)
	{
		while (1)
		{
			cout << "请输入括号中的指令\n"
				<< "(0).退出\n"
				<< "(1).支付\n"
				<< "(2).修改密码\n"
				<< "(3).查看卡片信息\n"
				<< "(4).查看流水记录\n";
			int order = getOrder();
			if (order == -1 || order == 0)
				return true;
			unsigned the_money;
			string the_place;
			string new_passward, the_new_passward;
			switch (order)
			{
			case 0:
				return true;
			case 1:
				cout << "\n请输入消费地点：";
				cin >> the_place;
				cout << "\n请输入消费金额：";
				cin >> the_money;
				if (pay(the_place, the_money))
					cout << "\n支付成功" << endl;
				else
					cout << "\n支付失败" << endl;
				break;
			case 2:
				cout << "\n请输入原密码：";
				cin >> new_passward;
				if (new_passward != passward)
				{
					cout << "\n密码错误" << endl;
					break;
				}
				cout << "\n请输入新密码：";
				cin >> new_passward;
				cout << "\n请确认新密码：";
				cin >> the_new_passward;
				if (new_passward == the_new_passward)
				{
					passward = new_passward;
					cout << "\n修改密码成功\n";
				}
				else
				{
					cout << "\n密码不一致\n";
				}
				break;
			case 3:
				cout << "\n卡片信息如下：\n";
				Campus_card::showMessage();
				break;
			case 4:
				cout << "\n账户流水记录如下：\n";
				showAccount();
				break;
			default:
				cout << "\n指令错误，请重新输入\n";
				break;
			}
		}
	}
	else
	{
		return false;
	}
}
bool Binding_card::init(unsigned num)
{
	Campus_card temp = campus_card_list[num];
	this->date = temp.date;
	this->num = temp.num;
	this->name = temp.name;
	this->student_num = temp.student_num;
	this->department = temp.department;
	this->balance = temp.balance;
	this->passward = temp.passward;

	return true;
}
bool Binding_card::transfer_into(unsigned object, unsigned money)
{
	balance += money;
	writeAccount("转入", object, money, "");

	return true;
}
bool Binding_card::bind(unsigned num)
{
	deposit_card_list[num].bind_flag = true;
	binded_card.push_back(num);

	return true;
}
bool Binding_card::pay(string place, unsigned money)
{
	if (balance >= money)
	{
		balance -= money;
		writeAccount("消费", 0, money, place);

		return true;
	}
	int count = money-balance;
	int sum = 0;
	for (auto item = binded_card.begin(); item != binded_card.end(); ++item)
	{
		sum += deposit_card_list[*item].balance + deposit_card_list[*item].overdraft;
	}
	if (sum >= count)
	{
		for (auto item = binded_card.begin(); item != binded_card.end(); ++item)
		{
			count = deposit_card_list[*item].remit(num, count, "auto");
			if (count == 0)
				break;
		}

		balance = 0;
		writeAccount("消费", 0, money, place);
		return true;
	}
	else
		return false;
}
bool Binding_card::writeAccount(string op, unsigned object_num, unsigned money, string place)
{
	string temp;
	string obj;
	if (object_num == 0)
		obj = place;
	else
		obj = to_string(object_num);
	temp = getTime() + "," + op + "," + obj + ","  + to_string(money) + "," + to_string(balance)+','+"无";
	account.push_back(temp);

	return true;
}
bool Binding_card::showMessage() const
{
	cout << "#卡号：		" << num << endl;
	cout << "#姓名：		" << name << endl;
	cout << "#学号：		" << student_num << endl;
	cout << "#院系：		" << department << endl;
	cout << "#余额：		" << balance << endl;
	cout << "#发卡日期：	" << date << endl;

	return true;
}