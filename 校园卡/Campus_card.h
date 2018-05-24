#ifndef CAMPUS_CARD_H
#define CAMPUS_CARD_H

#include "Card.h"

class Campus_card :
	virtual public Card
{
public:
	friend class Binding_card;
	Campus_card() = default;
	virtual ~Campus_card() = default;

	friend std::ifstream& operator>>(std::ifstream &is, Campus_card& card);
	friend std::ofstream& operator<<(std::ofstream &os, Campus_card card);

	//���п�������
	virtual bool open_account(unsigned num, std::string name, unsigned student_num, 
		std::string department, unsigned money, std::string passward);

	virtual bool transfer_into(unsigned object, unsigned money) override;
	virtual bool operation() override;

protected:
	unsigned student_num;		//ѧ��
	std::string department;		//Ժϵ��Ϣ

	virtual bool pay(std::string place,unsigned money) override;
	virtual bool showMessage() const override;

	virtual bool writeAccount(std::string op, unsigned object_num, unsigned money, std::string place) override;
};

#endif // !CAMPUS_CARD_H