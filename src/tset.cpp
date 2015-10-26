// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

TSet::TSet(int mp) : BitField(mp), MaxPower(mp)
{
}

// конструктор копирования
TSet::TSet(const TSet &s) : BitField(s.BitField), MaxPower(s.MaxPower)
{
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : BitField(bf), MaxPower(bf.GetLength())
{
}

TSet::operator TBitField()
{
	return BitField;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
	return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
	return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
	BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
	BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
	MaxPower = s.MaxPower;
	BitField = s.BitField;
	return (*this);
}

int TSet::operator==(const TSet &s) const // сравнение
{
	if ((MaxPower == s.MaxPower) && (BitField == s.BitField))
		return 1;
	else
		return 0;
}

int TSet::operator!=(const TSet &s) const // сравнение
{
	return!((*this) == s);
}

TSet TSet::operator+(const TSet &s) // объединение
{
	TSet tmp(s.MaxPower);
	tmp.BitField = BitField | s.BitField;
	return tmp;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
	TSet tmp((*this).MaxPower);
	tmp.BitField.SetBit(Elem);
	return tmp;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
	TSet tmp((*this).MaxPower);
	tmp.BitField.ClrBit(Elem);
	return tmp;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
	TSet tmp(s.MaxPower);
	tmp.BitField = BitField & s.BitField;
	return tmp;
}

TSet TSet::operator~(void) // дополнение
{
	~BitField;
	return(*this);
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
	int tmp;
	char ch;
	do
	{
		istr >> ch;
	} while (ch != '{');
	do
	{
		istr >> tmp;
		s.InsElem(tmp);
		do
		{
			istr >> ch;
		} 
		while ((ch != ',') && (ch != '}'));
	}
	while (ch != '}');
	return istr;
}


ostream &operator<<(ostream &ostr, const TSet &bf) // вывод
{
	int i, n;
	char ch = ' ';
	ostr << '{';
	n = bf.GetMaxPower();
	for (i = 0; i < n; i++)
	{
		if (bf.IsMember(i))
		{
			ostr << ch << ' ' << i;
			ch = ',';
		}
	}
	ostr << '}';
	return ostr;
}
