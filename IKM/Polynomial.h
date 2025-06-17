#pragma once
#include <string>

class Node
{
public:
    int Coefficient;
    int Power;
    Node* Next;

    Node(int coeff, int power);
};

class Polynomial
{
private:
    Node* Head;
    std::string CleanString(const std::string& input);
    void InsertTerm(int coeff, int power);

public:
    Polynomial();
    ~Polynomial();

    void ParseFromString(const std::string& inputRaw);
    std::string ToString() const;
    void LoadFromFile(const std::string& filename);
    void AppendToFile(const std::string& filename) const;
};
