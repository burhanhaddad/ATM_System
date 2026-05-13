#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

const string ClientsFileName = "Clients.txt";

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
void ShowWithdrawScreen() {
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tQuick Withdraw\n";
    cout << "===========================================\n";
	cout << "      [1] 50      [2] 100      [3] 200\n";
	cout << "      [4] 500     [5] 1000     [6] Exit\n";
    int WithdrawOption;
	sClient Client= CurrentClient;
	cout << "Your Current Balance is: " << Client.AccountBalance << endl;
	cout << "Choose Withdraw Amount? [1 to 6]? ";
    cin >> WithdrawOption;
    char Answer = 'n';


    cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
        switch (WithdrawOption)
        {
        case 1:
            Client.AccountBalance -= 50;
            break;
        case 2:
            Client.AccountBalance -= 100;
            break;
        case 3:
            Client.AccountBalance -= 200;
            break;
        case 4:
            Client.AccountBalance -= 500;
            break;
        case 5:
            Client.AccountBalance -= 1000;
            break;
		case 6:
            AtmMainMenue();
			break;
        }
        cout << "\n\nDone Successfully. New balance is: " << Client.AccountBalance;
    }
    else
    {
		cout << "\n\nTransaction Canceled.";
    }
   
}
int ReadMainMenueOptions() {
    cout << "Choose what do you want to do? [1 to 5]? ";
    int MainMenueOption;
    cin >> MainMenueOption;
    return MainMenueOption;
}
void PerformMainMenueOption(enMainMenueOptions MainMenueOption) {
    switch (MainMenueOption)
    {
    case eQuickWithdraw:
		system("cls");
        ShowWithdrawScreen();
        GoBackToMainMenue();
        break;
    case eNormalWithdraw:
        cout << "You choose Normal Withdraw.\n";
        break;
    case eDeposit:
        cout << "You choose Deposit.\n";
        break;
    case eCheckBalance:
        cout << "You choose Check Balance.\n";
        break;
    case eLogout:
        cout << "You choose Logout.\n";
        break;
    default:
        cout << "Invalid Option.\n";
        break;
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
    

}


int main()
{
    Login();
    
    return 0;
}


