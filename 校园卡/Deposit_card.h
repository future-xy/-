#ifndef DEPOSIT_CARD_H
#define DEPOSIT_CARD_H

#include "Card.h"
class Deposit_card :
	virtual public Card
{
public:
	friend class Binding_card;

	Deposit_card() = default;
	virtual ~Deposit_card() = default;

	friend std::ifstream& operator>>(std::ifstream &is, Deposit_card &card);
	friend std::ofstream& operator<<(std::ofstream &os, Deposit_card card);

	//���п�������
	virtual bool open_account(unsigned num, std::string name, unsigned money, std::string passward);

	virtual bool transfer_into(unsigned object, unsigned money) override;
	virtual bool operation() override;

private:

	unsigned overdraft;		//͸֧���
	bool bind_flag;			//�󶨱�־

	//�ж��Ƿ񱻰�
	bool is_binded() const;
	//��ȡ͸֧���
	unsigned getOverdraft() const;

	//���
	bool deposit(unsigned money);
	//ȡ��
	bool withdraw(unsigned money);
	//ת��
	unsigned remit(unsigned object, unsigned money,std::string tyep="none");

	virtual bool pay(std::string place, unsigned money) override;
	virtual bool showMessage() const override;
	virtual bool writeAccount(std::string op, unsigned object_num, unsigned money, std::string place) override;
};

#endif // !DEPOSIT_CARD_H