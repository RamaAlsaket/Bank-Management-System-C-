#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
using namespace std;
const string bankFile = "bank.txt";
const string UsersFileName = "user.txt";
void menu();
void showTranseactionScreen();
void showUserScreen();
struct clinetData {
    string accNumber;
    string pinCode;
    string name;
    string mobileNumber;
    double balance;
    bool markToDelete = false;
};
struct stUser {
    string UserName;
    string Password;
    int Permissions;
    bool MarkForDelete = false;
};
enum enUsers {
    enListUsres = 1, eAddNewUser = 2, eDeleteUser = 3,
    eUpdateUser = 4, eFindUser = 5, eMainMenue = 6
};
enum enMenuList
{
    enShowClinet = 1, enAddClinet = 2,
    endDeleteClient = 3, enUpdateClinet = 4,
    enFindClinet = 5, eTransactiona = 6, eManageUsers = 7, eExit = 8
};
enum enPermissions {
    eAll = -1, pListClients = 1, pAddNewClient = 2, pDeleteClient = 4,
    pUpdateClients = 8, pFindClient = 16, pTranactions = 32, pManageUsers = 64
};
enum enTrancstion {
    enDeposiet = 1, enWithDraw = 2,
    enTotalBalance = 3, enMainMenu = 4
};
stUser currectUser;
vector <string> split(string line, string saparete) {
    vector <string> vWord;
    string sWord;
    int pos = 0;
    while ((pos = line.find(saparete)) != std::string::npos) {
        sWord = line.substr(0, pos);
        if (sWord != "") {
            vWord.push_back(sWord);
        }
        line.erase(0, pos + saparete.length());
    }
    if (line != "") {
        vWord.push_back(line);
    }
    return vWord;
}
clinetData converteFromLineToRecored(string data, string saparete = "#//#") {
    vector <string> veClinte = split(data, saparete);
    clinetData stClinte;
    stClinte.accNumber = veClinte[0];
    stClinte.pinCode = veClinte[1];
    stClinte.name = veClinte[2];
    stClinte.mobileNumber = veClinte[3];
    stClinte.balance = stod(veClinte[4]);
    return stClinte;
}
string converteFromRecoredToLine(clinetData cliPerson, string sapareter = "#//#") {
    string toLine = "";
    toLine += cliPerson.accNumber + sapareter;
    toLine += cliPerson.pinCode + sapareter;
    toLine += cliPerson.name + sapareter;
    toLine += cliPerson.mobileNumber + sapareter;
    toLine += to_string(cliPerson.balance);
    return toLine;
}
stUser conveertUserInfroToRecord(string User, string separator = "#//#") {
    vector<string> vecUser = split(User, separator);

    stUser theUser;
    theUser.UserName = vecUser[0];
    theUser.Password = vecUser[1];
    theUser.Permissions = stoi(vecUser[2]);
    return theUser;
}
string convertUserRecordToLine(stUser user, string sapaerate = "#//#") {
    string toLine = "";
    toLine += user.UserName + sapaerate;
    toLine += user.Password + sapaerate;
    toLine += to_string(user.Permissions) + sapaerate;
    return toLine;
}
bool clientExistsByAccountNumber(string AccountNumber, string FileName) {
    fstream myFile(FileName, ios::in);
    if (myFile.is_open()) {
        string line;
        while (getline(myFile, line)) {
            clinetData client = converteFromLineToRecored(line);
            if (client.accNumber == AccountNumber) {
                myFile.close();
                return true;
            }
        }
        myFile.close();
    }
    return false;
}
bool userExitByUsername(string username, string fileName) {
    fstream MyFile;
    MyFile.open(fileName, ios::in);//read Mode

    if (MyFile.is_open())
    {

        string Line;
        stUser User;

        while (getline(MyFile, Line))
        {

            User = conveertUserInfroToRecord(Line);
            if (User.UserName == username)
            {
                MyFile.close();
                return true;
            }

        }

        MyFile.close();

    }

    return false;


}
clinetData ReadNewClient()
{
    clinetData Client;
    cout << "Enter Account Number? ";
    getline(cin >> ws, Client.accNumber);
    while (clientExistsByAccountNumber(Client.accNumber, bankFile))
    {
        cout << "\nClient with [" << Client.accNumber << "] already exists, Enter another Account Number? ";
        getline(cin >> ws, Client.accNumber);
    }
    cout << "Enter PinCode? ";
    getline(cin, Client.pinCode);

    cout << "Enter Name? ";
    getline(cin, Client.name);

    cout << "Enter Phone? ";
    getline(cin, Client.mobileNumber);

    cout << "Enter AccountBalance? ";
    cin >> Client.balance;

    return Client;
}
int ReadPermissionsToSet() {
    int perrm = 0;
    char answer = 'n';
    cout << "\nDo you want to give full access? y/n? ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y') {
        return -1;
    }
    cout << "\nDo you want to give access to : \n ";
    cout << "\nShow Client List? y/n? ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y') {
        perrm += enPermissions::pListClients;
    }
    cout << "\nAdd New Client? y/n? ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y') {
        perrm += enPermissions::pAddNewClient;
    }
    cout << "\nDelete Client? y/n? ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y') {
        perrm += enPermissions::pDeleteClient;
    }
    cout << "\nUpdate Client? y/n? ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y') {
        perrm += enPermissions::pUpdateClients;
    }
    cout << "\nFind Client? y/n? ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y') {
        perrm += enPermissions::pFindClient;
    }
    cout << "\nTransactions? y/n? ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y') {
        perrm += enPermissions::pTranactions;
    }
    cout << "\nManage Users? y/n? ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y') {
        perrm += enPermissions::pManageUsers;
    }
    return perrm;
}
stUser readNewUser() {
    stUser userData;
    cout << "Enter Username? ";
    getline(cin >> ws, userData.UserName);
    while (userExitByUsername(userData.UserName, UsersFileName)) {
        cout << "\nUser with [" << userData.UserName << "] already exists, Enter another Username ? ";
        getline(cin >> ws, userData.UserName);
    }
    cout << "Enter Password? ";
    getline(cin, userData.Password);
    userData.Permissions = ReadPermissionsToSet();
    return userData;
}
vector<stUser> loadUserDataFile(string fileName) {
    vector<stUser> vUsers;
    fstream MyFile;
    MyFile.open(fileName, ios::in);

    if (!MyFile.is_open()) {
        cout << "Error: Unable to open user file: " << fileName << endl;
        return vUsers;
    }

    string Line;
    stUser User;

    while (getline(MyFile, Line)) {
        User = conveertUserInfroToRecord(Line);
        vUsers.push_back(User);
    }

    MyFile.close();
    return vUsers;
}
vector <clinetData> loadClientData(string fileName) {
    fstream myFile;
    myFile.open(fileName, ios::in);
    vector <clinetData> veData;
    if (myFile.is_open()) {
        string line;
        clinetData person;
        while (getline(myFile, line)) {
            person = converteFromLineToRecored(line);
            veData.push_back(person);
        }
        myFile.close();
    }
    return veData;
}
void printTheRecored(clinetData clinInfo) {
    cout << "| " << setw(15) << left << clinInfo.accNumber;
    cout << "| " << setw(10) << left << clinInfo.pinCode;
    cout << "| " << setw(40) << left << clinInfo.name;
    cout << "| " << setw(12) << left << clinInfo.mobileNumber;
    cout << "| " << setw(12) << left << clinInfo.balance;
}
void printUserRecord(stUser userInfo) {
    cout << "| " << setw(15) << left << userInfo.UserName;
    cout << "| " << setw(10) << left << userInfo.Password;
    cout << "| " << setw(40) << left << userInfo.Permissions;
}
void printTheTotalBalance(clinetData client) {
    cout << "| " << setw(15) << left << client.accNumber;
    cout << "| " << setw(40) << left << client.name;
    cout << "| " << setw(12) << left << client.balance;

}
void ShowAccessDeniedMessage() {
    cout << "\n------------------------------------\n";
    cout << "Access Denied, \nYou dont Have Permission To Do this,\nPlease Conact Your Admin.";
    cout << "\n------------------------------------\n";
}
void printAllClinteData() {
    vector <clinetData> vecClinteData = loadClientData(bankFile);
    cout << "\n\t\t\t\t\t Clinte List (" << vecClinteData.size() << ") Clintes";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "| " << left << setw(15) << "Accont Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    if (vecClinteData.size() == 0) {
        cout << "\t\t\t\tNo Clients Available In the System!";
    }
    else
        for (clinetData s : vecClinteData) {
            printTheRecored(s);
            cout << endl;
        }
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}
void showAllUserScreen() {
    vector <stUser> user = loadUserDataFile(UsersFileName);
    cout << "\n\t\t\t\t\t Users List (" << user.size() << ") User";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "| " << left << setw(15) << "Username";
    cout << "| " << left << setw(10) << "Password";
    cout << "| " << left << setw(40) << "Permissions";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    if (user.size() == 0) {
        cout << "\t\t\t\tNo Users Available In the System!";
    }
    else
        for (stUser u : user) {
            printUserRecord(u);
            cout << endl;
        }
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}
void showTotalBalanceScreen() {
    vector <clinetData> vecClinteData = loadClientData(bankFile);
    cout << "\n\t\t\t\t\tBalances List (" << vecClinteData.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "| " << left << setw(15) << "Accont Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    double TotalBalances = 0;

    if (vecClinteData.size() == 0) {
        cout << "\t\t\t\tNo Clients Available In the System!";
    }
    else
        for (clinetData s : vecClinteData) {
            printTheTotalBalance(s);
            TotalBalances += s.balance;
            cout << endl;
        }
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "\t\t\t\t\t   Total Balances = " << TotalBalances;

}
void printTheCard(clinetData clinInfo) {
    cout << "\nThe following are the client details:\n";
    cout << "-----------------------------------";
    cout << "\nAccout Number  : " << clinInfo.accNumber;
    cout << "\nPin Code       : " << clinInfo.pinCode;
    cout << "\nName           : " << clinInfo.name;
    cout << "\nPhone          : " << clinInfo.mobileNumber;
    cout << "\nAccount Balance: " << clinInfo.balance;
    cout << "\n-----------------------------------\n";
}
void printUserCard(stUser User) {
    cout << "\nThe following are the user details:\n";
    cout << "-----------------------------------";
    cout << "\nUsername    : " << User.UserName;
    cout << "\nPassword    : " << User.Password;
    cout << "\nPermissions : " << User.Permissions;
    cout << "\n-----------------------------------\n";
}
bool findClientByAcc(vector <clinetData> veClinet, clinetData& stClient, string numAcc) {
    for (clinetData& C : veClinet) {
        if (C.accNumber == numAcc) {
            stClient = C;
            return true;
        }
    }
    return false;
}
bool findByUsername(string username, vector <stUser> vUser, stUser& user) {
    for (stUser U : vUser) {
        if (U.UserName == username) {
            user = U;
            return true;
        }
    }
    return false;
}
bool findUserByUsernameAndPass(string name, string pass, stUser& user) {
    vector<stUser> vUsers = loadUserDataFile(UsersFileName);

    for (stUser U : vUsers)
    {

        if (U.UserName == name && U.Password == pass)
        {
            user = U;
            return true;
        }

    }
    return false;


}
clinetData updateTheClientData(string number) {
    clinetData updatedDatat;
    updatedDatat.accNumber = number;
    cout << "\n\nEnter PinCode? ";
    getline(cin >> ws, updatedDatat.pinCode);
    cout << "Enter Name? ";
    getline(cin, updatedDatat.name);
    cout << "Enter Phone? ";
    getline(cin, updatedDatat.mobileNumber);
    cout << "Enter AccountBalance? ";
    cin >> updatedDatat.balance;
    return updatedDatat;
}
stUser changeUserRecord(string username) {
    stUser user;
    user.UserName = username;
    cout << "\n\nEnter Password? ";
    getline(cin >> ws, user.Password);

    user.Permissions = ReadPermissionsToSet();

    return user;
}
bool markToDelete(vector <clinetData>& veClinet, string numAcc) {
    for (clinetData& C : veClinet) {
        if (C.accNumber == numAcc) {
            C.markToDelete = true;
            return true;
        }
    }
    return false;
}
bool markUserForDelete(string username, vector <stUser> user) {
    for (stUser U : user) {
        if (U.UserName == username) {
            U.MarkForDelete = true;
            return true;
        }
    }
    return false;
}
vector <clinetData> saveDataToFile(string nameOfFile, vector <clinetData> veClinet) {
    fstream myFile;
    myFile.open(nameOfFile, ios::out);
    string clinet;
    if (myFile.is_open()) {

        for (clinetData C : veClinet) {
            if (C.markToDelete == false) {
                clinet = converteFromRecoredToLine(C);
                myFile << clinet << endl;
            }
        }
        myFile.close();
    }
    return veClinet;
}
vector <stUser> saveUserDataToFile(string filename, vector <stUser> vUsers) {
    fstream myfile;
    myfile.open(filename, ios::out);
    string dataLine;
    if (myfile.is_open())
    {

        for (stUser U : vUsers)
        {
            if (U.MarkForDelete == false) {
                dataLine = convertUserRecordToLine(U);
                myfile << dataLine << endl;
            }
            myfile.close();
        }
        return vUsers;
    }
}
void addToFile(string fileName, string stClient) {
    fstream myFile;
    myFile.open(fileName, ios::out | ios::app);
    if (myFile.is_open()) {
        myFile << stClient << endl;
        myFile.close();
    }
}
void addDataLineToFile(string fileName, string stUser) {
    fstream myFile;
    myFile.open(fileName, ios::out | ios::app);
    if (myFile.is_open()) {
        myFile << stUser << endl;
        myFile.close();
    }
}
void addNewClient() {
    clinetData person;
    person = ReadNewClient();
    addToFile(bankFile, converteFromRecoredToLine(person));
}
void addNewUsers() {
    stUser NewUser;
    NewUser = readNewUser();
    addDataLineToFile(UsersFileName, convertUserRecordToLine(NewUser));
}
void addNewUser() {
    char addMore = 'y';
    do {
        cout << "Add new User: " << endl;
        addNewUsers();
        cout << "\nUser Added Successfully,Do you want to add more User ? \n";
        cin >> addMore;
    } while (toupper(addMore) == 'Y');
}
void addMore() {
    char addMore = 'y';
    do {
        cout << "Add new clinte: " << endl;
        addNewClient();
        cout << "\nClient Added Successfully,Do you want to add more clinte ? \n";
        cin >> addMore;
    } while (toupper(addMore) == 'Y');
}
bool deleteAcc(vector <clinetData>& vecClient, string accNum) {
    char deleteAreYouSure = 'n';
    clinetData client;
    if (findClientByAcc(vecClient, client, accNum)) {
        printTheCard(client);
        cout << "\nAre you sure you want to delete this client? Y/N: ";
        cin >> deleteAreYouSure;
        if (deleteAreYouSure == 'y' || deleteAreYouSure == 'Y') {
            markToDelete(vecClient, accNum);
            saveDataToFile(bankFile, vecClient);
            vecClient = loadClientData(bankFile);
            cout << "\n\nClinte Deleted Successfully \n\n";
            return true;
        }
    }
    else {
        cout << "\nClient with Account Number (" << accNum << ") is Not Found!";
        return false;
    }
}
bool deleteUserByUsername(vector <stUser> vecUser, string username) {
    if (username == "Admin") {
        cout << "\n\nYou cannot Delete This User.";
        return false;
    }
    stUser theUser;
    char answer = 'n';

    if (findByUsername(username, vecUser, theUser)) {
        printUserCard(theUser);
        cout << "\n\nAre you sure you want delete this User? y/n ? ";
        cin >> answer;
        if (answer == 'y' || answer == 'Y') {
            markUserForDelete(username, vecUser);
            saveUserDataToFile(UsersFileName, vecUser);

            vecUser = loadUserDataFile(UsersFileName);
            cout << "\n\nUser Deleted Successfully.";
            return true;
        }
    }
    else
    {
        cout << "\nUser with Username (" << username << ") is Not Found!";
        return false;
    }
}
bool UpdateUserByUsername(string username, vector <stUser>& vecUser) {
    stUser user;
    char answer = 'n';
    if (findByUsername(username, vecUser, user)) {
        printUserCard(user);
        cout << "\n\nAre you sure you want update this User? y/n ? ";
        cin >> answer;
        if (answer == 'y' || answer == 'Y')
        {
            for (stUser U : vecUser) {
                if (U.UserName == username) {
                    U = changeUserRecord(username);
                    break;
                }
            }
            saveUserDataToFile(UsersFileName, vecUser);
            cout << "\n\nUser Updated Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nUser with Account Number (" << username << ") is Not Found!";
        return false;
    }

}
bool updateClientDataByAccNumber(vector <clinetData>& vecClient, string accNumber) {
    clinetData client;
    char update = 'n';
    if (findClientByAcc(vecClient, client, accNumber)) {
        printTheCard(client);
        cout << "Are you sure you want to update this clinte data ? Y/N || y/n: ";
        cin >> update;
        if (update == 'Y' || update == 'y') {
            for (clinetData& c : vecClient) {
                if (c.accNumber == accNumber) {
                    c = updateTheClientData(accNumber);
                    break;
                }
            }
            saveDataToFile(bankFile, vecClient);
            cout << "\n\nClinte Updated Successfully \n\n";
            return true;
        }
    }
    else {
        cout << "\nClient with Account Number (" << accNumber << ") is Not Found!";
        return false;
    }
}
bool despositeClientBalance(vector <clinetData>& vector, string accNumber, double amount) {
    char answer = 'n';
    cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y')
    {
        for (clinetData& c : vector) {
            if (c.accNumber == accNumber) {
                c.balance += amount;
                saveDataToFile(bankFile, vector);
                cout << "\n\nDone Successfully. New balance is: " << c.balance;
                return true;
            }
        }
        return false;
    }

}
string readAccNumber() {
    string numAcc = "";
    cout << "Enter the number of Account you search about: ";
    cin >> numAcc;
    return numAcc;
}
string readUsername() {
    string Username = "";
    cout << "\nPlease enter Username? ";
    cin >> Username;
    return Username;
}
void ShowListUserScreen() {
    showAllUserScreen();
}
void ShowAddNewUserScreen() {
    cout << "\n----------------------------------------------------------------------\n";
    cout << "\t\t\t\t\tAdd New User Screen";
    cout << "\n----------------------------------------------------------------------\n";
    addNewUser();
}
void ShowDeleteUserScreen() {
    cout << "\n-----------------------------------\n";
    cout << "\t\t\t\t\tDelete User Screen";
    cout << "\n-----------------------------------\n";
    vector <stUser> veUser = loadUserDataFile(UsersFileName);
    string username = readUsername();
    deleteUserByUsername(veUser, username);
}
void ShowUpdateUserrScreen() {
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate Users Screen";
    cout << "\n-----------------------------------\n";

    vector <stUser> vUsers = loadUserDataFile(UsersFileName);
    string Username = readUsername();

    UpdateUserByUsername(Username, vUsers);

}
bool CheckPermetion(enPermissions permission) {
    return (currectUser.Permissions == -1 || (currectUser.Permissions & permission) == permission);
}
void printDeleteScreen() {
    if (!CheckPermetion(enPermissions::pDeleteClient))
    {
        ShowAccessDeniedMessage();
        return;
    }

    cout << "\n-----------------------------------\n";
    cout << "\t\t\t\t\tDelete Client Screen";
    cout << "\n-----------------------------------\n";
    vector <clinetData> vecClient = loadClientData(bankFile);
    string accNumber = readAccNumber();
    deleteAcc(vecClient, accNumber);
}
void ShowUpdateClientScreen() {
    cout << "\n-----------------------------------\n";
    cout << "\t\t\t\t\tUpdate Client Info Screen";
    cout << "\n-----------------------------------\n";
    vector <clinetData> vClients = loadClientData(bankFile);
    string accNum = readAccNumber();
    updateClientDataByAccNumber(vClients, accNum);
}
void ShowAddNewClientsScreen() {
    cout << "\n----------------------------------------------------------------------\n";
    cout << "\t\t\t\t\tAdd New Clients Screen";
    cout << "\n----------------------------------------------------------------------\n";
    addMore();
}
void showfFindClientData() {
    cout << "\n----------------------------------------------------------------------\n";
    cout << "\t\t\t\t\tFind Client Screen";
    cout << "\n----------------------------------------------------------------------\n";
    vector <clinetData> vecClient = loadClientData(bankFile);
    clinetData client;
    string accountNumber = readAccNumber();
    if (findClientByAcc(vecClient, client, accountNumber)) {
        printTheCard(client);
    }
    else {
        cout << "\nClient with Account Number[" << accountNumber << "] is not found!";
    }
}
void ShowFindUserScreen() {
    cout << "\n-----------------------------------\n";
    cout << "\tFind User Screen";
    cout << "\n-----------------------------------\n";
    vector <stUser> vUsers = loadUserDataFile(UsersFileName);
    stUser User;
    string username = readUsername();
    if (findByUsername(username, vUsers, User)) {
        printUserCard(User);
    }
    else {
        cout << "\nUser with Username [" << username << "] is not found!";

    }
}
void endScreen() {
    cout << "\n----------------------------------------------------------------------\n";
    cout << "\t\t\t\t\tProgram Ends :-)";
    cout << "\n----------------------------------------------------------------------\n";
}
void showDepositeScreen() {
    cout << "\n-----------------------------------\n";
    cout << "\tDeposit Screen";
    cout << "\n-----------------------------------\n";
    vector <clinetData> vecClient = loadClientData(bankFile);
    clinetData client;
    string accountNumber = readAccNumber();
    while (!findClientByAcc(vecClient, client, accountNumber)) {
        cout << "\nClient with [" << accountNumber << "] does not exist.\n";
        accountNumber = readAccNumber();

    }
    printTheCard(client);
    double amount = 0;
    cout << "\nPlease enter deposit amount? ";
    cin >> amount;
    despositeClientBalance(vecClient, accountNumber, amount);

}
bool wethDrawClientBalance(vector <clinetData>& vector, string accNumber, double amount) {
    char answer = 'n';
    cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y')
    {
        for (clinetData& c : vector) {
            if (c.accNumber == accNumber) {
                c.balance -= amount;
                saveDataToFile(bankFile, vector);
                cout << "\n\nDone Successfully. New balance is: " << c.balance;
                return true;
            }
        }
        return false;
    }
}
void showWithDrawScreen() {
    cout << "\n-----------------------------------\n";
    cout << "\tWithdraw Screen";
    cout << "\n-----------------------------------\n";
    vector <clinetData> veClient = loadClientData(bankFile);
    clinetData client;
    string accountNumber = readAccNumber();
    while (!findClientByAcc(veClient, client, accountNumber)) {
        cout << "\nClient with [" << accountNumber << "] does not exist.\n";
        accountNumber = readAccNumber();
    }
    printTheCard(client);
    double amount = 0;
    cout << "\nPlease enter wethdraw amount? ";
    cin >> amount;
    wethDrawClientBalance(veClient, accountNumber, amount);
}
void backToMenuSecreen() {
    cout << "\n\nPress any key to go back to Main Menu...";
    system("pause>0");
    menu();
}
void backToTrancestionMenu() {
    cout << "\n\nPress any key to go back to Transecation Menu...";
    system("pause>0");
    showTranseactionScreen();
}
void GoBackToManageUsersMenue()
{
    cout << "\n\nPress any key to go back to Mnager Menue...";
    system("pause>0");
    showUserScreen();
}
void performUserMenuOption(enUsers userOption) {
    switch (userOption) {
    case enUsers::eAddNewUser:
        system("cls");
        ShowAddNewUserScreen();
        GoBackToManageUsersMenue();
        break;
    case enUsers::eDeleteUser:
        system("cls");
        ShowDeleteUserScreen();
        GoBackToManageUsersMenue();
        break;
    case enUsers::eFindUser:
        system("cls");
        ShowFindUserScreen();
        GoBackToManageUsersMenue();
        break;
    case enUsers::eMainMenue:
        system("cls");
        menu();
        break;
    case enUsers::enListUsres:
        system("cls");
        ShowListUserScreen();
        GoBackToManageUsersMenue();
        break;
    case enUsers::eUpdateUser:
        system("cls");
        ShowUpdateUserrScreen();
        GoBackToManageUsersMenue();
        break;
    }
}
int readUserOption() {
    cout << "Choose what do you want to do? [1 to 6]? ";
    int option = 0;
    cin >> option;
    return option;
}
void showUserScreen() {
    if (!CheckPermetion(enPermissions::pManageUsers)) {
        ShowAccessDeniedMessage();
        menu();
        return;
    }
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tManage Users Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] List Users.\n";
    cout << "\t[2] Add New User.\n";
    cout << "\t[3] Delete User.\n";
    cout << "\t[4] Update User.\n";
    cout << "\t[5] Find User.\n";
    cout << "\t[6] Main Menue.\n";
    cout << "===========================================\n";

    performUserMenuOption((enUsers)readUserOption());
}
int readNumberToTrancestion() {
    cout << "Enter the operation you want to do it: " << endl;
    int num;
    cin >> num;
    return num;
}
void trancestionPerformance(enTrancstion choice) {
    switch (choice) {
    case enTrancstion::enDeposiet:
    {
        system("cls");
        showDepositeScreen();
        backToTrancestionMenu();
        break;
    }
    case enTrancstion::enWithDraw:
    {
        system("cls");
        showWithDrawScreen();
        backToTrancestionMenu();
        break;
    }
    case enTrancstion::enTotalBalance:
    {
        system("cls");
        showTotalBalanceScreen();
        backToTrancestionMenu();
        break;
    }
    case enTrancstion::enMainMenu:
    {
        system("cls");
        menu();
        break;
    }
    }
}
void showTranseactionScreen() {
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tTransactions Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Total Balances.\n";
    cout << "\t[4] Main Menue.\n";
    cout << "===========================================\n";
    trancestionPerformance((enTrancstion)readNumberToTrancestion());
}
int readData() {
    int choice;
    cout << "\t\t\tChoose what do you want to do? [1 to 7]? ";
    cin >> choice;
    return choice;
}
void chooesMenu(enMenuList customerOption) {
    switch (customerOption) {
    case enMenuList::enShowClinet:
    {
        system("cls");
        printAllClinteData();
        backToMenuSecreen();
        break;
    }

    case enMenuList::enAddClinet:
    {
        system("cls");
        ShowAddNewClientsScreen();
        backToMenuSecreen();
        break;
    }

    case enMenuList::endDeleteClient:
    {
        system("cls");
        printDeleteScreen();
        backToMenuSecreen();
        break;
    }
    case enMenuList::enUpdateClinet:
    {
        system("cls");
        ShowUpdateClientScreen();
        backToMenuSecreen();
        break;
    }
    case enMenuList::enFindClinet:
    {
        system("cls");
        showfFindClientData();
        backToMenuSecreen();
        break;
    }
    case enMenuList::eTransactiona:
    {
        system("cls");
        showTranseactionScreen();
        backToMenuSecreen();
        break;
    }
    case enMenuList::eManageUsers:
    {
        system("cls");
        showUserScreen();
        backToMenuSecreen();
        break;
    }
    case enMenuList::eExit:
    {
        system("cls");
        endScreen();
        break;
    }
    }
}
void menu() {
    system("cls");
    cout << "=================================================================================\n";
    cout << "\t\t\t\tMain Menue Screen\n";
    cout << "=================================================================================\n";
    cout << "\t\t\t\t[1] Show Client List.\n";
    cout << "\t\t\t\t[2] Add New Client.\n";
    cout << "\t\t\t\t[3] Delete Client.\n";
    cout << "\t\t\t\t[4] Update Client Information.\n";
    cout << "\t\t\t\t[5] Find Client.\n";
    cout << "\t\t\t\t[6] Transeactions.\n";
    cout << "\t\t\t\t[7] Manage Users.\n";
    cout << "\t\t\t\t[8] LogOut.\n";
    cout << "=================================================================================\n";
    chooesMenu((enMenuList)readData());
}
bool loadUserInfo(string username, string pass) {
    if (findUserByUsernameAndPass(username, pass, currectUser))
        return true;
    else
        return false;

}
void login() {
    bool LoginFaild = false;

    string Username, Password;
    do
    {
        cout << "\n---------------------------------\n";
        cout << "\tLogin Screen";
        cout << "\n---------------------------------\n";

        if (LoginFaild)
        {
            cout << "Invlaid Username/Password!\n";
        }

        cout << "Enter Username? ";
        cin >> Username;

        cout << "Enter Password? ";
        cin >> Password;

        LoginFaild = !loadUserInfo(Username, Password);

    } while (LoginFaild);


    menu();
}
int main() {
    login();
    return 0;
}