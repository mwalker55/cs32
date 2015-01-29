#include "newMultiset.h"
using namespace std;
#include <iostream>
#include <stdlib.h>

Multiset::Multiset()
{
    set = new holder[DEFAULT_MAX_ITEMS];
    m_Number = DEFAULT_MAX_ITEMS;
    for (int i = 0; i < m_Number; i++)
    {
        set[i].count = 0;
    }
}

Multiset::Multiset(int n)
{
    if (n < 0)
        exit(0);
    set = new holder[n];
    m_Number = n;
    for (int i = 0; i < m_Number; i++)
    {
        set[i].count = 0;
    }
}

Multiset::Multiset(const Multiset& n)
{
    m_Number = n.m_Number;
    set = new holder[m_Number];
    for (int i = 0; i < m_Number; i++)
    {
        set[i] = n.set[i];
    }
}

Multiset& Multiset::operator=(const Multiset& n)
{
    if (&n == this)
    {
        return (*this);
    }
    delete [] set;
    m_Number = n.m_Number;
    set = new holder[m_Number];
    for (int i = 0; i < m_Number; i++)
    {
        set[i] = n.set[i];
    }

    return (*this);
}

Multiset::~Multiset()
{
    delete [] set;
}

bool Multiset::empty() const
{
    for (int i = 0; i < m_Number; i++)
    {
        if (set[i].count != 0)
            return false;
    }

    return true;
}

int Multiset::size() const
{
    int total = 0;
    for (int i = 0; i < m_Number; i++)
    {
        total+=set[i].count;
    }

    return total;
}

int Multiset::uniqueSize() const
{
    int total = 0;
    for (int i = 0; i < m_Number; i++)
    {
        if (set[i].count > 0)
            total++;
    }

    return total;
}

bool Multiset::insert(const ItemType& value)
{
    for (int i = 0; i < m_Number; i++)
    {
        if (set[i].yes == value)
        {
            set[i].count++;
            return true;
        }
        else if (set[i].count == 0)
        {
            set[i].yes = value;
            set[i].count++;
            return true;
        }
    }
    return false;
}

int Multiset::erase(const ItemType& value)
{
    for (int i = 0; i < m_Number; i++)
    {
        if (set[i].yes == value && set[i].count > 0)
        {
            set[i].count--;
            return 1;
        }
    }

    return 0;
}

int Multiset::eraseAll(const ItemType& value)
{
    for (int i = 0; i < m_Number; i++)
    {
        if (set[i].yes == value && set[i].count > 0)
        {
            int returnVal = set[i].count;
            set[i].count = 0;
            return returnVal;
        }
    }

    return 0;
}

bool Multiset::contains(const ItemType& value) const
{
    for (int i = 0; i < m_Number; i++)
    {
        if (set[i].yes == value && set[i].count > 0)
            return true;
    }

    return false;
}

int Multiset::count(const ItemType& value) const
{
    for (int i = 0; i < m_Number; i++)
    {
        if (set[i].yes == value)
        {
            return set[i].count;
        }
    }

    return 0;
}

int Multiset::get(int i, ItemType& value) const
{
    if (i < 0 || i >= uniqueSize())
        return 0;
    int j = 0;
    for (j = 0; j < m_Number && i >= 0; j++)
    {
        if (set[j].count > 0)
            i--;
    }
    value = set[j-1].yes;
    return set[j-1].count;
}

void Multiset::swap(Multiset& other)
{
    holder *swag = other.set;
    other.set = set;
    set = swag;
    int num1 = other.m_Number;
    other.m_Number = m_Number;
    m_Number = num1;
}
