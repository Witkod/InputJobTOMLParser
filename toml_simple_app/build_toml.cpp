#include "cpptoml.h"
#include <iostream>
//Table - to tablica par klucz = wartość , czyli taka mapa
//Array to zwykła tablica o elementach jednego typu


//czemu w innej kolejnosci się wypisuje to nie wiem
//cout wypisuje chyba konkretne typy w określonej kolejności
//w zależności od typu i zagnieżdżenia
//ale nigdzie nie widziałem o tym wyjaśnienia

int main(int argc, char* argv[])
{   
    //tworzymy tabele [mapę] źródłową, która opakowuje cały plik.toml
    std::shared_ptr<cpptoml::table> root = cpptoml::make_table();
    
//dodajemy do pliku pojedyncze pary: "klucz" = wartość
    root->insert("Integer", 1234L);
    root->insert("Double", 1.234);
    root->insert("String", std::string("ABCD"));


//tworzymy obiekt tabeli [mapy]
    auto table = cpptoml::make_table();
//dodajemy do tego obiektu kilka par klucz=wart
    table->insert("ElementOne", 1L);
    table->insert("ElementTwo", 2.0);
    table->insert("ElementThree", std::string("THREE"));

//tworzymy kolejną tabelę, którą potem wstawimy do poprzedniej
//jako zagnieżdżoną
    auto nested_table = cpptoml::make_table();
    nested_table->insert("ElementOne", 2L);
    nested_table->insert("ElementTwo", 3.0);
    nested_table->insert("ElementThree", std::string("FOUR"));

//zagnieżdżamy wcześniej utworzoną tabele w "table"
//nadając jej wartość klucza "Nested"
    table->insert("Nested", nested_table);

//i table wstawiamy do głównego pliku
//nadając nazwę czyli wart klucza "Table"
    root->insert("Table", table);

//tworzymy zwykłą tablicę [już nie mapę]
    auto int_array = cpptoml::make_array();
//dodajemy do niej elementy tego samego typu!
    int_array->push_back(1L);
    int_array->push_back(2L);
    int_array->push_back(3L);
    int_array->push_back(4L);
    int_array->push_back(5L);

//dodajemy naszą zwykłą tablicę do pliku
//nadając nazwę "IntegerArray"
    root->insert("IntegerArray", int_array);

//kolejna zwykła tablica
    auto double_array = cpptoml::make_array();
    double_array->push_back(1.1);
    double_array->push_back(2.2);
    double_array->push_back(3.3);
    double_array->push_back(4.4);
    double_array->push_back(5.5);
//dodajemy naszą zwykłą tablicę do pliku
    root->insert("DoubleArray", double_array);

    auto string_array = cpptoml::make_array();
    string_array->push_back(std::string("A"));
    string_array->push_back(std::string("B"));
    string_array->push_back(std::string("C"));
    string_array->push_back(std::string("D"));
    string_array->push_back(std::string("E"));
//dodajemy naszą zwykłą tablicę do pliku
    root->insert("StringArray", string_array);

//tworzymy tablicę map [Array of Tables]
    auto table_array = cpptoml::make_table_array();
//i 3 razy dodajemy tę samę wcześniej utworzoną tablicę
    table_array->push_back(table);
    table_array->push_back(table);
    table_array->push_back(table);

//dodajemy Array of Tables do pliku
    root->insert("TableArray", table_array);

//tworzymy zwykłą tablicę
    auto array_of_arrays = cpptoml::make_array();
//tylko tym razem wypełniamy ją innymi, wczesniej stworzonymi tablicami
    array_of_arrays->push_back(int_array);
    array_of_arrays->push_back(double_array);
    array_of_arrays->push_back(string_array);
//dodajemy do pliku
    root->insert("ArrayOfArrays", array_of_arrays);
//wypisujemy zawartość pliku
    std::cout << (*root);
    return 0;
}
