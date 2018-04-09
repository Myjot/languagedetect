#include "pojemnik.h"

pojemnik::pojemnik()
{
    //ctor
}

pojemnik::pojemnik(float tablica[26])
{
    for(int i = 0; i!=26; i++)
    {
        frequencies[i] = tablica[i];
    }
}

pojemnik::~pojemnik()
{
    //dtor
}
