#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

const string ClientsFileName = "Clients.txt";
void GoBackToMainMenue();
void AtmMainMenue();
void Login();
void ShowQuickWithdrawScreen();
struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;


};
sClient CurrentClient;
enum enMainMenueOptions
{
    eQuickWithdraw = 1,
    eNormalWithdraw = 2,
    eDeposit = 3,
    eCheckBalance = 4,
    eLogout = 5
};
vector<string> SplitString(string S1, string Delim)
{

    vector<string> vString;

    short pos = 0;
    string sWord; // define a string variable  

    // use find() function to get the position of the delimiters  
    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos); // store the word   
        if (sWord != "")
        {
            vString.push_back(sWord);
        }

        S1.erase(0, pos + Delim.length());  /* erase() until positon and move to next word. */
    }

    if (S1 != "")
    {
        vString.push_back(S1); // it adds last word of the string.
    }

    return vString;

}

sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{

    sClient Client;
    vector<string> vClientData;

    vClientData = SplitString(Line, Seperator);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);//cast string to double


    return Client;

}
string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{

    string stClientRecord = "";

    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);

    return stClientRecord;

}

vector <sClient> LoadCleintsDataFromFile(string FileName)
{

    vector <sClient> vClients;

    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {

        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {

            Client = ConvertLinetoRecord(Line);

            vClients.push_back(Client);
        }

        MyFile.close();

    }

    return vClients;

}
vector <sClient> SaveCleintsDataToFile(string FileName, vector <sClient> vClients)
{

    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite

    string DataLine;

    if (MyFile.is_open())
    {

        for (sClient C : vClients)
        {

            if (C.MarkForDelete == false)
            {
                //we only write records that are not marked for delete.  
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;

            }

        }

        MyFile.close();

    }

    return vClients;

}

bool FindClientByAccountNumber(string AccountNumber,string PinCode, sClient& Client)
{
	vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    for (sClient C : vClients)
    {

        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }

    }
    return false;

}
bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector <sClient>& vClients)
{

 
    char Answer = 'n';


    cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {

        for (sClient& C : vClients)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance += Amount;
                SaveCleintsDataToFile(ClientsFileName, vClients);
                cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance;
                
                return true;
            }

        }


        return false;
    }

}

bool LoadClientInfo(string AccountNumber, string PinCode) {

    if (FindClientByAccountNumber(AccountNumber, PinCode, CurrentClient))
    {
        return true;

    }
    else
        return false;
}
void GoBackToMainMenue() {
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    AtmMainMenue();
}
short GetQuickWitDrawOption(short WithdrawOption) {

    

    switch (WithdrawOption) {
    case 1:
        return 50;
    case 2:
        return 100;
    case 3:
        return 200;
    case 4:
        return 500;
    case 5:
        return 1000;
    default:
        return 0;
    }
}
short ReadQuickOption() {
    short WithdrawOption;

    do
    {
        cout << "Choose Withdraw Amount? [1 to 6]? ";
        cin >> WithdrawOption;
    } while (WithdrawOption > 6 || WithdrawOption < 1);
    return WithdrawOption;
}
void PerformQuickWithDrawOption(short QuickOption) {

    if (QuickOption == 6)
        return;
    short WitdrawBalance = GetQuickWitDrawOption(QuickOption);

    if (WitdrawBalance > CurrentClient.AccountBalance)
    {
        cout << "The amount exceeds your balance, make another choice.\n";
        cout << "Press Anykey to continue...";
        system("pause>0");
        ShowQuickWithdrawScreen();
    }
   
    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, WitdrawBalance * -1, vClients);
    CurrentClient.AccountBalance -= WitdrawBalance;
}
void ShowQuickWithdrawScreen() {
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tQuick Withdraw\n";
    cout << "===========================================\n";
	cout << "      [1] 50      [2] 100      [3] 200\n";
	cout << "      [4] 500     [5] 1000     [6] Exit\n";
    cout << "Your Balance is: " << CurrentClient.AccountBalance << endl;
    PerformQuickWithDrawOption(ReadQuickOption());
}

void ShowDepositScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDeposit Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    double Amount = 0;
    cout << "\nPlease enter deposit amount? ";
    cin >> Amount;

    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, Amount, vClients);
    CurrentClient.AccountBalance += Amount;
}
void ShowWithDrawScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tWithdraw Screen";
    cout << "\n-----------------------------------\n";
    cout << "Your Balance is: " << CurrentClient.AccountBalance << endl;
    

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    

    int Amount = 0;
    cout << "\nPlease enter withdraw amount? ";
    cin >> Amount;
    while (Amount % 5 != 0) {
        cout << "Enter an amount multiple of 5's ?";
        cin >> Amount;
    }
    while (Amount > CurrentClient.AccountBalance)
    {
        cout << "\nAmount Exceeds the balance, you can withdraw up to : " << CurrentClient.AccountBalance << endl;
        cout << "Please enter another amount? ";
        cin >> Amount;
    }

    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, Amount * -1, vClients);
    CurrentClient.AccountBalance -= Amount;
}

void ShowCheckBalanceScreen() {

    cout << "\n-----------------------------------\n";
    cout << "\tWithdraw Screen";
    cout << "\n-----------------------------------\n";

    cout << "Your Balance is " << CurrentClient.AccountBalance;

}
int ReadMainMenueOptions() {
    cout << "Choose what do you want to do? [1 to 5]? ";
    int MainMenueOption;
    cin >> MainMenueOption;
    return MainMenueOption;
}
void PerformMainMenueOption(enMainMenueOptions MainMenueOption) {

    switch (MainMenueOption) {

    case enMainMenueOptions::eQuickWithdraw:
        system("cls");
        ShowQuickWithdrawScreen();
        break;
    case enMainMenueOptions::eNormalWithdraw:
        system("cls");
        ShowWithDrawScreen();
        GoBackToMainMenue();
        break;
    case enMainMenueOptions::eDeposit:
        system("cls");
        ShowDepositScreen();
        GoBackToMainMenue();
        break;
    case enMainMenueOptions::eCheckBalance:
        system("cls");
        ShowCheckBalanceScreen();
        GoBackToMainMenue();
        break;
    case enMainMenueOptions::eLogout:
        system("cls");
        Login();
        break;
    default:
        cout << "Invalid Choise!";
    }

}
void AtmMainMenue() {

    system("cls");
    cout << "===========================================\n";
    cout << "\t\tATM Main Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Quick Withdraw.\n";
    cout << "\t[2] Normal Withdraw.\n";
    cout << "\t[3] Deposit.\n";
    cout << "\t[4] Check Balance.\n";
    cout << "\t[5] Logout.\n";
    cout << "===========================================\n";
    PerformMainMenueOption((enMainMenueOptions)ReadMainMenueOptions());
}
void Login() {

    string AccountNumber, PinCode;
    bool LoginFaild = false;
    do
    {
        system("cls");
        cout << "\n---------------------------------\n";
        cout << "\tLogin Screen";
        cout << "\n---------------------------------\n";
        if (LoginFaild)
        {
            cout << "Invalid AccountNumber/PinCode!\n";
        }
        cout << "Enter Account Number? ";
        cin >> AccountNumber;
        cout << "Enter Pin Code? ";
        cin >> PinCode;
        LoginFaild = !LoadClientInfo(AccountNumber, PinCode);
    } while (LoginFaild);
    AtmMainMenue();
    

}


int main()
{
    Login();
    
    return 0;
}


