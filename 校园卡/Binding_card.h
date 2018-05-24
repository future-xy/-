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

	//��ʼ���󶨿�
	bool init(unsigned num);
	//���а󶨲���
	bool bind(unsigned num);

private:

	std::vector <unsigned> binded_card;		//�洢���󶨴���ĺ���

	bool showMessage() const;
	bool pay(std::string place, unsigned money) override;
	bool writeAccount(std::string op, unsigned object_num, unsigned money, std::string place) override;
};

#endif // !BINDING_CARD_H