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
	
	//��ȡ��Ƭ����
	unsigned getNum() const;

	//����Ƭ������װ��operation()������
	virtual bool operation() = 0;
	
	//���е�ת�뺯��������������Ƭת���ʽ�
	virtual bool transfer_into(unsigned object, unsigned money) = 0;
protected:

	unsigned num;							//����
	std::string passward;					//����
	unsigned balance;						//���
	std::string name;						//����
	std::string date;						//��������
	std::vector <std::string> account;		//��Ŀ��¼
	
	unsigned getBalance() const;			//��ȡ�����Ϣ
	std::string getName() const;			//��ȡ������Ϣ
	std::string getDate() const;			//��ȡ��������
	bool showAccount() const;				//��ʾ��ˮ��¼
	std::string getTime();					//�õ���ǰʱ��

	//֧������
	virtual bool pay(std::string place, unsigned money);
	//��ʾ��Ƭ��Ϣ
	virtual bool showMessage() const;
	//���˺���
	virtual bool writeAccount(std::string op, unsigned object_num, unsigned money,std::string place);

};

#endif // !CARD_H