//***************** My Solution *******************


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;


const string ClientsFileName = "Clients.txt";

void Login();
void ShowATMMainMenuScreen();

struct stClient {
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	double AccountBalance;
	bool MarkForDelete = false;
};

enum enATMMainMenuOptions {
	QuickWithdraw = 1,
	NormalWithdraw = 2,
	Deposit = 3,
	CheckBalance = 4,
	Logout = 5
};

enum enQuickWithdrawOptions {
	Twenty = 1,
	Fifty = 2,
	OneHunderd = 3,
	TwoHundred = 4,
	FourHundred = 5,
	SixHundred = 6,
	EightHundred = 7,
	OneThousand = 8,
	Exit = 9
};

stClient CurrentClient;

vector <string> SplitString(string S1, string Delim) {
	vector <string> vString;

	short pos = 0;
	string sWord;

	while ((pos = S1.find(Delim)) != std::string::npos) {
		sWord = S1.substr(0, pos);
		if (sWord != "") {
			vString.push_back(sWord);
		}

		S1.erase(0, pos + Delim.length());
	}

	if (S1 != "") {
		vString.push_back(S1);
	}

	return vString;
}

stClient ConvertLineToRecord(string Line, string Separator = "#//#") {
	stClient Client;
	vector <string> vClientData;

	vClientData = SplitString(Line, Separator);

	Client.AccountNumber = vClientData[0];
	Client.PinCode = vClientData[1];
	Client.Name = vClientData[2];
	Client.Phone = vClientData[3];
	Client.AccountBalance = stod(vClientData[4]);

	return Client;
}

vector <stClient> LoadClientsDataFromFile(string FileName) {
	vector <stClient> vClients;

	fstream MyFile;
	MyFile.open(FileName, ios::in);

	if (MyFile.is_open()) {
		string Line;
		stClient Client;

		while (getline(MyFile, Line)) {
			Client = ConvertLineToRecord(Line);
			vClients.push_back(Client);
		}

		MyFile.close();
	}

	return vClients;
}

bool FindClientByAccountNumberAndPinCode(string AccountNumber, string PinCode, stClient& Client) {
	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);

	for (stClient C : vClients) {
		if (C.AccountNumber == AccountNumber && C.PinCode == PinCode) {
			Client = C;
			return true;
		}
	}

	return false;
}

bool LoadClientInfo(string AccountNumber, string PinCode, stClient& Client) {
	return FindClientByAccountNumberAndPinCode(AccountNumber, PinCode, Client);
}

void ShowBalance() {
	cout << "Your Balance is: " << CurrentClient.AccountBalance;
}

void ShowCheckBalanceScreen() {
	system("cls");
	cout << "===========================================\n";
	cout << "\t\tCheck Balance Screen\n";
	cout << "===========================================\n";

	ShowBalance();
}

void GoBackToATMMainMenu() {
	cout << "\n\nPress any key to go back to Main Menu...";
	system("pause>0");
	ShowATMMainMenuScreen();
}

string ConvertRecordToLine(stClient Client, string Separator = "#//#") {
	string ClientRecord = "";

	ClientRecord += Client.AccountNumber + Separator;
	ClientRecord += Client.PinCode + Separator;
	ClientRecord += Client.Name + Separator;
	ClientRecord += Client.Phone + Separator;
	ClientRecord += to_string(Client.AccountBalance);

	return ClientRecord;
}

vector <stClient> SaveClientsDataToFile(string FileName, vector <stClient> vClients) {
	fstream MyFile;
	MyFile.open(FileName, ios::out);

	string DataLine;

	if (MyFile.is_open()) {
		for (stClient C : vClients) {
			if (C.MarkForDelete == false) {
				DataLine = ConvertRecordToLine(C);
				MyFile << DataLine << endl;
			}
		}

		MyFile.close();
	}

	return vClients;
}

void UpdateClientRecord(vector <stClient>& vClients, const stClient& UpdatedClient) {
	for (stClient& C : vClients) {
		if (C.AccountNumber == UpdatedClient.AccountNumber) {
			C = UpdatedClient;
			break;
		}
	}
}

bool UpdateBalanceForClient(double Amount, vector <stClient>& vClients) {
	char Answer = 'n';
	cout << "\n\nAre you sure you want to perform this transaction? y/n ? ";
	cin >> Answer;

	if (toupper(Answer) == 'Y') {
		CurrentClient.AccountBalance += Amount;
		UpdateClientRecord(vClients, CurrentClient);
		SaveClientsDataToFile(ClientsFileName, vClients);
		cout << "\n\nDone Successfully. New balance is: " << CurrentClient.AccountBalance;
		return true;
	}
	return false;
}

void ShowDepositScreen() {
	cout << "\n-----------------------------------\n";
	cout << "\tDeposit Screen";
	cout << "\n-----------------------------------\n";

	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);

	double Amount = 0;
	cout << "\nEnter a positive deposit amount? ";
	cin >> Amount;

	while (Amount <= 0) {
		cout << "\nPlease, enter a positive deposit amount? ";
		cin >> Amount;
	}

	UpdateBalanceForClient(Amount, vClients);
}

void ShowNormalWithdrawScreen() {
	cout << "\n-----------------------------------\n";
	cout << "\tNormal Withdraw Screen";
	cout << "\n-----------------------------------\n";

	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);;

	int Amount = 0;
	cout << "\nEnter an amount multiple of 5's? ";
	cin >> Amount;

	while (Amount <= 0 || Amount % 5 != 0) {
		cout << "\nPlease, enter an amount multiple of 5's? ";
		cin >> Amount;
	}

	while (Amount > CurrentClient.AccountBalance) {
		cout << "\nAmount Exceeds the balance, you can withdraw up to : " << CurrentClient.AccountBalance << endl;
		cout << "Please enter another amount? ";
		cin >> Amount;
	}

	UpdateBalanceForClient(Amount * -1, vClients);
}

short ReadQuickWithdrawOption() {
	cout << "Choose what do you want to do? [1 to 8]? ";
	short Choice = 0;
	cin >> Choice;
	return Choice;
}

void ShowQuickWithdrawScreen() {
	cout << "===========================================\n";
	cout << "\tQuick Withdraw Screen\n";
	cout << "===========================================\n";
	cout << "  [1] 20                [2] 50\n";
	cout << "  [3] 100               [4] 200\n";
	cout << "  [5] 400               [6] 600\n";
	cout << "  [7] 800               [8] 1000\n";
	cout << "  [9] Exit                      \n";
	cout << "===========================================\n";

	ShowBalance();
	cout << "\n";

	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);

	enQuickWithdrawOptions QuickWithdrawOption = (enQuickWithdrawOptions) ReadQuickWithdrawOption();

	int WithdrawAmount = 0;

	switch (QuickWithdrawOption) {
	case enQuickWithdrawOptions::Twenty:
		WithdrawAmount = 20;
		break;
	case enQuickWithdrawOptions::Fifty:
		WithdrawAmount = 50;
		break;
	case enQuickWithdrawOptions::OneHunderd:
		WithdrawAmount = 100;
		break;
	case enQuickWithdrawOptions::TwoHundred:
		WithdrawAmount = 200;
		break;
	case enQuickWithdrawOptions::FourHundred:
		WithdrawAmount = 400;
		break;
	case enQuickWithdrawOptions::SixHundred:
		WithdrawAmount = 600;
		break;
	case enQuickWithdrawOptions::EightHundred:
		WithdrawAmount = 800;
		break;
	case enQuickWithdrawOptions::OneThousand:
		WithdrawAmount = 1000;
		break;
	case enQuickWithdrawOptions::Exit:
		system("cls");
		ShowATMMainMenuScreen();
		return;
	default:
		cout << "\nInvalid Option.\n";
		return;
	}

	if (WithdrawAmount > CurrentClient.AccountBalance) {
		cout << "\nAmount Exceeds the balance, you can withdraw up to : " << CurrentClient.AccountBalance << endl;
		cout << "Press any key to continue...";
		system("pause>0");
		system("cls");
		ShowQuickWithdrawScreen();
		return;
	}

	UpdateBalanceForClient(-WithdrawAmount, vClients);
}

void PerformATMMainMenuOption(enATMMainMenuOptions ATMMainMenuOption) {
	switch (ATMMainMenuOption) {
		case enATMMainMenuOptions::QuickWithdraw:
			system("cls");
			ShowQuickWithdrawScreen();
			GoBackToATMMainMenu();
			break;
		case enATMMainMenuOptions::NormalWithdraw:
			system("cls");
			ShowNormalWithdrawScreen();
			GoBackToATMMainMenu();
			break;
	case enATMMainMenuOptions::Deposit:
		system("cls");
		ShowDepositScreen();
		GoBackToATMMainMenu();
		break;
	case enATMMainMenuOptions::CheckBalance:
		system("cls");
		ShowCheckBalanceScreen();
		GoBackToATMMainMenu();
		break;
	case enATMMainMenuOptions::Logout:
		system("cls");
		Login();
		break;
	}
}

short ReadATMMainMenuOption() {
	cout << "Choose what do you want to do? [1 to 5]? ";
	short Choice = 0;
	cin >> Choice;
	return Choice;
}

void ShowATMMainMenuScreen() {

	system("cls");
	cout << "===========================================\n";
	cout << "\tATM Main Menu Screen\n";
	cout << "===========================================\n";
	cout << "\t[1] Quick Withdraw.\n";
	cout << "\t[2] Normal Withdraw.\n";
	cout << "\t[3] Deposit.\n";
	cout << "\t[4] Check Balance.\n";
	cout << "\t[5] Logout.\n";
	cout << "===========================================\n";
	PerformATMMainMenuOption((enATMMainMenuOptions)ReadATMMainMenuOption());
}

void Login() {
	bool LoginFailed = false;

	string AccountNumber, PinCode;
	do {
		system("cls");

		cout << "\n-------------------------------------------\n";
		cout << "\tLogin Screen";
		cout << "\n-------------------------------------------\n";

		if (LoginFailed) {
			cout << "Invalid Account Number/PinCode!\n";
		}

		cout << "Enter Account Number? ";
		cin >> AccountNumber;

		cout << "Enter PinCode? ";
		cin >> PinCode;

		LoginFailed = !LoadClientInfo(AccountNumber, PinCode, CurrentClient);

	} while (LoginFailed);

	ShowATMMainMenuScreen();
}

int main() {

	Login();


	return 0;
}





// The Same Logic With Samll Difference