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
	os << "#���ţ�, " << card.num << endl
		<< "#������, " << card.name << endl
		<< "#ѧ�ţ�, " << card.student_num << endl
		<< "#Ժϵ��, " << card.department << endl
		<< "#��, " << card.balance << endl
		<< "#�������ڣ�, " << card.date << endl
		<< "#���룺, " << card.passward << endl;
	os << "#�󶨿��ţ�, " << card.binded_card.size() << endl;
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
				Campus_card::showMessage();
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
	writeAccount("ת��", object, money, "");

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
		writeAccount("����", 0, money, place);

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
		writeAccount("����", 0, money, place);
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
	temp = getTime() + "," + op + "," + obj + ","  + to_string(money) + "," + to_string(balance)+','+"��";
	account.push_back(temp);

	return true;
}
bool Binding_card::showMessage() const
{
	cout << "#���ţ�		" << num << endl;
	cout << "#������		" << name << endl;
	cout << "#ѧ�ţ�		" << student_num << endl;
	cout << "#Ժϵ��		" << department << endl;
	cout << "#��		" << balance << endl;
	cout << "#�������ڣ�	" << date << endl;

	return true;
}