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
        std::vector<unsigned int> letter_count(26, 0);
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
        }
        if(!infile.eof())
        {
            error("Blad 404");
            return false;
        }
        for(unsigned int i = 0; i != 26; i++)
        {
            frequencies[i] = letter_count[i]/(double)num_characters;
        }
        return true;
    }
}

int main(int argc, char* argv[])
{
    float frequencies[26];
    int j = 0;
    std::ostringstream ss;
    ss << j;
    std::string strtmp, filename;
    int intjez;
    std::string txt = ".txt";
    std::cout << "Hello world!" << std::endl;
    std::vector<pojemnik> pudlo;
    if(argc != 2)
    {
        std::cout << "Podaj nazwe: " << std::endl;
        std::cin >> strtmp;
        std::cout << "Podaj jezyk: " << std::endl;
        std::cout << "1.Angielski: " << std::endl;
        std::cout << "2.Finski: " << std::endl;
        std::cout << "3.Francuski: " << std::endl;
        std::cout << "4.Niemiecki: " << std::endl;
        std::cout << "5.Polski: " << std::endl;
        std::cout << "6.Hiszpanski: " << std::endl;
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
            fileOut << j << " 26 6" << std::endl << std::endl;
            for(int k = 0; k<j; k++)
            {
                for(int i = 0; i!=26; i++)
                    fileOut << pudlo[k].frequencies[i] << " ";
                fileOut << std::endl << std::endl;
                for(int i = 0; i<6; i++)
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

    }
       // error("podaj jeden dodatkowy argument z nazwa pliku");
    else
    {
        if(generate_frequencies(argv[1], frequencies))
        {
            for(int i = 0; i!=26; i++)
            {
                std::cout << frequencies[i] << " ";
            }
            std::cout << std::endl;

            //struct fann *ann = fann_create_standard(3, 26, 15, 6);
            struct fann *ann = fann_create_from_file("language_classify.net");
            //fann_train_on_file(ann, "input.data", 200, 10, 0.0001);
            //fann_save(ann, "language_classify.net");
            //fann_print_parameters(ann);

            float *output = fann_run(ann, frequencies);
            std::cout << "Angielski: " << output[0] << std::endl
                      << "Finski : " << output[1] << std::endl
                      << "Francuski : " << output[2] << std::endl
                      << "Niemiecki : " << output[3] << std::endl
                      << "Polski : " << output[4] << std::endl
                      << "Hiszpanski : " << output[5] << std::endl;
            fann_destroy(ann);
        }
    }
    return 0;
}
