#ifndef BINDING_CARD_H
#define BINDING_CARD_H

#include<vector>

#include "Campus_card.h"
#include "Deposit_card.h"

class Binding_card final:
	public Campus_card, public Deposit_card
{
public:
	Binding_card()=default;
	~Binding_card()=default;

	friend std::ifstream& operator>>(std::ifstream &is, Binding_card &card);
	friend std::ofstream& operator<<(std::ofstream &os, Binding_card card);

	bool transfer_into(unsigned object, unsigned money) override;
	bool operation() override;

	//初始化绑定卡
	bool init(unsigned num);
	//进行绑定操作
	bool bind(unsigned num);

private:

	std::vector <unsigned> binded_card;		//存储被绑定储蓄卡的号码

	bool showMessage() const;
	bool pay(std::string place, unsigned money) override;
	bool writeAccount(std::string op, unsigned object_num, unsigned money, std::string place) override;
};

#endif // !BINDING_CARD_H