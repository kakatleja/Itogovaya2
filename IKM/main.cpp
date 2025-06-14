#include <iostream>
#include "Polynomial.h"

int main()
{
    setlocale(LC_ALL, "Russian");
    cout << "=== Работа с многочленами ===" << endl;

    string filename;
    cout << "Введите имя файла: ";
    cin >> filename;

    try
    {
        ifstream file(filename);
        if (!file.is_open()) { cout << "\nОшибка чтения файла."; exit(0); }

        string line;
        getline(file, line);
        file.close();

        cout << "\nИсходный многочлен: " << line << endl;

        Polynomial poly;
        poly.LoadFromFile(filename);

        cout << "\nСчитанный многочлен: " << poly.ToString() << endl;

        poly.AppendToFile(filename);
        cout << "\nРезультат добавлен в файл." << endl;
    }
    catch (const exception& ex)
    {
        cerr << "Ошибка: " << ex.what() << endl;
    }

    return 0;
}