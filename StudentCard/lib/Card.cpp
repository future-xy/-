#include<algorithm>
#include<iostream>

#include "Card.h"

#ifdef _WIN32
#define LOCALTIME(a, b) localtime_s(a, b)
#else
#define LOCALTIME(a, b) localtime_r(b, a)
#endif

using std::cout;
using std::endl;
using std::string;

unsigned Card::getNum() const
{
	return num;
}
unsigned Card::getBalance() const
{
	return balance;
}
string Card::getName() const
{
	return name;
}
string Card::getDate() const
{
	return date;
}
bool Card::writeAccount(std::string op, unsigned object_num, unsigned money, std::string place)
{
	return true;
}
string Card::getTime()
{
	char tmp[66];
	time_t t = time(0);
	struct tm now_time;
	LOCALTIME(&now_time, &t);
	// localtime_s(&now_time, &t);
	strftime(tmp, sizeof(tmp), "%Y/%m/%d/.%X.%A", &now_time);
	string temp(tmp);

	return temp;
}
bool Card::showAccount() const
{
	if (account.empty())
		cout << "û�м�¼!" << endl;
	else
	{
		cout << "ʱ��\t\t\t\t����\t����\t���\t���\t͸֧���\t\n";
		for (auto item = account.begin(); item != account.end(); ++item)
		{
			string temp = *item;
			std::replace(temp.begin(), temp.end(), ',', '\t');
			cout << temp << endl;
		}
	}
	cout << endl;


	return true;
}
bool Card::operation()
{
	return true;
}
bool Card::transfer_into(unsigned object, unsigned money)
{
	return true;
}
bool Card::pay(string place, unsigned money)
{
	return true;
}
bool Card::showMessage() const
{
	return true;
}