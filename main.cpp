#include <vector>
#include <fstream>
#include <iostream>
#include <ctype.h>
#include <sstream>
#include "fann.h"
#include "pojemnik.h"

void error(const char* p, const char* p2 = "")
{
     std::cerr << p << ' ' << p2 << std::endl;
}
bool generate_frequencies(const char* filename, float* frequencies)
{
    std::ifstream infile(filename);
    if(!infile)
    {
         error("Nie mozna otworzyc pliku", filename);
        return false;
    }
    else
    {
        std::vector<unsigned int> letter_count(36, 0);
        unsigned int num_characters = 0;
        char c;
        while(infile.get(c))
        {
            c = tolower(c);
            if(c >= 'a' && c <= 'z')
            {
                 letter_count[c - 'a']++;
                 num_characters++;
            }
            else
            {
                std::cout << c;
                if (c == '├ą')
                    std::cout << "X KUIRWA D";
            }
        }
        if(!infile.eof())
        {
            error("Blad 404");
            return false;
        }
        for(unsigned int i = 0; i != 36; i++)
        {
            frequencies[i] = letter_count[i]/(double)num_characters;
        }
        return true;
    }
}

int main(int argc, char* argv[])
{
    float frequencies[36];
    int changer;
    std::ostringstream ss;

    std::string strtmp, filename;
    int intjez;
    struct fann *ann;
    std::string txt = ".txt";
    std::cout << "Hello world!" << std::endl;
    std::vector<pojemnik> pudlo;
    do
    {

        std::cout << "----------------------------------" << std::endl;
        std::cout << "Podaj liczbe:" << std::endl;
        std::cout << "1. Tworz nowy plik do nauki jezyka." << std::endl;
        std::cout << "2. Stworz siec na nowo." << std::endl;
        std::cout << "3. Otworz gotowa siec z pliku." << std::endl;
        std::cout << "4. Wyjdz" << std::endl;
        std::cin >> changer;
        switch (changer)
        {
            case 1:
            {
                int j = 0;
                ss.str("");
                ss.clear();
                ss << j;
                std::cout << "Podaj nazwe: " << std::endl;
                std::cin >> strtmp;
                std::cout << "Podaj jezyk: " << std::endl;
                std::cout << "1.Angielski: " << std::endl;
                std::cout << "2.Finski: " << std::endl;
                std::cout << "3.Francuski: " << std::endl;
                std::cout << "4.Niemiecki: " << std::endl;
                std::cout << "5.Polski: " << std::endl;
                std::cout << "6.Hiszpanski: " << std::endl;
                std::cout << "7.Portugalski: " << std::endl;
                std::cout << "8.Wloski: " << std::endl;
                std::cout << "9.Holenderski: " << std::endl;
                std::cin >> intjez;
                filename = strtmp + ss.str() + txt;
                std::cout << std::endl;
                while(generate_frequencies(filename.c_str(), frequencies))
                {
                    std::cout<< filename << " otwarty" << std::endl;
                    j++;
                    ss.str("");
                    ss.clear();
                    ss << j;
                    filename = strtmp + ss.str() + txt;
                    pudlo.push_back(pojemnik(frequencies));
                }
                if ( j != 0)
                {
                    std::ofstream fileOut(strtmp + ".data");
                    fileOut << j << " 36 9" << std::endl << std::endl;
                    for(int k = 0; k<j; k++)
                    {
                        for(int i = 0; i!=36; i++)
                            fileOut << pudlo[k].frequencies[i] << " ";
                        fileOut << std::endl << std::endl;
                        for(int i = 0; i<9; i++)
                        {
                            if(i+1 != intjez)
                            {
                                fileOut << "0 ";
                            }
                            else
                            {
                                fileOut << "1 ";
                            }
                        }
                        fileOut << std::endl << std::endl;
                    }
                }
                break;
            }
            case 2:
            {
                ann = fann_create_standard(3, 36, 18, 9);
                break;
            }
            case 3:
            {
                char* nazwa_sieci;
                std::cout << "Podaj nazwe pliku z siecia" << std::endl;
                std::cin >> nazwa_sieci;
                ann = fann_create_from_file(nazwa_sieci);
                if(ann == 0)
                    changer = 0;
                break;
            }
            case 4:
            {
                return 0;
            }
        }
    } while(changer != 2 && changer != 3);
    if(changer == 2 || changer == 3)
    {
        do
        {
            std::cout << "-------------------------------" << std::endl;
            std::cout << "Podaj liczbe" << std::endl;
            std::cout << "1. Trenuj siec" << std::endl;
            std::cout << "2. Sprawdz tekst" << std::endl;
            std::cout << "3. Zapisz siec" << std::endl;
            std::cout << "4. Wyjdz" << std::endl;
            std::cin >> changer;
            switch(changer)
            {
                case 1:
                {
                    fann_train_on_file(ann, "train/input.data", 600, 10, 0.0001);
                    break;
                }
                case 2:
                {
                    std::string nazwa_pliku;
                    std::cout << "Podaj nazwe pliku z tekstem" << std::endl;
                    std::cin >> nazwa_pliku;
                    std::cout << nazwa_pliku;
                    if(generate_frequencies(nazwa_pliku.c_str(), frequencies))
                    {
                        for(int i = 0; i!=36; i++)
                        {
                            std::cout << frequencies[i] << " ";
                        }
                        std::cout << std::endl;

                        float *output = fann_run(ann, frequencies);
                        std::cout << "Angielski: " << output[0] << std::endl
                                  << "Finski : " << output[1] << std::endl
                                  << "Francuski : " << output[2] << std::endl
                                  << "Niemiecki : " << output[3] << std::endl
                                  << "Polski : " << output[4] << std::endl
                                  << "Hiszpanski : " << output[5] << std::endl
                                  << "Portugalski : " << output[6] << std::endl
                                  << "Wloski : " << output[7] << std::endl
                                  << "Holenderski : " << output[8] << std::endl;
                    }
                    break;
                }
                case 3:
                {
                    std::string nazwa_pliku;
                    std::cout << "Podaj nazwe by zapisac siec" << std::endl;
                    std::cin >> nazwa_pliku;
                    nazwa_pliku + ".data";
                    fann_save(ann, nazwa_pliku.c_str());
                    break;
                }

            }
        } while (changer != 4);
        std::cout << "Pamiec sieci zwolniona" << std::endl;
        fann_destroy(ann);
    }
    return 0;
}

/*if(generate_frequencies(argv[1], frequencies))
            {
                for(int i = 0; i!=26; i++)
                {
                    std::cout << frequencies[i] << " ";
                }
                std::cout << std::endl;

                //struct fann *ann = fann_create_standard(3, 26, 18, 9);
                struct fann *ann = fann_create_from_file("language_classify2.net");
                //fann_train_on_file(ann, "train/input.data", 300, 10, 0.0001);

                fann_save(ann, "language_classify2.net");
                //fann_print_parameters(ann);

                float *output = fann_run(ann, frequencies);
                std::cout << "Angielski: " << output[0] << std::endl
                          << "Finski : " << output[1] << std::endl
                          << "Francuski : " << output[2] << std::endl
                          << "Niemiecki : " << output[3] << std::endl
                          << "Polski : " << output[4] << std::endl
                          << "Hiszpanski : " << output[5] << std::endl
                          << "Portugalski : " << output[6] << std::endl
                          << "Wloski : " << output[7] << std::endl
                          << "Holenderski : " << output[8] << std::endl;
                fann_destroy(ann);
            }*/
