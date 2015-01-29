#include "StudentMultiset.h"
#include <iostream>
using namespace std;

StudentMultiset::StudentMultiset()
{}

bool StudentMultiset::add(unsigned long id)
{
    return students.insert(id);
}

int StudentMultiset::size() const
{
    return students.size();
}

void StudentMultiset::print() const
{
    for(int i = 0; i < students.uniqueSize(); i++)
    {
        unsigned long temp = 0;
        int z = students.get(i, temp);
        for (int j = 0; j < z; j++)
        {
            cout << temp << endl;
        }
    }
}
