#include <iostream>
#include <string>
#include <vector>
#include<fstream>
#include<iomanip>
#include <algorithm>
using namespace std;
struct stclient_data
{
    string account_num;
    string pin_code;
    string name;
    string phone;
    double account_balance;
    bool mark_for_delete=false;
    bool mark_for_update=false;
};
struct stuser_data
{
    string authority;
    string password;
    string permissions;
    bool mark_for_delete=false;
    bool mark_for_update=false;
    vector<bool>v_permissions;
};
enum choice_client{
    show_list_of_clients=1,
    add_new_client=2,
    delete_client=3,
    update_client_information=4,
    find_client_=5,
    transactions=6,
    manage_user=7,
    logout=8,
};
enum choice_transactions{
   Deposit=1,
    with_draw=2,
    total_balances=3,
    Main_menu=4,

};
enum choice_manage_users{

    show_list_of_users=1,
    add_new_user=2,
    delete_user=3,
    update_user_information=4,
    find_user_=5,
    main_menu_=6
};

const string FILE_NAME = "Clients.txt";
const string FILE_NAME_users = "Users.txt";

bool search_by_account_number(string account_number);
void main_program(choice_client choice,stuser_data user);
void transaction_menu(stuser_data user);
void login_screen();
void manage_user_screen(stuser_data user);
void go_back_manage_menu(stuser_data user);
bool find_by_username(string username, vector<stuser_data> users,stuser_data &user);
vector<bool>permissions_handling(int &permissions);
void access_denied_message(){
    cout<<"access denied,\n";
    cout<<"you dont have permission to do this\n please contact your manager"<<endl;
}
string account_num() {
    string acc_number;
    cout << "enter the account number: ";
    cin >> acc_number;
    return acc_number;
}
stclient_data Readclient()
{

    stclient_data client;
    do {
        client.account_num=account_num();
        bool exists =search_by_account_number(client.account_num);
        if(exists){
            cout<<endl<<"already "<<client.account_num<<" exists "<<endl;
        }
        else{
            break;
        }
    }
    while(true);
    cout << "enter the pin code: ";
    cin.ignore(1,'\n');
    getline(cin, client.pin_code);
    cout << "enter your name: ";
    getline(cin, client.name);
    cout << "enter the phone: ";
    cin.ignore(1,'\n');
    getline(cin, client.phone);
    cout << "enter the account balance: ";
    cin.ignore(1,'\n');
    cin >> client.account_balance;
    return client;
}
stclient_data change_client_rec(string account_number)
{
    stclient_data client;
    client.account_num=account_number;
    cout << "enter the pin code: ";
    cin.ignore(1,'\n');
    getline(cin, client.pin_code);
    cout << "enter your name: ";
    getline(cin, client.name);
    cout << "enter the phone: ";
    cin.ignore(1,'\n');
    getline(cin, client.phone);
    cout << "enter the account balance: ";
    cin.ignore(1,'\n');
    cin >> client.account_balance;
    return client;
}

string join_st(stclient_data client, string sep = "#//#") {
    string joined;
    joined += client.account_num + sep;
    joined += client.pin_code + sep;
    joined += client.name + sep;
    joined += client.phone + sep;
    joined += to_string(client.account_balance);
    return joined;
}

void append_to_file(stclient_data client) {
    fstream file_record;
    file_record.open(FILE_NAME, ios::app);
    if (file_record.is_open()) {
        string rec= join_st(client);
            file_record<<rec<<endl;
        file_record.close();
    }
    cout << "saved sucssesfully !!\n";
}
vector<string>split_rec(string whole_rec, string sep = "#//#") {
    short pos;
    string rec;
    vector<string>vrecord;

    while ((pos = whole_rec.find(sep)) != string::npos) {
        rec = whole_rec.substr(0, pos);
        if (rec != "") {
            vrecord.push_back(rec);
        }
        whole_rec.erase(0, pos + sep.size());
    }
    if (whole_rec != "") {
        vrecord.push_back(whole_rec);
    }

    return vrecord;
}
void print_client_rec(stclient_data Client){
    cout << "| " << setw(15) << left << Client.account_num;
    cout << "| " << setw(10) << left << Client.pin_code;
    cout << "| " << setw(40) << left << Client.name;
    cout << "| " << setw(12) << left << Client.phone;
    cout << "| " << setw(12) << left << Client.account_balance<<endl;
}
void menu_client(vector <stclient_data> vClients)
{ cout << "\n\t\t\t\t\tClient List ("<< vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(10) << "Pin Code";

    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    for (stclient_data Client : vClients) {
        print_client_rec(Client);
    }
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}

stclient_data ConvertLinetoRecord(string line) {
    stclient_data Client;
    vector<string> vClientData = split_rec(line);
    Client.account_num = vClientData[0];
    Client.pin_code = vClientData[1];
    Client.name = vClientData[2];
    Client.phone = vClientData[3];
    Client.account_balance = stod(vClientData[4]);
    return Client;
}
vector<stclient_data> load_client_data_from_file() {
    ifstream file(FILE_NAME);
    vector<stclient_data>rec;
    if (file.is_open()) {
        string line;
        stclient_data client;

        while (getline(file, line)) {
            client = ConvertLinetoRecord(line);
            rec.push_back(client);
        }
        file.close();
    }
    return rec;
}



bool search_by_account_number(  string account_number ) {
    vector<stclient_data>clients=load_client_data_from_file();
    for (stclient_data &client  : clients)
    {
        if (client.account_num == account_number) {
            cout << "client name :" << client.name<<endl;
            cout << "client account balance :" << client.account_balance<<endl;
            cout << "client account number :" << client.account_num<<endl;
            cout << "client phone :" << client.phone<<endl;
            cout << "client pin code :" << client.pin_code<<endl;
            return true;
        }

    }
    return false;

}
void adding_clients(){
    stclient_data client;
   client = Readclient();
    append_to_file(client);
}
void add_clients_screen(){
    cout<<"---------------------------"<<endl;
    cout<<"adding new client screen"<<endl;
    cout<<"---------------------------"<<endl;
    adding_clients();
}
void showing_clients(){
    vector<stclient_data> clients =load_client_data_from_file();
    menu_client(clients);
}

bool mark_for_del(string account_number, vector<stclient_data> &clients) {
    for (stclient_data& c : clients)
    {
        if (c.account_num == account_number) {
            c.mark_for_delete = true;
            return true;
        }
    }
    return false;
}
bool find_by_account_number(string account_number, vector<stclient_data> clients,stclient_data &client) {
    for (stclient_data& c : clients)
    {
        if (c.account_num == account_number) {
            client = c;
            return true;
        }
    }
    return false;
}
void print_client_card(stclient_data clients) {
    cout<<"-------------------------------------------------------------------"<<endl;
    cout << "client name :" << clients.name << endl;
    cout << "client account balance :" << clients.account_balance << endl;
    cout << "client account number :" << clients.account_num << endl;
    cout << "client phone :" << clients.phone << endl;
    cout << "client pin code :" << clients.pin_code << endl;
    cout<<"-------------------------------------------------------------------"<<endl;
}
void save_clients_del(const vector <stclient_data>&clients) {
    fstream file;
    file.open(FILE_NAME, ios::out);
    string data;
    stclient_data new_client;
    if (file.is_open()) {
        for ( const stclient_data &c : clients)
        {
            if (!c.mark_for_delete) {
                data = join_st(c);
                file << data << endl;
            }
        }
        file.close();
    }
}
bool del_search(string acc_num,vector<stclient_data>clients) {
    stclient_data client;
    char answer = 'n';
    if (find_by_account_number(acc_num, clients, client)) {
        print_client_card(client);
        cout << "are you sure you want ot delete this client y/n: ";
        cin >> answer;
        if (answer == 'y' || answer == 'Y') {
            mark_for_del(acc_num, clients);
            save_clients_del(clients);
            clients = load_client_data_from_file();
            cout << "deleted successfully"<<endl;
            return true;
        }
    }
    else {
        cout << "not found " << acc_num;
    }
    return false;
}
void deleting_screen(){
    cout<<"---------------------------"<<endl;
    cout<<"deleting client screen"<<endl;
    cout<<"---------------------------"<<endl;
    vector<stclient_data>clients=load_client_data_from_file();
    string account_number=account_num();
    del_search(account_number,clients);

}
bool mark_for_update(const string &AccountNumber, vector <stclient_data>&vClients) {
    for (stclient_data& C : vClients)
    {
        if (C.account_num == AccountNumber) {
            C.mark_for_update = true;
            return true;
        }
    }
    return false;
}
void save_clients_update(const string &account_num ,const vector <stclient_data>&clients) {
    fstream file;
    file.open(FILE_NAME, ios::out);
    string data;
    stclient_data new_client;
    if (file.is_open()) {
        for ( const stclient_data &c : clients)
        {
           if(c.mark_for_update){
               new_client= change_client_rec(account_num);
               data = join_st(new_client);
               file<<data<<endl;
           }
           else{
               data= join_st(c);
               file<<data<<endl;
           }

        }
        file.close();

    }

}
void UpdateClientByAccountNumber(const string &AccountNumber, vector<stclient_data>&vClients) {

    stclient_data Client;
     char Answer = 'n';
    if (find_by_account_number(AccountNumber, vClients, Client)) {
        print_client_card(Client);
        cout << "\n\nAre you sure you want update this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y') {
            mark_for_update(AccountNumber, vClients);
            save_clients_update(AccountNumber, vClients);
            vClients = load_client_data_from_file();
            cout << "\nClient updated Successfully."<<endl;
            return;
        }

    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber<< ") is Not Found!";
        return ;
    }
}
void updating_screen(){
    cout<<"---------------------------"<<endl;
    cout<<"updating client screen"<<endl;
    cout<<"---------------------------"<<endl;
    vector<stclient_data>clients=load_client_data_from_file();
    string account_number=account_num();
    UpdateClientByAccountNumber(account_number,clients);

}
void find_client(const string &acc_num){
    bool found = search_by_account_number(acc_num);
    if (!found) {
        cout << "not found"<<endl;
    }
}
void finding_client_screen(){
    cout<<"---------------------------"<<endl;
    cout<<"finding client screen"<<endl;
    cout<<"---------------------------"<<endl;
    string account_number=account_num();
    find_client(account_number);

}
choice_client validate_choice() {
    int choice;
    string input;
    do {
        cin >> input;
        if (all_of(input.begin(), input.end(), [](char c) { return isdigit(c); })) {
            choice = stoi(input);
        }
    }while (choice < 1 || choice > 8);
    return choice_client(choice);
}


void main_menu(stuser_data user_logged){

    cout<<"========================================"<<endl;
    cout<<setw(25)<<"main menu screen"<<endl;
    cout<<"========================================"<<endl;
    cout<<setw(30)<<"[1] show list of clients "<<endl;
    cout<<setw(24)<<"[2] add new client "<<endl;
    cout<<setw(22)<<"[3] delete client"<<endl;
    cout<<setw(35)<<"[4] update client information "<<endl;
    cout<<setw(21)<<"[5] find client "<<endl;
    cout<<setw(22)<<"[6] transactions "<<endl;
    cout<<setw(22)<<"[7] manage users "<<endl;
    cout<<setw(16)<<"[8] logout "<<endl;
    cout<<"========================================"<<endl;
    cout<<"choose what do you want to do? [1 to 8]? ";
    choice_client choice=validate_choice();
    main_program(choice,user_logged);

}
void go_back_to_main_menu(stuser_data user_logged){
    system("pause");
    main_menu(user_logged);
}
choice_transactions validate_choice_transactions() {
    int choice;
    string input;
    do {
        cin >> input;
        if (all_of(input.begin(), input.end(), [](char c) { return isdigit(c); })) {
            choice = stoi(input);
        }
    }while (choice < 1 || choice > 4);
    return choice_transactions(choice);
}

void deposit_transaction(stclient_data &client){
    short balance_deposit;

    string input;
    do {
        cout<<"\nhow much do you want to deposit(dont exceed 25000 in one transaction)? ";
        cin >> input;
        if (all_of(input.begin(), input.end(), [](char c) { return isdigit(c); })) {
            balance_deposit = stoi(input);
        }
    }while (balance_deposit>25000);
    char answer='n';
    cout<<"are you sure you want to confirm this transaction y/n";
    cin>>answer;
    if(answer=='y'||answer=='Y') {
        client.account_balance += balance_deposit;
        cout<<"\nthe new balance for "<<client.account_num<<" is "<<client.account_balance<<endl;
    }
    else{
        cout<<"\ntransaction is canceled.....\n";
    }
}
vector<stclient_data> update_client_with_new_balance(string account_number,stclient_data &client,vector<stclient_data>&clients){
    for (stclient_data &client_:clients) {
          if(client_.account_num==account_number){
              client_=client;
          }
    }
    return clients;

}
void save_clients_transactions_to_file(const vector <stclient_data>&clients) {
    fstream file;
    file.open(FILE_NAME, ios::out);
    string data;
    stclient_data new_client;
    if (file.is_open()) {
        for ( const stclient_data &c : clients)
        {
            string line= join_st(c);
            file<<line<<endl;

        }
        file.close();

    }

}
void deposit_screen(){
    cout<<"---------------------------"<<endl;
    cout<<"deposit screen"<<endl;
    cout<<"---------------------------"<<endl;
    vector<stclient_data>clients=load_client_data_from_file();
    stclient_data client;
    string account_number=account_num();
    bool exists=find_by_account_number(account_number,clients,client);
    while(!exists){
         cout<<"\nthis account doesn't exist\n";
         account_number=account_num();
        exists=find_by_account_number(account_number,clients,client);
    }
    print_client_card(client);
    deposit_transaction(client);
    clients=update_client_with_new_balance(account_number,client,clients);
    save_clients_transactions_to_file(clients);

}
void withdraw_transaction(stclient_data &client){
    short balance_withdraw;

    string input;
    do {
        cout<<"\nhow much do you want to withdraw(dont exceed 25000 in one transaction)? ";
        cin >> input;
        if (all_of(input.begin(), input.end(), [](char c) { return isdigit(c); })) {
            balance_withdraw = stoi(input);
        }
    }while (balance_withdraw>25000||balance_withdraw>client.account_balance);
    char answer='n';
    cout<<"are you sure you want to confirm this transaction y/n";
    cin>>answer;
    if(answer=='y'||answer=='Y') {
        client.account_balance -= balance_withdraw;
        cout<<"\nthe new balance for "<<client.account_num<<" is "<<client.account_balance<<endl;
    }
    else{
        cout<<"\ntransaction is canceled.....\n";
    }
}
void withdraw_screen(){
    cout<<"---------------------------"<<endl;
    cout<<"withdraw screen"<<endl;
    cout<<"---------------------------"<<endl;
    vector<stclient_data>clients=load_client_data_from_file();
    stclient_data client;
    string account_number=account_num();
    bool exists=find_by_account_number(account_number,clients,client);
    while(!exists){
        cout<<"\nthis account doesn't exist\n";
        account_number=account_num();
        exists=find_by_account_number(account_number,clients,client);
    }
    print_client_card(client);
    withdraw_transaction(client);
    clients=update_client_with_new_balance(account_number,client,clients);
    save_clients_transactions_to_file(clients);
}
void print_balance_card(stclient_data client){
    cout << "| " << setw(15) << left << client.account_num;
    cout << "| " << setw(40) << left << client.name;
    cout << "| " << setw(12) << left << client.account_balance<<endl;
}
int total_balance_menu(vector<stclient_data>clients){
    int total_balances=0;
    cout << "\n\t\t\t\t\tbalance List ("<< clients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    for (stclient_data client : clients) {
        print_balance_card(client);
        total_balances+=client.account_balance;
    }
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    return total_balances;
}
void show_total_balances(){
    cout<<"------------------------------"<<endl;
    cout<<"showing total balances screen"<<endl;
    cout<<"------------------------------"<<endl;
    vector<stclient_data>clients=load_client_data_from_file();
    int total_balances=total_balance_menu(clients);
    cout<<"\n\ntotal balance is "<<total_balances<<endl;
}
void go_back_to_transaction_menu(stuser_data user){
    system("pause");
    transaction_menu(user);
}
void transaction_performance(choice_transactions choice_tran,stuser_data user){
    while(true) {
        if (choice_tran == ::Deposit) {
            deposit_screen();
            go_back_to_transaction_menu(user);
            break;
        }
        if (choice_tran == ::with_draw) {
            withdraw_screen();
            go_back_to_transaction_menu(user);
            break;
        }
        if (choice_tran == ::total_balances) {
            show_total_balances();
            go_back_to_transaction_menu(user);
            break;
        }
        if (choice_tran == ::Main_menu) {
            go_back_to_main_menu(user);
            break;
        }
    }
}
void transaction_menu(stuser_data user){
    cout<<"\n\n========================================"<<endl;
    cout<<setw(33)<<"transactions  screen"<<endl;
    cout<<"========================================"<<endl;
    cout<<setw(14)<<"[1] deposit "<<endl;
    cout<<setw(15)<<"[2] withdraw "<<endl;
    cout<<setw(21)<<"[3] total balances "<<endl;
    cout<<setw(16)<<"[4] main menu "<<endl;
    cout<<"========================================"<<endl;
    cout<<"choose what do you want to do? [1 to 4]? ";
    choice_transactions choice_tran = validate_choice_transactions();
    transaction_performance(choice_tran,user);
}
void main_program(choice_client choice,stuser_data user_logged){
    while(true) {

        if (choice == ::show_list_of_clients&&(user_logged.v_permissions.at(0)||user_logged.v_permissions.empty())) {
            showing_clients();
            go_back_to_main_menu(user_logged);
            break;
        }
        else{
            access_denied_message();
            go_back_to_main_menu(user_logged);
        }
        if (choice == ::add_new_client&&(user_logged.v_permissions.at(1)||user_logged.v_permissions.empty())) {
            add_clients_screen();
            go_back_to_main_menu(user_logged);

            break;
        }
        else{
            access_denied_message();
            go_back_to_main_menu(user_logged);
        }
        if (choice == ::find_client_&&(user_logged.v_permissions.at(4)||user_logged.v_permissions.empty())) {
            finding_client_screen();
            go_back_to_main_menu(user_logged);

            break;

        }
        else{
            access_denied_message();
            go_back_to_main_menu(user_logged);
        }
        if (choice == ::update_client_information&&(user_logged.v_permissions.at(3)||user_logged.v_permissions.empty())) {
            updating_screen();
            go_back_to_main_menu(user_logged);

            break;

        }
        else{
            access_denied_message();
            go_back_to_main_menu(user_logged);
        }
        if (choice == ::delete_client&&(user_logged.v_permissions.at(2)||user_logged.v_permissions.empty())) {
            deleting_screen();
            go_back_to_main_menu(user_logged);

            break;

        }
        else{
            access_denied_message();
            go_back_to_main_menu(user_logged);
        }
        if(choice==::transactions&&(user_logged.v_permissions.at(5)||user_logged.v_permissions.empty())){
             transaction_menu(user_logged);
            go_back_to_main_menu(user_logged);
            break;
        }
        else{
            access_denied_message();
            go_back_to_main_menu(user_logged);
        }
        if(choice==::manage_user&&(user_logged.v_permissions.at(6)||user_logged.v_permissions.empty())){
               manage_user_screen(user_logged);
            go_back_to_main_menu(user_logged);
            break;

        }
        else{
            access_denied_message();
            go_back_to_main_menu(user_logged);
        }
        if (choice == ::logout) {
            login_screen();
            return;
        }
    }
}


//--------------------------------------------------------------------------------------------------
stuser_data ConvertLinetoRecord_user_version(string line) {
    stuser_data user;
    vector<string> vUser_data = split_rec(line);
    user.authority=vUser_data.at(0);
    user.password=vUser_data.at(1);
    user.permissions=vUser_data.at(2);
    return user;
}
string read_user_name(){
    string username;
    cout<<"\nenter username: ";
    cin>>username;
    return username;
}
bool user_found(string username,vector<stuser_data>&users){

    for (auto &user:users) {
        if(user.authority==username){
            return true;
        }
    }
    return false;
}
string join_st_user(stuser_data client, string sep = "#//#") {
    string joined;
    joined += client.authority + sep;
    joined += client.password + sep;
    joined += client.permissions;
    return joined;
}

vector<stuser_data>read_users_from_file(){
    ifstream user_file(FILE_NAME_users);
    string line;
    vector<stuser_data>vUsers;
    if(user_file.is_open()) {
        while (getline(user_file, line)) {
            stuser_data user = ConvertLinetoRecord_user_version(line);
            vUsers.push_back(user);
        }
        user_file.close();
    }
    return vUsers;
}
void showing_user_card(stuser_data user){
    cout<<"-------------------------------------------------------------------"<<endl;
    cout << "user name :" << user.authority << endl;
    cout << "user password :" << user.password << endl;
    cout << "user permissions :" << user.permissions << endl;
    cout<<"-------------------------------------------------------------------"<<endl;
}
choice_manage_users validate_choice_users() {
    int choice;
    string input;
    do {
        cin >> input;
        if (all_of(input.begin(), input.end(), [](char c) { return isdigit(c); })) {
            choice = stoi(input);
        }
    }while (choice < 1 || choice > 6);
    return choice_manage_users(choice);
}
void print_user_rec(stuser_data user){
    cout << "| " << setw(15) << left << user.authority;
    cout << "| " << setw(10) << left << user.password;
    cout << "| " << setw(40) << left << user.permissions<<endl;

}
//----------------------------------------------------------------------------------------------------------
void finding_screen_user(){
    string username;
    stuser_data user;
    cout<<"---------------------------"<<endl;
    cout<<" finding user screen "<<endl;
    cout<<"---------------------------"<<endl;
    vector<stuser_data>users=read_users_from_file();
    while(true){
        username=read_user_name();
        if(!user_found(username,users)){
            cout<<username<<" is not found please enter another username ";
            continue;
        }
        if(user_found(username,users)){
            find_by_username(username,users,user);
            showing_user_card(user);
            break;
        }
    }
}

void save_users_update(string username,const vector <stuser_data>&users) {
    fstream file;
    file.open(FILE_NAME_users, ios::out);
    string data;
    stuser_data updated_user;
    if (file.is_open()) {
        for ( const stuser_data &user : users)
        {
            if (!user.mark_for_update) {
                data = join_st_user(user);
                file << data << endl;
            }
            if (user.mark_for_update) {
                updated_user.authority=username;
                cout<<"\nenter password: ";
                cin>>updated_user.password;
                char full_acc;
                cout<<"\ndo you want to give full accsess y/n";
                cin>>full_acc;
                if(full_acc=='y'||full_acc=='Y'){
                    updated_user.permissions="-1";
                }
                if(full_acc=='n'||full_acc=='N'){
                    int permissions=0;
                    updated_user.v_permissions= permissions_handling(permissions);
                    updated_user.permissions= to_string(permissions);
                }
                data = join_st_user(updated_user);
                file << data << endl;
            }

        }
        file.close();
    }
}
bool mark_for_update(string username, vector<stuser_data> &users) {
    for (stuser_data& user : users)
    {
        if(user.authority==username){
            user.mark_for_update= true;
            return true;
        }
    }
    return false;
}
bool update_users(string username,vector<stuser_data>&users) {
    stuser_data user;
    char answer = 'n';
    if (find_by_username(username, users, user)) {
        showing_user_card(user);
        cout << " are you sure you want ot update this user y/n: ";
        cin >> answer;
        if (answer == 'y' || answer == 'Y') {
            mark_for_update(username, users);
            save_users_update(username,users);
            users = read_users_from_file();
            cout << "\nupdated successfully\n" << endl;
            return true;
        }
    }
    return false;
}
void updating_screen_user(){
    string username;
    cout<<"---------------------------"<<endl;
    cout<<" updating user screen "<<endl;
    cout<<"---------------------------"<<endl;
    vector<stuser_data>users=read_users_from_file();
    while(true){
        username=read_user_name();
        if(!user_found(username,users)){
            cout<<username<<" is not found please enter another username ";
            continue;
        }
        if(username=="Admin"){
            cout<<"\n can't update admin the admin\n ";
            continue;
        }
        if(user_found(username,users)){
            break;
        }
    }
    update_users(username,users);
}

bool find_by_username(string username, vector<stuser_data> users,stuser_data &user) {
    for (stuser_data& user_ : users)
    {
        if (user_.authority ==username ) {
            user = user_;
            return true;
        }
    }
    return false;
}
bool mark_for_del(string username, vector<stuser_data> &users) {
    for (stuser_data& user : users)
    {
        if (user.authority == username) {
            user.mark_for_delete = true;
            return true;
        }
    }
    return false;
}
void save_users_del(const vector <stuser_data>&users) {
    fstream file;
    file.open(FILE_NAME_users, ios::out);
    string data;
    if (file.is_open()) {
        for ( const stuser_data &user : users)
        {
            if (!user.mark_for_delete) {
                data = join_st_user(user);
                file << data << endl;
            }
        }
        file.close();
    }
}
bool del_search(string username,vector<stuser_data>&users) {
    stuser_data user;
    char answer = 'n';
    if (find_by_username(username, users, user)) {
        showing_user_card(user);
        cout << " are you sure you want ot delete this user y/n: ";
        cin >> answer;
        if (answer == 'y' || answer == 'Y') {
            mark_for_del(username, users);
            save_users_del(users);
            users = read_users_from_file();
            cout << "\ndeleted successfully\n" << endl;
            return true;
        }
    }
    return false;
}
void deleting_screen_user(){
    string username;
    cout<<"---------------------------"<<endl;
    cout<<" deleting user screen "<<endl;
    cout<<"---------------------------"<<endl;
    vector<stuser_data>users=read_users_from_file();
    while(true){
        username=read_user_name();
        if(!user_found(username,users)){
            cout<<username<<" is not found please enter another username ";
            continue;
        }
        if(username=="Admin"){
            cout<<"\n can't delete the admin\n ";
            continue;
        }
        if(user_found(username,users)){
            break;
        }
    }
    del_search(username,users);
}
vector<bool>permissions_handling(int &permissions){
    vector<bool>v_permissions(7);
    char per;
    int values_in_bin[]{1,2,4,8,16,32,64};
    cout<<"do you want to give access to:\n";
    cout<<"show client list ? y/n";
    cin>>per;
    if(per=='y'){
       v_permissions.at(0)=true;
    }
    cout<<"add new client  ? y/n ";
    cin>>per;
    if(per=='y'){
        v_permissions.at(1)=true;
    }
    cout<<"delete client ? y/n ";
    cin>>per;
    if(per=='y'){
        v_permissions.at(2)=true;

    }
    cout<<"update client ? y/n ";
    cin>>per;
    if(per=='y'){
        v_permissions.at(3)=true;
    }
    cout<<"find client ? y/n ";
    cin>>per;
    if(per=='y'){
        v_permissions.at(4)=true;
    }
    cout<<"transaction ? y/n ";
    cin>>per;
    if(per=='y'){
        v_permissions.at(5)=true;

    }
    cout<<"manage user ? y/n ";
    cin>>per;
    if(per=='y'){
        v_permissions.at(6)=true;
    }
    for (int i = 0; i < 7; ++i) {
        if(v_permissions.at(i)){
            permissions+=values_in_bin[i];
        }
    }
    return v_permissions;
}

bool adding_screen_validation(stuser_data &user_added){
    vector<stuser_data>vUsers=read_users_from_file();
    for (auto &i:vUsers) {
        if(i.authority==user_added.authority){
            return false;
        }
    }

    return true;
}

void user_add(stuser_data &user){
    bool wrong_add= false;
    do {
        if(wrong_add){
            cout<<user.authority<<" exists please enter another username";
        }
        cout<<"\nenter username: ";
        cin>>user.authority;
        cout<<"\nenter password: ";
        cin>>user.password;
        wrong_add=true;
    }
    while(!adding_screen_validation(user));
    wrong_add= false;
    char full_acc;
    cout<<"\ndo you want to give full accsess y/n";
    cin>>full_acc;
    if(full_acc=='y'||full_acc=='Y'){
        user.permissions="-1";
    }
    if(full_acc=='n'||full_acc=='N'){
        int permissions=0;
        user.v_permissions= permissions_handling(permissions);
        user.permissions= to_string(permissions);
    }
}
void append_to_file_user(stuser_data user) {
    fstream file_record;
    string line;
    file_record.open(FILE_NAME_users, ios::app);
    if (file_record.is_open()) {
        string rec= join_st_user(user);
        file_record<<rec<<endl;
        file_record.close();
    }
    cout << "saved sucssesfully !!\n";
}

void add_users_screen(){
    cout<<"---------------------------"<<endl;
    cout<<"adding new user screen"<<endl;
    cout<<"---------------------------"<<endl;
    char add_more='y';
    while(true){
        if(add_more=='n'){
            break;
        }
    stuser_data user;
    user_add(user);
    append_to_file_user(user);
    cout<<"\ndo you want to add more users y/n";
    cin>>add_more;

    }
}

void showing_users_menu(vector <stuser_data> vUsers)
{ cout << "\n\t\t\t\t\tClient List ("<< vUsers.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "| " << left << setw(15) << "username";
    cout << "| " << left << setw(10) << "Password";
    cout << "| " << left << setw(40) << "permissions";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    for (auto i: vUsers) {
        print_user_rec(i);
    }
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}
void showing_users(){
    vector<stuser_data>vUsers=read_users_from_file();
    showing_users_menu(vUsers);
}

void manage_user_logic(choice_manage_users choice,stuser_data user){
    while(true) {
        if (choice == ::show_list_of_users) {
            showing_users();
            go_back_manage_menu(user);
            break;
        }
        if (choice == ::add_new_user) {
            add_users_screen();
            go_back_manage_menu(user);
            break;
        }
        if (choice == ::delete_user) {
            deleting_screen_user();
            go_back_manage_menu(user);
            break;
        }
        if(choice==::update_user_information){
            updating_screen_user();
            go_back_manage_menu(user);
        }

        if(choice==::find_user_){
            finding_screen_user();
            go_back_manage_menu(user);
        }
        if(choice==::main_menu_){
            go_back_to_main_menu(user);
        }
        if (choice == ::main_menu_) {
            return;
        }
    }

}
void manage_user_screen(stuser_data user){
    cout<<"========================================"<<endl;
    cout<<setw(30)<<"manage user screen"<<endl;
    cout<<"========================================"<<endl;
    cout<<setw(30)<<"[1] show list of users "<<endl;
    cout<<setw(24)<<"[2] add new user "<<endl;
    cout<<setw(22)<<"[3] delete user"<<endl;
    cout<<setw(35)<<"[4] update user information "<<endl;
    cout<<setw(21)<<"[5] find user "<<endl;
    cout<<setw(21)<<"[6] main menu "<<endl;
    cout<<"========================================"<<endl;
    cout<<"choose what do you want to do? [1 to 6]? ";
    choice_manage_users choice=validate_choice_users();
    manage_user_logic(choice,user);
}

void go_back_manage_menu(stuser_data user){
    system("pause");
   manage_user_screen(user);
}

bool login_screen_validation(stuser_data &user_logged,vector<stuser_data>&vUsers){
        for (auto &i:vUsers) {
            if(i.authority==user_logged.authority||i.password==user_logged.password){
                return true;
            }
        }

    return false;
}
void user_login(stuser_data &user,vector<stuser_data>&vUsers){
while(true){
        cout<<"\nenter username: ";
        cin>>user.authority;
        cout<<"\nenter password: ";
        cin>>user.password;
        if(!login_screen_validation(user,vUsers)){
            cout<<"\nwrong username/password";
            continue;
        }
        if(login_screen_validation(user,vUsers)){
            break;
        }
}
}
void login_screen(){
    stuser_data user;
    cout<<"\n\n========================================"<<endl;
        cout<<setw(25)<<"login screen"<<endl;
        cout<<"========================================"<<endl;
    vector<stuser_data>vUsers=read_users_from_file();
    user_login(user,vUsers);
    main_menu(user);
}


int main(){
login_screen();

    return 0;
}