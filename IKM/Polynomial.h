#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <vector>

using namespace std;

// Узел однонаправленного списка
class Node
{
public:
    int Coefficient;
    int Power;
    Node* Next;

    Node(int coeff, int power)
    {
        Coefficient = coeff;
        Power = power;
        Next = nullptr;
    }
};

// Класс для работы с многочленами
class Polynomial
{
private:
    Node* Head;

    // Удаление всех пробелов и замена тире на минус
    string CleanString(const string& input)
    {
        string cleaned;
        for (char ch : input)
        {
            if (ch == ' ') continue;
            if (ch == '–' || ch == '—') cleaned += '-';
            else cleaned += ch;
        }
        return cleaned;1
    }

    // Вставка с упорядочиванием
    void InsertTerm(int coeff, int power)
    {
        if (coeff == 0) return;

        Node* newNode = new Node(coeff, power);

        if (!Head || Head->Power < power)
        {
            newNode->Next = Head;
            Head = newNode;
            return;
        }

        Node* current = Head;
        Node* prev = nullptr;

        while (current && current->Power > power)
        {
            prev = current;
            current = current->Next;
        }

        if (current && current->Power == power)
        {
            current->Coefficient += coeff;
            if (current->Coefficient == 0)
            {
                // удалить узел
                if (prev) prev->Next = current->Next;
                else Head = current->Next;
                delete current;
            }
            delete newNode;
        }
        else
        {
            newNode->Next = current;
            if (prev) prev->Next = newNode;
            else Head = newNode;
        }
    }

public:
    Polynomial() { Head = nullptr; }

    ~Polynomial()
    {
        while (Head)
        {
            Node* temp = Head;
            Head = Head->Next;
            delete temp;
        }
    }

     // Парсинг строки в список
    void ParseFromString(const string& inputRaw)
    {
        string input = CleanString(inputRaw);

        if (input.empty())
            throw runtime_error("Строка пуста");

        if (input[0] != '+' && input[0] != '-')
            input = "+" + input;

        vector<string> terms;
        size_t pos = 0;
        for (size_t i = 1; i < input.length(); ++i)
        {
            if (input[i] == '+' || input[i] == '-')
            {
                terms.push_back(input.substr(pos, i - pos));
                pos = i;
            }
        }
        terms.push_back(input.substr(pos));

        char variable = '\0'; // Переменная, которая должна быть везде одна

        for (const string& term : terms)
        {
            if (term.length() < 2)
                throw runtime_error("Некорректный член: " + term);

            size_t i = 0;
            int sign = (term[i++] == '-') ? -1 : 1;

            // Извлечение коэффициента
            int coeff = 0;
            while (i < term.length() && isdigit(term[i]))
            {
                coeff = coeff * 10 + (term[i] - '0');
                ++i;
            }

            if (coeff == 0 && (i == 1 || !isdigit(term[1]))) // т.е. было просто "+x"
                coeff = 1;

            coeff *= sign;

            if (i == term.length())
            {
                // Это число без переменной, степень = 0
                InsertTerm(coeff, 0);
                continue;
            }

            // Ожидается переменная: ровно одна буква
            if (!isalpha(term[i]) || (i + 1 < term.length() && isalpha(term[i + 1])))
                throw runtime_error("Некорректная переменная в: " + term);

            char currentVar = term[i++];

            if (variable == '\0')
                variable = currentVar;
            else if (currentVar != variable)
                throw runtime_error(string("Разные переменные: ") + variable + " и " + currentVar);

            int power = 1; // Ïî óìîë÷àíèþ

            if (i < term.length())
            {
                if (term[i++] != '^')
                    throw runtime_error("Ожидался символ '^' в: " + term);

                if (i == term.length())
                    throw runtime_error("Степень не указана в: " + term);

                power = 0;
                while (i < term.length() && isdigit(term[i]))
                {
                    power = power * 10 + (term[i] - '0');
                    ++i;
                }

                if (i != term.length())
                    throw runtime_error("Недопустимые символы после степени в: " + term);
            }

            InsertTerm(coeff, power);
        }
    }

    // Вывод в строковом формате
    string ToString() const
    {
        if (!Head) return "0";

        stringstream ss;
        Node* current = Head;

        while (current)
        {
            int coeff = current->Coefficient;
            int power = current->Power;

            if (coeff > 0 && current != Head)
                ss << "+";
            if (power == 0)
                ss << coeff;
            else
            {
                if (coeff == 1) ss << "y";
                else if (coeff == -1) ss << "-y";
                else ss << coeff << "y";

                if (power != 1)
                    ss << "^" << power;
            }

            current = current->Next;
        }

        return ss.str();
    }

    // Загрузка из файла
    void LoadFromFile(const string& filename)
    {
        ifstream file(filename);
        if (!file.is_open()) throw runtime_error("Не удалось открыть файл.");

        string line;
        getline(file, line);
        file.close();

        ParseFromString(line);
    }

    // Дописать результат в файл
    void AppendToFile(const string& filename) const
    {
        ofstream file(filename, ios::app);
        if (!file.is_open()) throw runtime_error("Не удалось открыть файл для записи.");

        file << endl << "Результат: " << ToString() << endl;
        file.close();
    }
};
