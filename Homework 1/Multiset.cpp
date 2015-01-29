#include "Multiset.h"

Multiset::Multiset()
{
    for(int i = 0; i < DEFAULT_MAX_ITEMS; i++)
    {
        set[i].count = 0;
    }
}

bool Multiset::empty() const
{
    for (int i = 0; i < DEFAULT_MAX_ITEMS; i++)
    {
        if (set[i].count != 0)
            return false;
    }

    return true;
}

int Multiset::size() const
{
    int total = 0;
    for (int i = 0; i < DEFAULT_MAX_ITEMS; i++)
    {
        total+=set[i].count;
    }

    return total;
}

int Multiset::uniqueSize() const
{
    int total = 0;
    for (int i = 0; i < DEFAULT_MAX_ITEMS; i++)
    {
        if (set[i].count > 0)
            total++;
    }

    return total;
}

bool Multiset::insert(const ItemType& value)
{
    for (int i = 0; i < DEFAULT_MAX_ITEMS; i++)
    {
        if (set[i].yes == value)
        {
            set[i].count++;
            return true;
        }
        if (set[i].count == 0)
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
    for (int i = 0; i < DEFAULT_MAX_ITEMS; i++)
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
    for (int i = 0; i < DEFAULT_MAX_ITEMS; i++)
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
    for (int i = 0; i < DEFAULT_MAX_ITEMS; i++)
    {
        if (set[i].yes == value && set[i].count > 0)
            return true;
    }

    return false;
}

int Multiset::count(const ItemType& value) const
{
    for (int i = 0; i < DEFAULT_MAX_ITEMS; i++)
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
    for (j = 0; j < DEFAULT_MAX_ITEMS && i >= 0; j++)
    {
        if (set[j].count > 0)
            i--;
    }
    value = set[j-1].yes;
    return set[j-1].count;
}

void Multiset::swap(Multiset& other)
{
    for (int i = 0; i < DEFAULT_MAX_ITEMS; i++)
    {
        holder temp = other.set[i];
        other.set[i] = set[i];
        set[i] = temp;
    }
}
