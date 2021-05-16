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
	os << "#���ţ�, " << card.num << endl;
	os << "#������, " << card.name << endl;
	os << "#��, " << card.balance << endl;
	os << "#͸֧��ȣ�, " << card.overdraft << endl;
	os << "#�������ڣ�, " << card.date << endl;
	os << "#�Ƿ񱻰󶨣�, " << card.bind_flag << endl
		<< "#���룺, " << card.passward << endl;
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
		cout << "���������룺";
		cin >> the_passward;
		if (the_passward == passward)
		{
			cout << "\n������ȷ\n";
			break;
		}
		else
			cout << "\n�����������������\n"
			<< "����" << i << "�λ���\n";
		--i;
	}
	if (the_passward == passward)
	{
		while (1)
		{
			cout << "�����������е�ָ��\n"
				<< "(0).�˳�\n"
				<< "(1).���\n"
				<< "(2).ȡ��\n"
				<< "(3).ת��\n"
				<< "(4).֧��\n"
				<< "(5).�޸�����\n"
				<< "(6).�鿴��Ƭ��Ϣ\n"
				<< "(7).�鿴��ˮ��¼\n";
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
				cout << "\n�������";
				cin >> the_money;
				if (deposit(the_money))
					cout << "\n�����ɹ���" << the_money << "Ԫ�Ѵ���"
					<< "\n���Ϊ��	" << getBalance()
					<< "\n͸֧���Ϊ:" << getOverdraft() << endl;
				else
					cout << "\n���ʧ��\n";
				break;
			case 2:
				cout << "\n�������";
				cin >> the_money;
				if (withdraw(the_money))
					cout << "\n�����ɹ���" << the_money << "Ԫ��ȡ��"
					<< "\n���Ϊ��	" << getBalance()
					<< "\n͸֧���Ϊ:" << getOverdraft() << endl;
				else
					cout << "\nȡ��ʧ��\n";
				break;
			case 3:
				cout << "\n������Ҫת�ʵĿ��ţ�";
				cin >> card_num;
				if (campus_card_list.count(card_num) || deposit_card_list.count(card_num) || binding_card_list.count(card_num))
				{
					cout << "������ת�˽�";
					cin >> the_money;
					if (remit(card_num, the_money) != -1)
						cout << "\n�����ɹ���" << the_money << "Ԫ��ת��"
						<< "\n���Ϊ��	" << getBalance()
						<< "\n͸֧���Ϊ:" << getOverdraft() << endl;
					else
						cout << "\nת��ʧ��\n";
				}
				else
					cout << "\n��Ƭ������" << endl;
				break;
			case 4:
				cout << "\n���������ѵص㣺";
				cin >> place;
				cout << "\n���������ѽ��";
				cin >> the_money;
				if (pay(place, the_money))
					cout << "\n֧���ɹ�\n";
				else
					cout << "\n֧��ʧ��\n";
				break;
			case 5:
				cout << "\n������ԭ���룺";
				cin >> new_passward;
				if (new_passward != passward)
				{
					cout << "\n�������" << endl;
					break;
				}
				cout << "\n�����������룺";
				cin >> new_passward;
				cout << "\n��ȷ�������룺";
				cin >> the_new_passward;
				if (new_passward == the_new_passward)
				{
					passward = new_passward;
					cout << "\n�޸�����ɹ�\n";
				}
				else
				{
					cout << "\n���벻һ��\n";
				}
				break;
			case 6:
				cout << "*****************************\n";
				cout << "\n��Ƭ��Ϣ���£�\n";
				showMessage();
				cout << "*****************************\n";
				break;
			case 7:
				cout << "*****************************\n";
				cout << "\n�˻���ˮ��¼���£�\n";
				showAccount();
				cout << "*****************************\n";
				break;
			default:
				cout << "\nָ���������������\n"<<endl;
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

	writeAccount("ת��", object, money, " ");
	return true;
}
bool Deposit_card::is_binded() const
{
	return bind_flag;
}
bool Deposit_card::showMessage() const
{
	cout << "#���ţ�		" << num << endl;
	cout << "#������		" << name << endl;
	cout << "#��		" << balance << endl;
	cout << "#͸֧��ȣ�	" << overdraft << endl;
	cout << "#�������ڣ�	" << date << endl;

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

	writeAccount("���", 0, money, " ");
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
	writeAccount("ȡ��", 0, money, " ");
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
				writeAccount("ת��", object, temp1, " ");

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

				writeAccount("ת��", object, money, " ");
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
		writeAccount("ת��", object, money, " ");

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
		writeAccount("ת��", object, money, " ");

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
		writeAccount("ת��", object, money, " ");

		return 0;
	}
	return -1;
}
bool Deposit_card::writeAccount(string op, unsigned object_num, unsigned money,string place)
{
	string temp;
	string obj;
	if (object_num == 0)
		obj = "��";
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
	writeAccount("֧��", 0, money, place);
	return true;
}