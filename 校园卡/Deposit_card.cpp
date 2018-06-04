#include<iostream>
#include<map>
#include<set>

#include"Campus_card.h"
#include"Binding_card.h"
#include "Deposit_card.h"

using std::string;
using std::map;
using std::cout;
using std::cin;
using std::endl;
using std::to_string;

extern int err_flag;
extern map <unsigned, Campus_card> campus_card_list;
extern map <unsigned, Deposit_card> deposit_card_list;
extern map <unsigned, Binding_card> binding_card_list;
extern int getOrder();

std::ifstream& operator>>(std::ifstream &is,Deposit_card& card)
{
	string temp;
	int num;
	is >> temp >> card.num;
	is >> temp >> card.name;
	is >> temp >> card.balance;
	is >> temp >> card.overdraft;
	is >> temp >> card.date;
	is >> temp >> card.bind_flag;
	is >> temp >> card.passward;

	is >> num;
	for (int i = 0; i < num; ++i)
	{
		is >> temp;
		card.account.push_back(temp);
	}

	return is;
}
std::ofstream& operator<<(std::ofstream &os, Deposit_card card)
{
	os << "#卡号：, " << card.num << endl;
	os << "#姓名：, " << card.name << endl;
	os << "#余额：, " << card.balance << endl;
	os << "#透支额度：, " << card.overdraft << endl;
	os << "#发卡日期：, " << card.date << endl;
	os << "#是否被绑定：, " << card.bind_flag << endl
		<< "#密码：, " << card.passward << endl;
	os << card.account.size() << endl;
	for (auto item = card.account.begin(); item != card.account.end(); ++item)
	{
		os << *item << endl;
	}

	return os;
}


bool Deposit_card::operation()
{
	string the_passward="000000";
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
				<< "(1).存款\n"
				<< "(2).取款\n"
				<< "(3).转账\n"
				<< "(4).支付\n"
				<< "(5).修改密码\n"
				<< "(6).查看卡片信息\n"
				<< "(7).查看流水记录\n";
			int order = getOrder();
			if (order == 0 || order == -1)
				return false;
			unsigned the_money;
			unsigned card_num;
			string place;
			string new_passward, the_new_passward;
			switch (order)
			{
			case 0:
				return true;
			case 1:
				cout << "\n请输入金额：";
				cin >> the_money;
				if (deposit(the_money))
					cout << "\n操作成功，" << the_money << "元已存入"
					<< "\n余额为：	" << getBalance()
					<< "\n透支额度为:" << getOverdraft() << endl;
				else
					cout << "\n存款失败\n";
				break;
			case 2:
				cout << "\n请输入金额：";
				cin >> the_money;
				if (withdraw(the_money))
					cout << "\n操作成功，" << the_money << "元已取出"
					<< "\n余额为：	" << getBalance()
					<< "\n透支额度为:" << getOverdraft() << endl;
				else
					cout << "\n取款失败\n";
				break;
			case 3:
				cout << "\n请输入要转帐的卡号：";
				cin >> card_num;
				if (campus_card_list.count(card_num) || deposit_card_list.count(card_num) || binding_card_list.count(card_num))
				{
					cout << "请输入转账金额：";
					cin >> the_money;
					if (remit(card_num, the_money) != -1)
						cout << "\n操作成功，" << the_money << "元已转出"
						<< "\n余额为：	" << getBalance()
						<< "\n透支额度为:" << getOverdraft() << endl;
					else
						cout << "\n转账失败\n";
				}
				else
					cout << "\n卡片不存在" << endl;
				break;
			case 4:
				cout << "\n请输入消费地点：";
				cin >> place;
				cout << "\n请输入消费金额";
				cin >> the_money;
				if (pay(place, the_money))
					cout << "\n支付成功\n";
				else
					cout << "\n支付失败\n";
				break;
			case 5:
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
			case 6:
				cout << "*****************************\n";
				cout << "\n卡片信息如下：\n";
				showMessage();
				cout << "*****************************\n";
				break;
			case 7:
				cout << "*****************************\n";
				cout << "\n账户流水记录如下：\n";
				showAccount();
				cout << "*****************************\n";
				break;
			default:
				cout << "\n指令错误，请重新输入\n"<<endl;
				break;
			}
		}
	}
	else
	{
		return false;
	}
	return true;
}
bool Deposit_card::open_account(unsigned num, string name, unsigned money,string passward)
{
	this->num = num;
	this->name = name;
	balance = money;
	overdraft = 1000;
	bind_flag = false;
	date = getTime();
	this->passward = passward;

	return true;
}
bool Deposit_card::transfer_into(unsigned object, unsigned money)
{
	overdraft += money;
	money = 0;
	if (overdraft - 1000 > 0)
	{
		money = overdraft - 1000;
		overdraft = 1000;
	}
	balance += money;

	writeAccount("转入", object, money, " ");
	return true;
}
bool Deposit_card::is_binded() const
{
	return bind_flag;
}
bool Deposit_card::showMessage() const
{
	cout << "#卡号：		" << num << endl;
	cout << "#姓名：		" << name << endl;
	cout << "#余额：		" << balance << endl;
	cout << "#透支额度：	" << overdraft << endl;
	cout << "#发卡日期：	" << date << endl;

	return true;
}
unsigned Deposit_card::getOverdraft() const
{
	return overdraft;
}
bool Deposit_card::deposit(unsigned money)
{
	overdraft += money;
	money = 0;
	if (overdraft > 1000)
	{
		money = overdraft - 1000;
		overdraft = 1000;
	}
	balance += money;

	writeAccount("存款", 0, money, " ");
	return true;
}
bool Deposit_card::withdraw(unsigned money)
{
	if (overdraft + balance < money)
		return false;
	if (balance < money)
	{
		overdraft -= (money - balance);
		balance = 0;
	}
	else
	{
		balance -= money;
	}
	writeAccount("取款", 0, money, " ");
	return true;
}
unsigned Deposit_card::remit(unsigned object, unsigned money,string type)
{
	if (type == "auto")
	{
		if (binding_card_list.count(object))
		{
			if (balance + overdraft < money)
			{
				unsigned temp1 = balance + overdraft;
				money -= (balance + overdraft);
				binding_card_list[object].Campus_card::transfer_into(num, balance + overdraft);
				balance = overdraft = 0;
				writeAccount("转出", object, temp1, " ");

				return money;
			}
			else
			{
				if (balance < money)
				{
					overdraft -= (money - balance);
					balance = 0;
				}
				else
				{
					balance -= money;
				}
				binding_card_list[object].transfer_into(num, money);

				writeAccount("转出", object, money, " ");
				return 0;
			}
		}
		else
			return -1;
	}
	if (deposit_card_list.count(object))
	{
		if (balance + overdraft < money)
			return -1;
		deposit_card_list[object].transfer_into(num, money);
		if (balance < money)
		{
			overdraft -= (money - balance);
			balance = 0;
		}
		else
		{
			balance -= money;
		}
		writeAccount("转出", object, money, " ");

		return 0;
	}
	if (campus_card_list.count(object))
	{
		if (balance + overdraft < money)
			return -1;
		campus_card_list[object].transfer_into(num, money);
		if (balance < money)
		{
			overdraft -= (money - balance);
			balance = 0;
		}
		else
		{
			balance -= money;
		}
		writeAccount("转出", object, money, " ");

		return 0;
	}
	if (binding_card_list.count(object))
	{
		if (balance + overdraft < money)
			return -1;
		binding_card_list[object].Campus_card::transfer_into(num, money);
		if (balance < money)
		{
			overdraft -= (money - balance);
			balance = 0;
		}
		else
		{
			balance -= money;
		}
		writeAccount("转出", object, money, " ");

		return 0;
	}
	return -1;
}
bool Deposit_card::writeAccount(string op, unsigned object_num, unsigned money,string place)
{
	string temp;
	string obj;
	if (object_num == 0)
		obj = "无";
	else
		obj = to_string(object_num);
	temp = getTime() + "," + op + "," + obj + "," + to_string(money) + "," + to_string(balance) + "," + to_string(overdraft);
	account.push_back(temp);
	return true;
}
bool Deposit_card::pay(string place, unsigned money)
{
	if (overdraft + balance < money)
		return false;
	if (balance < money)
	{
		overdraft -= (money - balance);
		balance = 0;
	}
	else
	{
		balance -= money;
	}
	writeAccount("支付", 0, money, place);
	return true;
}