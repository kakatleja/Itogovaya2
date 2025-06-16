#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <vector>

using namespace std;

// Óçåë îäíîíàïðàâëåííîãî ñïèñêà
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

// Êëàññ äëÿ ðàáîòû ñ ìíîãî÷ëåíàìè
class Polynomial
{
private:
    Node* Head;

    // Óäàëåíèå âñåõ ïðîáåëîâ è çàìåíà òèðå íà ìèíóñ
    string CleanString(const string& input)
    {
        string cleaned;
        for (char ch : input)
        {
            if (ch == ' ') continue;
            if (ch == '–' || ch == '—') cleaned += '-';
            else cleaned += ch;
        }
        return cleaned;
    }

    // Âñòàâêà ñ óïîðÿäî÷èâàíèåì
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
                // óäàëèòü óçåë
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

    // Ïàðñèíã ñòðîêè â ñïèñîê
    void ParseFromString(const string& inputRaw)
    {
        string input = CleanString(inputRaw);

        if (input.empty())
            throw runtime_error("Ñòðîêà ïóñòà");

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

        char variable = '\0'; // Ïåðåìåííàÿ, êîòîðàÿ äîëæíà áûòü âåçäå îäíà

        for (const string& term : terms)
        {
            if (term.length() < 2)
                throw runtime_error("Íåêîððåêòíûé ÷ëåí: " + term);

            size_t i = 0;
            int sign = (term[i++] == '-') ? -1 : 1;

            // Èçâëå÷åíèå êîýôôèöèåíòà
            int coeff = 0;
            while (i < term.length() && isdigit(term[i]))
            {
                coeff = coeff * 10 + (term[i] - '0');
                ++i;
            }

            if (coeff == 0 && (i == 1 || !isdigit(term[1]))) // ò.å. áûëî ïðîñòî "+x"
                coeff = 1;

            coeff *= sign;

            if (i == term.length())
            {
                // Ýòî ÷èñëî áåç ïåðåìåííîé, ñòåïåíü = 0
                InsertTerm(coeff, 0);
                continue;
            }

            // Îæèäàåòñÿ ïåðåìåííàÿ: ðîâíî îäíà áóêâà
            if (!isalpha(term[i]) || (i + 1 < term.length() && isalpha(term[i + 1])))
                throw runtime_error("Íåêîððåêòíàÿ ïåðåìåííàÿ â: " + term);

            char currentVar = term[i++];

            if (variable == '\0')
                variable = currentVar;
            else if (currentVar != variable)
                throw runtime_error(string("Разные переменные: ") + variable + " и " + currentVar);

            int power = 1; // Ïî óìîë÷àíèþ

            if (i < term.length())
            {
                if (term[i++] != '^')
                    throw runtime_error("Îæèäàëñÿ ñèìâîë '^' â: " + term);

                if (i == term.length())
                    throw runtime_error("Ñòåïåíü íå óêàçàíà â: " + term);

                power = 0;
                while (i < term.length() && isdigit(term[i]))
                {
                    power = power * 10 + (term[i] - '0');
                    ++i;
                }

                if (i != term.length())
                    throw runtime_error("Íåäîïóñòèìûå ñèìâîëû ïîñëå ñòåïåíè â: " + term);
            }

            InsertTerm(coeff, power);
        }
    }

    // Âûâîä â ñòðîêîâîì ôîðìàòå
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

    // Çàãðóçêà èç ôàéëà
    void LoadFromFile(const string& filename)
    {
        ifstream file(filename);
        if (!file.is_open()) throw runtime_error("Íå óäàëîñü îòêðûòü ôàéë.");

        string line;
        getline(file, line);
        file.close();

        ParseFromString(line);
    }

    // Äîïèñàòü ðåçóëüòàò â ôàéë
    void AppendToFile(const string& filename) const
    {
        ofstream file(filename, ios::app);
        if (!file.is_open()) throw runtime_error("Íå óäàëîñü îòêðûòü ôàéë äëÿ çàïèñè.");

        file << endl << "Ðåçóëüòàò: " << ToString() << endl;
        file.close();
    }
};
