// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
// Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
// Переработано в соответсвии с C++11 (частично) Пановым А.А. 2021
// Битовое поле

#include <limits>
#include <iostream>
#include "tbitfield.h"


TBitField::TBitField(size_t len)
{
    if (len < 0)
    {
        throw len;
    }
    if (len == 0)
    {
        bitLen = len;
        memLen = len;
        pMem = nullptr;
    }
    else
    {
        bitLen = len;
        if (len % (sizeof(elType) * 8) != 0)
            memLen = len / (sizeof(elType) * 8) + 1;
        else
            memLen = len / (sizeof(elType) * 8);
        pMem = new elType[memLen];
        for (int i = 0; i < memLen; i++) pMem[i] = 0;

    }
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    bitLen = bf.bitLen;
    memLen = bf.memLen;
    pMem = new elType[memLen];
    for (int i = 0; i < memLen; i++) {
        pMem[i] = bf.pMem[i];
    }
}

size_t TBitField::getIndex(const size_t n) const  // индекс в pМем для бита n
{
    return (n / (sizeof(elType)*8));
}

elType TBitField::getMask(const size_t n) const // битовая маска для бита n
{
    return elType(1) << (n % (sizeof(elType)*8));
}

// доступ к битам битового поля
size_t TBitField::getLength() const // получить длину (к-во битов)
{
    return bitLen;
}

size_t TBitField::getNumBytes() const // получить количество байт выделенной памяти
{
    return memLen * sizeof(elType);
}

void TBitField::setBit(const size_t n) // установить бит
{
    if (n >= 0 && n < bitLen) {
        elType x = getMask(n);
        size_t i = getIndex(n);
        pMem[i] |= x;
    }
    else throw 1;
}

void TBitField::clrBit(const size_t n) // очистить бит
{
    if (n >= 0 && n < bitLen) {
        elType x = getMask(n);
        size_t i = getIndex(n);
        pMem[i] &= (~x);
    }
    else throw 1;
}

bool TBitField::getBit(const size_t n) const // получить значение бита
{
    if (n >= 0 && n < bitLen) {
        elType x = getMask(n);
        size_t i = getIndex(n);
        bool bit = pMem[i] & x;
        return bit;
    }
    else throw 1;

}

// битовые операции
TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this != &bf)
    {
        if (bf.bitLen != this->bitLen) {
            delete[] pMem;
            bitLen = bf.bitLen;
            memLen = bf.memLen;
            pMem = new elType[memLen];
        }
        for (int i = 0; i < memLen; i++) {
            pMem[i] = bf.pMem[i];
        }
        for (int i = (memLen - 1) * sizeof(elType); i < bitLen; i++) {
            if (bf.getBit(i))
                setBit(i);
        }
    }
    return *this;
}

bool TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (this != &bf)
    {
        if (bf.bitLen != this->bitLen)
            return false;

        for (int i = 0; i < memLen; i++) {
            if (pMem[i] != bf.pMem[i])
                return false;

        }
    }
    return true;
}

bool TBitField::operator!=(const TBitField &bf) const // сравнение
{
    if (this != &bf) {
        if (bf.bitLen != this->bitLen)
            return true;
        for (int i = 0; i < memLen; i++) {
            if (pMem[i] != bf.pMem[i])
                return true;
        }
    }
    return false;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    if (this == &bf)
        return *this;
    size_t max_len = std::max(bf.bitLen, bitLen);
    size_t max_size = std::max(bf.memLen, memLen);
    TBitField tbitfield(max_len);
    if (max_len == bitLen) {
        for (int i = 0; i < max_size; i++) {
            tbitfield.pMem[i] = bf.pMem[i];
            tbitfield.pMem[i] |= pMem[i];
        }
    }
    else {
        for (int i = 0; i < max_size; i++) {
            tbitfield.pMem[i] = pMem[i];
            tbitfield.pMem[i] |= bf.pMem[i];
        }
    }
    return tbitfield;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    if (this == &bf)
        return *this;
    size_t max_len = std::max(bf.bitLen, bitLen);
    size_t max_size = std::max(bf.memLen, memLen);
    TBitField tbitfield(max_len);
    if (max_len == bitLen) {
        for (int i = 0; i < max_size; i++) {
            tbitfield.pMem[i] = bf.pMem[i];
            tbitfield.pMem[i] &= pMem[i];
        }
    }
    else {
        for (int i = 0; i < max_size; i++) {
            tbitfield.pMem[i] = pMem[i];
            tbitfield.pMem[i] &= bf.pMem[i];
        }
    }
    return tbitfield;
}

TBitField TBitField::operator~() // отрицание
{
    TBitField field(bitLen);
    for (int i = 0; i < bitLen; i++) {
        if (getBit(i) == false)
            field.setBit(i);
    }
    return field;
}

TBitField::~TBitField()
{
    delete[] pMem;
    bitLen = 0;
    memLen = 0;
}

// ввод/вывод
std::istream &operator>>(std::istream &istr, TBitField &bf) // ввод
{
    for (int i = 0; i < bf.memLen; ++i)
        istr >> bf.pMem[i];
    return istr;
}

std::ostream &operator<<(std::ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.bitLen; i++){
        if (bf.getBit(i))
            ostr << '1';
        else
            ostr << '0';
    }
    return ostr;
}
