#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <Windows.h> 

using namespace std;

struct Bill
{
    string payer_account;
    string recipient_account;
    double amount;
};

void CreateBills(Bill* bills, const int N);
void PrintBills(Bill* bills, const int N);
void SortBills(Bill* bills, const int N);
double GetAmountByPayer(Bill* bills, const int N, const string& payer_account);
void SaveToFile(Bill* bills, const int N, const char* filename);
void LoadFromFile(Bill*& bills, int& N, const char* filename);

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int N;
    cout << "������ ������� ������� N: ";
    while (!(cin >> N) || N <= 0)
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "����������� ��������. ������ N �����: ";
    }

    Bill* bills = new Bill[N];

    string payer_account;
    double amount;
    char filename[100];
    int menuItem;

    do {
        cout << endl << endl;
        cout << "������� ��:" << endl;
        cout << " [1] - �������� ����� � ���������" << endl;
        cout << " [2] - ���� ����� �� �����" << endl;
        cout << " [3] - ������������� ������ �������� �� ��������� �������� ������� ��������" << endl;
        cout << " [4] - ���� ���������� ��� ����, ����� � ������� ��������" << endl;
        cout << " [5] - ����� ����� � ����" << endl;
        cout << " [6] - ���������� ����� �� �����" << endl;
        cout << " [0] - ����� �� ���������� ������ ��������" << endl;
        cout << "������ ��������: ";
        while (!(cin >> menuItem) || menuItem < 0 || menuItem > 6)
        {
            cin.clear();
            cin.ignore(10000, '\n'); // -> �������� �������� ������ ���� �������� �������� �������, ������ ������ �����.
            cout << "����������� ��������. ������ ����� ���� �����: ";
        }
        cin.ignore(); // ������� ����� �����

        switch (menuItem)
        {
        case 1:
            CreateBills(bills, N);
            break;
        case 2:
            PrintBills(bills, N);
            break;
        case 3:
            SortBills(bills, N);
            cout << "����� ������������." << endl;
            break;
        case 4:
            cout << "������ ������������� ������� ��������: ";
            getline(cin, payer_account);
            amount = GetAmountByPayer(bills, N, payer_account);
            if (amount >= 0)
            {
                cout << "����, ����� � ������� " << payer_account << ": " << amount << " ���." << endl;
            }
            else
            {
                cout << "�������� � ������������� �������� " << payer_account << " �� ��������." << endl;
            }
            break;
        case 5:
            cout << "������ ��'� �����: ";
            cin.getline(filename, 99);
            SaveToFile(bills, N, filename);
            break;
        case 6:
            cout << "������ ��'� �����: ";
            cin.getline(filename, 99);
            LoadFromFile(bills, N, filename);
            break;
        case 0:
            break;
        default:
            cout << "�� ����� ��������� ��������! ��� ������ ����� - ����� ��������� ������ ����." << endl;
        }

    } while (menuItem != 0);

    delete[] bills;
    return 0;
}

void CreateBills(Bill* bills, const int N)
{
    for (int i = 0; i < N; i++)
    {
        cout << "����� � " << i + 1 << ":" << endl;
        cout << " ������������� ������� ��������: ";
        getline(cin, bills[i].payer_account);
        cout << " ������������� ������� ����������: ";
        getline(cin, bills[i].recipient_account);
        cout << " ������������ ���� (���.): ";
        while (!(cin >> bills[i].amount) || bills[i].amount < 0)
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "����������� ��������. ������ ���� �����: ";
        }
        cin.ignore(); // ������� ����� �����
        cout << endl;
    }
}

void PrintBills(Bill* bills, const int N)
{
    cout << "========================================================================================" << endl;
    cout << "| � | ������������� ������� �������� | ������������� ������� ���������� | ���� (���.)  |" << endl;
    cout << "----------------------------------------------------------------------------------------" << endl;
    for (int i = 0; i < N; i++)
    {
        cout << "| " << setw(1) << right << i + 1 << " ";
        cout << "| " << setw(31) << left << bills[i].payer_account;
        cout << "| " << setw(33) << left << bills[i].recipient_account;
        cout << "| " << setw(12) << right << setprecision(2) << fixed << bills[i].amount << " |" << endl;
    }
    cout << "========================================================================================" << endl;
    cout << endl;
}

void SortBills(Bill* bills, const int N)
{
    Bill temp;
    for (int i = 0; i < N - 1; i++)
    {
        for (int j = 0; j < N - i - 1; j++)
        {
            if (bills[j].payer_account > bills[j + 1].payer_account)
            {
                temp = bills[j];
                bills[j] = bills[j + 1];
                bills[j + 1] = temp;
            }
        }
    }
}

double GetAmountByPayer(Bill* bills, const int N, const string& payer_account)
{
    double total_amount = 0;
    bool found = false;
    for (int i = 0; i < N; i++)
    {
        if (bills[i].payer_account == payer_account)
        {
            total_amount += bills[i].amount;
            found = true;
        }
    }
    if (found)
        return total_amount;
    else
        return -1; // ���� ������� �� ��������
}

void SaveToFile(Bill* bills, const int N, const char* filename)
{
    ofstream fout(filename, ios::binary);
    if (!fout)
    {
        cout << "������� �������� ����� ��� ������!" << endl;
        return;
    }
    fout.write((char*)&N, sizeof(N));
    for (int i = 0; i < N; i++)
    {
        size_t len;

        // �������� payer_account
        len = bills[i].payer_account.size();
        fout.write((char*)&len, sizeof(len));
        fout.write(bills[i].payer_account.c_str(), len);

        // �������� recipient_account
        len = bills[i].recipient_account.size();
        fout.write((char*)&len, sizeof(len));
        fout.write(bills[i].recipient_account.c_str(), len);

        // �������� amount
        fout.write((char*)&bills[i].amount, sizeof(bills[i].amount));
    }
    fout.close();
    cout << "��� ������ ��������� � ����." << endl;
}

void LoadFromFile(Bill*& bills, int& N, const char* filename)
{
    ifstream fin(filename, ios::binary);
    if (!fin)
    {
        cout << "������� �������� ����� ��� ����������!" << endl;
        return;
    }
    fin.read((char*)&N, sizeof(N));
    if (bills != nullptr)
    {
        delete[] bills;
    }
    bills = new Bill[N];
    for (int i = 0; i < N; i++)
    {
        size_t len;
        char* buffer;

        // ������� payer_account
        fin.read((char*)&len, sizeof(len));
        buffer = new char[len + 1];
        fin.read(buffer, len);
        buffer[len] = '\0';
        bills[i].payer_account = buffer;
        delete[] buffer;

        // ������� recipient_account
        fin.read((char*)&len, sizeof(len));
        buffer = new char[len + 1];
        fin.read(buffer, len);
        buffer[len] = '\0';
        bills[i].recipient_account = buffer;
        delete[] buffer;

        // ������� amount
        fin.read((char*)&bills[i].amount, sizeof(bills[i].amount));
    }
    fin.close();
    cout << "��� ������ ������� � �����." << endl;
}
