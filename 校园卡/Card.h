#ifndef CARD_H
#define CARD_H

#include<fstream>
#include<string>
#include<vector>
#include<time.h>

class Card
{
public:
	Card()=default;
	virtual ~Card()=default;
	
	//获取卡片号码
	unsigned getNum() const;

	//将卡片操作封装在operation()函数中
	virtual bool operation() = 0;
	
	//公有的转入函数，方便其它卡片转入资金
	virtual bool transfer_into(unsigned object, unsigned money) = 0;
protected:

	unsigned num;							//卡号
	std::string passward;					//密码
	unsigned balance;						//余额
	std::string name;						//姓名
	std::string date;						//开户日期
	std::vector <std::string> account;		//账目记录
	
	unsigned getBalance() const;			//获取余额信息
	std::string getName() const;			//获取姓名信息
	std::string getDate() const;			//获取开户日期
	bool showAccount() const;				//显示流水记录
	std::string getTime();					//得到当前时间

	//支付功能
	virtual bool pay(std::string place, unsigned money);
	//显示卡片信息
	virtual bool showMessage() const;
	//记账函数
	virtual bool writeAccount(std::string op, unsigned object_num, unsigned money,std::string place);

};

#endif // !CARD_H