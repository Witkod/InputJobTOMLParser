#include "cpptoml.h"
#include <iostream>


int main(int argc, char* argv[])
{
    std::shared_ptr<cpptoml::table> root = cpptoml::make_table();

    auto uzytkownicy = cpptoml::make_table_array();
 
    

    auto nobel1 = cpptoml::make_table();
    nobel1->insert("Dziedzina", std::string("Fizyka")); 
    nobel1->insert("Opis", std::string("""Odkryl nosnik energii \ 
        nazwany grawitonem"""));

    auto nobel2 = cpptoml::make_table();
    nobel2->insert("Dziedzina", std::string("Fizyka")); 
    nobel2->insert("Opis", std::string("Wyjasnil teorie strun"));  

    auto nobel3 = cpptoml::make_table();
    nobel3->insert("Dziedzina", std::string("Fizyka")); 
    nobel3->insert("Opis", std::string("Odkryl wnetrze czarnej dziury"));  

    auto noble = cpptoml::make_table_array();
    noble->push_back(nobel1);
    noble->push_back(nobel2);
    noble->push_back(nobel3);

    auto br = cpptoml::make_table();  
    br->insert("Imie", std::string("Bartlomiej"));
    br->insert("Nazwisko", std::string("Rachwal"));
    br->insert("Ilosc nobli", 3); 
    br->insert("Noble",noble);

    uzytkownicy->push_back(br);



    auto nobel4 = cpptoml::make_table();
    nobel4->insert("Dziedzina", std::string("Fizyka")); 
    nobel4->insert("Opis", std::string("Wyjasnil proces rozszerzania wszechswiata"));

    auto noble2 = cpptoml::make_table_array();
    noble2->push_back(nobel4);

    auto pw = cpptoml::make_table();  
    pw->insert("Imie", std::string("Piotr"));
    pw->insert("Nazwisko", std::string("Witkos"));
    pw->insert("Ilosc nobli", 1); 
    pw->insert("Noble",noble2);

    uzytkownicy->push_back(pw);



    root->insert("Uzytkownicy", uzytkownicy);
    std::cout << (*root);
    return 0;
}
