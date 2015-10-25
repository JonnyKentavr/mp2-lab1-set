// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	BitLen = len;
	MemLen = BitLen / (sizeof(TELEM) * 8) + 1;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	delete[]pMem;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i]; 
}

TBitField::~TBitField()
{
	delete[]pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return(n / (sizeof(TELEM) * 8));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	TELEM tmp = 1;
	tmp = tmp << n;
	return tmp;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] || GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] && ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (pMem[GetMemIndex(n)] && GetMemMask(n))
		return 1;
	else
		return 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (BitLen != bf.BitLen)
	{
		BitLen = bf.BitLen;
		MemLen = bf.MemLen;
		delete[]pMem;
		pMem = new TELEM(MemLen);
	}
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
	return (*this);
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (BitLen != bf.BitLen)
		return 0;
	int flag = 1;	
	for (int i = 0; i < MemLen; i++)
		if (pMem[i] != bf.pMem[i])
		{
			flag = 0;
			break;
		}
	return flag;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	return!((*this) == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	if (BitLen <= bf.BitLen)
	{
		TBitField tmp(bf);
		for (int i = 0; i < MemLen; i++)
			tmp.pMem[i] |= pMem[i];
		return(tmp);
	}
	else
	{
		TBitField tmp(*this);
		for (int i = 0; i < bf.MemLen; i++)
			tmp.pMem[i] |= bf.pMem[i];
		return(tmp);
	}
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	if (BitLen <= bf.BitLen)
	{
		TBitField tmp(bf), tmp2(bf.BitLen);
		tmp2 = tmp2 | (*this);
		for (int i = 0; i < bf.MemLen; i++)
			tmp.pMem[i] &= tmp2.pMem[i];
		return(tmp);
	}
	else
	{
		TBitField tmp(*this), tmp2((*this).BitLen);
		for (int i = 0; i < MemLen; i++)
			tmp.pMem[i] &= tmp2.pMem[i];
		return(tmp);
	}
}

TBitField TBitField::operator~(void) // отрицание
{
	for (int i = 0; i < BitLen; i++)
		if (GetBit(i) == 1)
			ClrBit(i);
		else
			SetBit(i);
	return (*this); 
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	int i = 0;
	char ch;
	do
	{
		istr >> ch;
	}
	while (ch != ' ');
	while (1)
	{
		istr >> ch;
		if (ch == '0')
			bf.ClrBit(i++);
		else if (ch == '1')	bf.SetBit(i++);
		else break;
	}
	return istr;
}


ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	int len = bf.GetLength();
	for (int i = 0; i < len; i++)
		if (bf.GetBit(i)) ostr << '1';
		else ostr << '0';
		return ostr;
}
