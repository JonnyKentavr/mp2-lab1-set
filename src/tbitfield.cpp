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
	if (BitLen < 0) throw 001;
	MemLen = BitLen / (sizeof(TELEM) * 8) + 1;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
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
	return (n / (sizeof(TELEM) * 8));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	int Pos = n % (sizeof(TELEM) * 8);
	TELEM tmp = 1;
	tmp <<= Pos;
	return tmp;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n < 0) throw 001;
	if (n >= BitLen) throw 002;
	int Index = GetMemIndex(n);
	TELEM Mask = GetMemMask(n);
	pMem[Index] |= Mask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n < 0) throw 001;
	if (n >= BitLen) throw 002;
	int Index = GetMemIndex(n);
	TELEM Mask = ~GetMemMask(n);
	pMem[Index] &= Mask;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n < 0) throw 001;
	if (n >= BitLen) throw 002;
	TELEM tmp = GetMemMask(n);
	if ((pMem[GetMemIndex(n)] & tmp) == 0) return 0; else return 1;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (BitLen != bf.BitLen)
	{
		delete[]pMem;
		BitLen = bf.BitLen;
		MemLen = bf.MemLen;
		pMem = new TELEM[MemLen];
	}
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
	return(*this);
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if ((MemLen != bf.MemLen) || (BitLen != bf.BitLen)) return 0;
	else
	{
		int f = 1;
		for (int i = 0; i < MemLen; i++)
		{
			if (pMem[i] != bf.pMem[i]) f = 0;
			break;
		}
		return f;
	}
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	if ((MemLen != bf.MemLen) || (BitLen != bf.BitLen)) return 1;
	else
	{
		int f = 0;
		for (int i = 0; i < MemLen; i++)
		{
			if (pMem[i] != bf.pMem[i]) f = 1;
			break;
		}
		return f;
	}
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
		if (GetBit(i) == 1) ClrBit(i); else SetBit(i);
	return(*this);
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	char *tmp;
	tmp = new char[1024];
	istr >> tmp;
	int len = 0;
	while (tmp[len] != ']')
	{
		len++; // 48 - 0 ; 49 - 1
		tmp[len - 1] = tmp[len];
	}
	len--;
	TBitField tmp_Bit(len);
	for (int i = 0; i < len; i++)
		if ((tmp[i] - 48) == 1) tmp_Bit.SetBit(i);
	bf = tmp_Bit;
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
