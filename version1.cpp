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
    bool mark_added=false;

};
enum choice_client{
    show_list_of_clients=1,
    add_new_client=2,
    delete_client=3,
    update_client_information=4,
    find_client_=5,
    exit_=6,
};
const string FILE_NAME = "Clients.txt";
bool search_by_account_number(string account_number);
void main_program(choice_client choice);

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
    cout << "client name :" << clients.name << endl;
    cout << "client account balance :" << clients.account_balance << endl;
    cout << "client account number :" << clients.account_num << endl;
    cout << "client phone :" << clients.phone << endl;
    cout << "client pin code :" << clients.pin_code << endl;
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
    }while (choice < 1 || choice > 6);
    return choice_client(choice);
}


void main_menu(){

    cout<<"========================================"<<endl;
    cout<<setw(25)<<"main menu screen"<<endl;
    cout<<"========================================"<<endl;
    cout<<setw(30)<<"[1] show list of clients "<<endl;
    cout<<setw(24)<<"[2] add new client "<<endl;
    cout<<setw(22)<<"[3] delete client"<<endl;
    cout<<setw(35)<<"[4] update client information "<<endl;
    cout<<setw(21)<<"[5] find client "<<endl;
    cout<<setw(14)<<"[6] exit "<<endl;
    cout<<"========================================"<<endl;
    cout<<"choose what do you want to do? [1 to 6]? ";
    choice_client choice=validate_choice();
    main_program(choice);

}
void go_back_to_main_menu(){
    system("pause");
    main_menu();
}

void main_program(choice_client choice){
    while(true) {

        if (choice == ::show_list_of_clients) {
            showing_clients();
            go_back_to_main_menu();
            break;
        }
        if (choice == ::add_new_client) {
            add_clients_screen();
            go_back_to_main_menu();
            break;
        }
        if (choice == ::find_client_) {
            finding_client_screen();
           go_back_to_main_menu();
            break;

        }
        if (choice == ::update_client_information) {
            updating_screen();
            go_back_to_main_menu();
            break;

        }
        if (choice == ::delete_client) {
            deleting_screen();
            go_back_to_main_menu();
            break;

        }
        if (choice == ::exit_) {
            cout << "exiting the program.....";
            return;
        }
    }
}
int main(){

main_menu();
    return 0;
}