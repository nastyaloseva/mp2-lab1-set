// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
    if (len < 1)
        throw len;
    BitLen = len;

    if (len < 32)
        MemLen = 1;
    else
        MemLen = ((len - 1) >> 5) + 1;
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
    {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n >> 5;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return 1 << (n & 31);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if ((n < 0) || (n >= BitLen))
        throw n;
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if ((n < 0) || (n >= BitLen))
        throw n;
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if ((n < 0) || (n >= BitLen))
        throw n;
    return pMem[GetMemIndex(n)] & GetMemMask(n);
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this == &bf)
        return *this;

    TELEM* newMem = new TELEM[bf.MemLen];

    for (int i = 0; i < bf.MemLen; i++)
        newMem[i] = bf.pMem[i];

    delete[] pMem;

    pMem = newMem;
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;

    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    int res = 1;

    if (BitLen != bf.BitLen)
        res = 0;
    else
    {
        for (int i = 0; i < MemLen; i++)
        {
            TELEM memMask = 0;

            if (i < MemLen - 1)
                memMask = 0xffffffff;
            else
                memMask = (1 << (BitLen % 32)) - 1;

            if ((memMask & pMem[i]) != (memMask & bf.pMem[i]))
            {
                res = 0;
                break;
            }
        }
    }

    return res;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int len = BitLen;

    if (bf.BitLen > len)
        len = bf.BitLen;

    TBitField tmp(len);

    for (int i = 0; i < MemLen; i++)
        tmp.pMem[i] = pMem[i];

    for (int i = 0; i < bf.MemLen; i++)
        tmp.pMem[i] |= bf.pMem[i];

    return tmp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int len = BitLen;

    if (bf.BitLen > len)
        len = bf.BitLen;

    TBitField tmp(len);

    for (int i = 0; i < MemLen; i++)
        tmp.pMem[i] = pMem[i];

    for (int i = 0; i < bf.MemLen; i++)
        tmp.pMem[i] &= bf.pMem[i];

    return tmp;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField tmp(BitLen);

    for (int i = 0; i < BitLen; i++)
    {
        if (!GetBit(i))
            tmp.SetBit(i);
    }
    return tmp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    int i = 0;
    char ch;

    do
    {
        istr >> ch;
    } while ((ch != '0') && (ch != '1'));

    while (1)
    {
        if (ch == '0')
            bf.ClrBit(i++);
        else if (ch == '1')
            bf.SetBit(i++);
        else
            break;

        if (i >= bf.BitLen)
            break;

        istr >> ch;
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    int len = bf.BitLen;

    for (int i = 0; i < len; i++)
    {
        if (bf.GetBit(i))
            ostr << '1';
        else
            ostr << '0';
    }
    return ostr;
}
