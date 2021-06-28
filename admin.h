#ifndef ADMIN_H
#define ADMIN_H
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cstring>
using namespace std;

// ------------------ADMIN FNS-----------------------
class Admin{
    char name[30];
    char email[50];
    char phone_no[20];
    char password[20];
    public:
    void add_admin(char* n, char* e, char* p, char* pass){
        strcpy(name, n);
        strcpy(email, e);
        strcpy(phone_no, p);
        strcpy(password, pass);
    }
    bool login(char* name, char* password){
        if(strcmp(name, this->name)==0 && strcmp(this->password,password)==0){
            //cout<<"here\n";
            return true;
        }
        return false;
    }
};

void initAdmins(Admin Ad[]){
    //add code
    std::ifstream f1;
    f1.open("C:\\Users\\abhay\\Desktop\\Travel Plan\\travel_plan__(windows-sys)\\TravelPlan\\admins.txt", ios::in);
    int i=0;
    while(!f1.eof()){
        f1.read( (char*)&Ad[i], sizeof(Admin) );
        i++;
    }
    //cout<<"Admins information read\n";
    f1.close();
}



// ----------------AGENTS-------------------
class Agent{
    public:
    char name[30];
    char email[50];
    char phone_no[20];
    public:
    Agent(string n="Alina", string e="a@mail.com", string p="9891002255"){
        strcpy(name, n.c_str());
        strcpy(email, e.c_str());
        strcpy(phone_no, p.c_str());
    }
    void add_details(string a, string b, string c){
        strcpy(name, a.c_str());
        strcpy(email, b.c_str());
        strcpy(phone_no, c.c_str());
    }
    string getName(){
        return name;
    }
};

void initAgents(Agent Ag[], int &agent_count){
    agent_count=0;
    std::ifstream f1;
    f1.open("C:\\Users\\abhay\\Desktop\\Travel Plan\\travel_plan__(windows-sys)\\TravelPlan\\agents.txt", ios::in);
    int i=0;
    f1.read( (char*)&Ag[i], sizeof(Agent) );

    while(!f1.eof()){
        i++;
        f1.read( (char*)&Ag[i], sizeof(Agent) );
    }
    //cout<<"Agent details initiated\n";
    agent_count = i;
    f1.close();
}

string assign_Agent(Agent A[], int no=10){
    int c = rand()%(no-1);
    return A[c].getName();
}


#endif // ADMIN_H
