#include "pojemnik.h"

pojemnik::pojemnik()
{
    //ctor
}

pojemnik::pojemnik(float tablica[36])
{
    for(int i = 0; i!=36; i++)
    {
        frequencies[i] = tablica[i];
    }
}

pojemnik::~pojemnik()
{
    //dtor
}
