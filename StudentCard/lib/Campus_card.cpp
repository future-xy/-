#include<iostream>
#include<map>

#include "Deposit_card.h"
#include "Binding_card.h"
#include "Campus_card.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::map;
using std::to_string;

extern int err_flag;
extern map <unsigned, Campus_card> campus_card_list;
extern map <unsigned, Deposit_card> deposit_card_list;
extern map <unsigned, Binding_card> binding_card_list;
extern int getOrder();

std::ifstream& operator>>(std::ifstream &is,Campus_card& card)
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
	is >> num;
	for (int i = 0; i < num; ++i)
	{
		is >> temp;
		card.account.push_back(temp);
	}

	return is;
}
std::ofstream& operator<<(std::ofstream &os, Campus_card card)
{
	os << "#���ţ�, " << card.num << endl
		<< "#������, " << card.name << endl
		<< "#ѧ�ţ�, " << card.student_num << endl
		<< "#Ժϵ��, " << card.department << endl
		<< "#��, " << card.balance << endl
		<< "#�������ڣ�, " << card.date << endl
		<< "#���룺, " << card.passward << endl;
	os << card.account.size() << endl;
	for (auto item = card.account.begin(); item != card.account.end(); ++item)
	{
		os << *item << endl;
	}

	return os;
}

bool Campus_card::open_account(unsigned num, string name, unsigned student_num, string department, unsigned money,string passward)
{
	this->date = getTime();
	this->num = num;
	this->name = name;
	this->student_num = student_num;
	this->department = department;
	this->balance = money;
	this->passward = passward;

	return true;
}
bool Campus_card::operation()
{
	string the_passward = "000000";
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
				<< "(1).֧��\n"
				<< "(2).�޸�����\n"
				<< "(3).�鿴��Ƭ��Ϣ\n"
				<< "(4).�鿴��ˮ��¼\n";
			int order = getOrder();
			unsigned the_money;
			string the_place;
			string new_passward, the_new_passward;
			switch (order)
			{
			case 0:
				return true;
			case 1:
				cout << "\n���������ѵص㣺";
				cin >> the_place;
				cout << "\n���������ѽ�";
				cin >> the_money;
				if (pay(the_place, the_money))
					cout << "\n֧���ɹ�" << endl;
				else
					cout << "\n֧��ʧ��" << endl;
				break;
			case 2:
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
			case 3:
				cout << "\n��Ƭ��Ϣ���£�\n";
				showMessage();
				break;
			case 4:
				cout << "\n�˻���ˮ��¼���£�\n";
				showAccount();
				break;
			default:
				cout << "\nָ���������������\n";
				break;
			}
		}
	}
	else
	{
		return false;
	}
}
bool Campus_card::pay(string place, unsigned money)
{
	if (balance < money)
		return false;
	else
		balance -= money;
	writeAccount("����", 0, money, place);

	return true;
}
bool Campus_card::transfer_into(unsigned object, unsigned money)
{
	balance += money;
	writeAccount("ת��", object, money, "");

	return true;
}
bool Campus_card::showMessage() const
{
	cout << "#���ţ�		" << num << endl;
	cout << "#������		" << name << endl;
	cout << "#ѧ�ţ�		" << student_num << endl;
	cout << "#Ժϵ��		" << department << endl;
	cout << "#��		" << balance << endl;
	cout << "#�������ڣ�	" << date << endl;

	return true;
}
bool Campus_card::writeAccount(string op, unsigned object_num, unsigned money, string place)
{
	string temp;
	string obj;
	if (object_num == 0)
		obj = place;
	else
		obj = to_string(object_num);
	temp = getTime() + "," + op + "," + obj +  "," + to_string(money) + "," + to_string(balance)+','+"��";
	account.push_back(temp);
	return true;
}