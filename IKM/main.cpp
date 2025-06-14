#include <iostream>
#include "Polynomial.h"

int main()
{
    setlocale(LC_ALL, "Russian");
    cout << "=== ������ � ������������ ===" << endl;

    string filename;
    cout << "������� ��� �����: ";
    cin >> filename;

    try
    {
        ifstream file(filename);
        if (!file.is_open()) { cout << "\n������ ������ �����."; exit(0); }

        string line;
        getline(file, line);
        file.close();

        cout << "\n�������� ���������: " << line << endl;

        Polynomial poly;
        poly.LoadFromFile(filename);

        cout << "\n��������� ���������: " << poly.ToString() << endl;

        poly.AppendToFile(filename);
        cout << "\n��������� �������� � ����." << endl;
    }
    catch (const exception& ex)
    {
        cerr << "������: " << ex.what() << endl;
    }

    return 0;
}