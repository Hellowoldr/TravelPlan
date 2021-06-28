#include "travelplan.h"
#include "ui_travelplan.h"
#include <QMessageBox>
#include "rapidcsv.h"
#include "graphs.h"
#include <iostream>
#include <QDebug>
#include "customers.h"
#include <QTextEdit>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QTextStreamManipulator>
#include <ctype.h>
#include <QDataStream>
#include "admin.h"

Plan P[10];
Trie CT;
GraphInt cost_graph;
GraphFloat time_graph;
map <string, int>m;
Admin Ad[3];
Agent Ag[15];
map<string, string> Parent;
map<string, int> dist;
string dest;
bool exists;
QString email;
int choice;
bool EXISTS;
string EMAIL;
string Email;
string EMail;
Plan Play;
int AMOUNT;

void initLocations(){
    string locations[50] = {
        "Delhi", "Mumbai", "Hongkong", "Tokyo", "Bangkok", "Singapore", "Sydney (Australia)", "Melbourne (Australia)",
        "Dubai", "Cairo", "Moscow", "Stockholm", "Seoul", "Canada", "San Francisco (USA)",
        "Washington DC (USA)", "LA (USA)", "Chicago (USA)", "New York (USA)", "Cape Town (South Africa)", "Rio De Janerio (Brazil)", "Santiago", "Mexico City",
        "Lima (Peru)", "Miami (USA)", "London", "Berlin (Germany)", "Rome", "Paris", "Spain", "Havana"
    };
    for(int i=0; locations[i]!=""; i++){
        m[locations[i]] = 1;
    }
    //cout<<"Initiated locations map\n";
}

void initGraphs(){
    //add code
    int edges=0;
    Edge E1;
    std::fstream f1;
    f1.open("C:\\Users\\abhay\\Desktop\\Travel Plan\\travel_plan__(windows-sys)\\TravelPlan\\places.txt", ios::in| ios::binary);
    f1.read( (char*)&E1, sizeof(Edge) );
    cost_graph.addEdge(E1.dest_1, E1.dest_2, E1.cost);

    while(!f1.eof()){
        edges++;
        f1.read( (char*)&E1, sizeof(Edge) );
        cost_graph.addEdge(E1.dest_1, E1.dest_2, E1.cost);
    }
    f1.close();
}

void initCustomers(Trie T){
    std::fstream f1;
    Customer C;

    f1.open("C:\\Users\\abhay\\Desktop\\Travel Plan\\travel_plan__(windows-sys)\\TravelPlan\\customers.txt", ios::in);
    //int i=0;
    do{
        //cout<<"im here\n";
        f1.read( (char*)&C, sizeof(Customer) );
        string email(C.email);
        T.insert(email);
        //i++;
    }while(!f1.eof());
    //cout<<"Initiated customer trie\n";
    f1.close();
}

TravelPlan::TravelPlan(QMainWindow *parent)
    : QMainWindow(parent)
    , ui(new Ui::TravelPlan)
{
    ui->setupUi(this);
    initAdmins(Ad);
    int agent_count;
    initAgents(Ag, agent_count);
    initLocations();
    initCustomers(CT);
    initGraphs();
    dijk_cost("Delhi", Parent, dist, cost_graph.m);
    initPlans(P, 10);
    srand (time(NULL));
    ui->stackedWidget->setCurrentIndex(0);
}

void print_path(string dest, Ui::TravelPlan *ui){
    QString destination=QString::fromStdString(dest);
    if(Parent[dest]==dest){
        ui->textEdit_4->insertPlainText(destination + "-->");
        return;
    }
    print_path(Parent[dest], ui);
    ui->textEdit_4->insertPlainText(destination + "-->");
}



TravelPlan::~TravelPlan()
{
    delete ui;
}



void TravelPlan::on_quitButton_clicked()
{
    qApp->exit();
}

void TravelPlan::on_adminPortal_clicked()
{
    ui->name->setText("");
    ui->Password->setText("");
    ui->stackedWidget->setCurrentIndex(1);
}

void TravelPlan::on_backToSafety_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void TravelPlan::on_userPortal_clicked()
{

    ui->stackedWidget->setCurrentIndex(2);
}
void TravelPlan::on_backSafety_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void TravelPlan::on_login_clicked()
{
    QString username = ui->name->text();
    QString password = ui->password->text();
    if(username =="Abhay Gupta")
    {
        if(password=="abhaygupta")
        {
                ui->stackedWidget->setCurrentIndex(3);
        }
        else
        {
            QMessageBox::warning(this, "Login", "Password entered is not correct");
        }
    }
    else if(username=="Abhilasha Bansal")
    {
        if(password=="12345abcde")
        {
                ui->stackedWidget->setCurrentIndex(3);
        }
        else
            QMessageBox::warning(this, "Login", "Password entered is not correct");
    }
    else
        QMessageBox::warning(this, "Login", "Username entered is not correct");
}

void TravelPlan::on_safety_clicked()
{
    ui->name->setText("");
    ui->password->setText("");
    ui->stackedWidget->setCurrentIndex(1);
}

void TravelPlan::on_addLocation_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void TravelPlan::on_safe_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void TravelPlan::on_back_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void TravelPlan::on_createAccount_clicked()
{
    ui->Password->setText("");
    ui->address->setText("");
    ui->email->setText("");
    ui->name_2->setText("");
    ui->number->setText("");
    ui->stackedWidget->setCurrentIndex(5);
}

void TravelPlan::on_goback_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void TravelPlan::on_findAPlan_clicked()
{
    ui->lineEdit->setText("");
    ui->stackedWidget->setCurrentIndex(6);

}

void TravelPlan::on_gosafe_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void TravelPlan::on_bookAPackage_clicked()
{
    ui->lineEdit_7->setText("");
    ui->stackedWidget->setCurrentIndex(7);
    ui->textEdit_5->setText("");
    for(int i=0; i<10; i++){
        QString Name_of_plan = QString::fromStdString(P[i].name_of_plan);
            ui->textEdit_5->insertPlainText(QString::number(i+1) + ")   " + Name_of_plan + "\n");
            ui->textEdit_5->insertPlainText("     Price per person : " + QString::number(P[i].cost) + "\n");

            ui->textEdit_5->insertPlainText("     Number of Days : " + QString::number(P[i].no_of_days) + "\n");

            ui->textEdit_5->insertPlainText("     Places : ");
            for(string s: P[i].l){

                QString places=QString::fromStdString(s);
                ui->textEdit_5->insertPlainText(places + " ");
            }
            ui->textEdit_5->insertPlainText("\n\n");

        }
    ui->textEdit_5->setReadOnly(true);
    ui->textEdit_5->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
}

void TravelPlan::on_gosafety_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void TravelPlan::on_allLocations_clicked()
{
    ui->stackedWidget->setCurrentIndex(8);
    ui->textEdit_3->setText("");
    int q=1;
    for(auto p: m)
    {
        QString name=QString::fromStdString(p.first);
        ui->textEdit_3->insertPlainText(QString::number(q) + ")   " + name + "\n");
        q++;
    }
    ui->textEdit_3->setReadOnly(true);
    ui->textEdit_3->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
}

void TravelPlan::on_Go_clicked()
{

    QString source = ui->sourceLocation->text();
    QString destination = ui->destinationLocation->text();
    int flight_cost = ui->flightCost->text().toInt();
    int flight_time = ui->flightTime->text().toInt();

    char a[30], b[30];
    memcpy(a, source.toStdString().c_str(), source.size());
    qDebug() << (char *)a;
    memcpy(b, destination.toStdString().c_str(), destination.size());
    qDebug() << (char *)b;
    Edge E(a, b, flight_cost, flight_time);
    std::fstream f3;
    f3.open("C:\\Users\\abhay\\Desktop\\Travel Plan\\travel_plan__(windows-sys)\\TravelPlan\\places.txt", ios::app);
    f3.write((char *)&E, sizeof(E));
    f3.close();
    cost_graph.addEdge(a, b, flight_cost);
    time_graph.addEdge(a, b, flight_time);
    QMessageBox::information(this, "Add Path", "New Path added successfully");
}


void TravelPlan::on_backs_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void TravelPlan::on_GO_clicked()
{
    string name = ui->name_2->text().toStdString();
    string address= ui->address->text().toStdString();
    string email = ui->email->text().toStdString();
    string number=ui->number->text().toStdString();
    string password=ui->Password->text().toStdString();
    createAccount(CT, name, address, email, number, password);
    ui->stackedWidget->setCurrentIndex(2);
}

void TravelPlan::on_safey_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void TravelPlan::on_viewAllBookings_clicked()
{
    ui->stackedWidget->setCurrentIndex(10);
    int total_books=0;
    QString name;
    QString email;
    QString date;
    QString number;
    QString total;
    QString agent;
    QString places;
    QString index;
    int i, j, k, l, m, x, p=1;
    ui->textEdit_2->setText("");
        std::ifstream f1;
        Booking B1;
        f1.open("C:\\Users\\abhay\\Desktop\\Travel Plan\\travel_plan__(windows-sys)\\TravelPlan\\bookings.txt",ios::in);
        f1.read( (char*)&B1, sizeof(Booking) );

        while(!f1.eof())
        {

            char NAME[30]={'\0'};
            char EMAIL[50]={'\0'};
            char DATE[20]={'\0'};
            char AGENT_NAME[30]={'\0'};
            char PLACES[30]={'\0'};
            i=0, j=0, k=0, l=0, m=0;
            while(isalnum(B1.cust_name[i]) || B1.cust_name[i]==' ')
            {
                NAME[i]=B1.cust_name[i];
                i++;
            }
            while(isalnum(B1.cust_email[j]) || B1.cust_email[j]=='@' || B1.cust_email[j]=='.')
            {
                EMAIL[j]=B1.cust_email[j];
                j++;
            }
            while(isalnum(B1.date[k]) || B1.date[k]=='/' || B1.date[k]=='.'  || B1.date[k]==' ')
            {
                DATE[k]=B1.date[k];
                k++;
            }
            while(isalnum(B1.agent_name[l]) || B1.agent_name[l]==' ')
            {
                AGENT_NAME[l]=B1.agent_name[l];
                l++;
            }
            name=QString::fromLocal8Bit(NAME, 30);
            date=QString::fromLocal8Bit(DATE, 20);
            email=QString::fromLocal8Bit(EMAIL, 50);
            agent=QString::fromLocal8Bit(AGENT_NAME, 30);
            //index=QString("%1").arg(p);
            ui->textEdit_2->insertPlainText(QString::number(p) + ")   Customer Name : " + name + "\n");
            ui->textEdit_2->insertPlainText("      Customer Email : " + email + "\n");
            ui->textEdit_2->insertPlainText("      Tentative date of travel : " + date + "\n");
            ui->textEdit_2->insertPlainText("      Places to be visited : ");
            while(B1.places[m][0]!='\0')
            {
                for(x=0;x<30;x++)
                {
                    if(B1.places[m][x]!='\0')
                        PLACES[x]=B1.places[m][x];
                    else
                        break;
                }
                places=QString::fromLocal8Bit(PLACES, 30);
                ui->textEdit_2->insertPlainText("      " + places + " ");
                m++;
            }

            number = QString("\n      Number of people in trip : %1\n").arg(B1.no_of_people);
            ui->textEdit_2->insertPlainText(number);
            total = QString("      Total amount of trip : %1\n").arg(B1.amount);
            ui->textEdit_2->insertPlainText(total);
            ui->textEdit_2->insertPlainText("      Booked by Agent : " + agent + "\n");
            ui->textEdit_2->insertPlainText("\n");
            total_books++;
            p++;
            f1.read( (char*)&B1, sizeof(Booking) );

        }
        QString total_bookings =QString("Total Bookings are : %1").arg( total_books);
        ui->textEdit_2->setReadOnly(true);
        ui->textEdit_2->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
        ui->label_23->setText(total_bookings);
        f1.close();
}

void TravelPlan::on_viewCustomers_clicked()
{
    ui->stackedWidget->setCurrentIndex(9);
    int total_cust=0;
    QString name;
    QString address;
    QString phone_number;
    QString email;
    int i, j, k, p=1;
    ui->textEdit->setText("");
        std::ifstream f1;
        Customer C1;
        f1.open("C:\\Users\\abhay\\Desktop\\Travel Plan\\travel_plan__(windows-sys)\\TravelPlan\\customers.txt", ios::in);
        f1.read( (char*)&C1, sizeof(Customer) );
        while(!f1.eof())
        {
            char Name[30]={'\0'};
            char Address[30]={'\0'};
            char Email[50]={'\0'};
            i=0, j=0, k=0;
            while(isalnum(C1.name[i]) || C1.name[i]==' ')
            {
                Name[i]=C1.name[i];
                i++;
            }
            while(isalnum(C1.address[j]) || C1.address[j]==' ')
            {
                Address[j]=C1.address[j];
                j++;
            }
            while(isalnum(C1.email[k]) || C1.email[k]=='@' || C1.email[k]=='.')
            {
                Email[k]=C1.email[k];
                k++;
            }
            name=QString::fromLocal8Bit(Name, 30);
           address= QString::fromLocal8Bit(Address, 30);
            phone_number=QString::fromLocal8Bit(C1.phone_no, 10);
            email=QString::fromLocal8Bit(Email, 50);
            ui->textEdit->insertPlainText(QString::number(p) + ") " + name + " ");
            ui->textEdit->insertPlainText(address + " ");
            ui->textEdit->insertPlainText(email + " ");
            ui->textEdit->insertPlainText(phone_number + "\n");
               total_cust++;
               p++;
               f1.read( (char*)&C1, sizeof(Customer) );
        }
        QString status =QString("Total Customers are : %1").arg(total_cust);
        ui->label_22->setText(status);
        ui->textEdit->setReadOnly(true);
        ui->textEdit->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
            f1.close();
}


void TravelPlan::on_Aage_clicked()
{
    dest = ui->lineEdit->text().toStdString();
    ui->stackedWidget->setCurrentIndex(11);
    if(m.count(dest)==0){
        QMessageBox::warning(this, "Location Not Found", "Entered location is not in correct format or Location is not there in the package. Redirecting You to the previos page");
        ui->stackedWidget->setCurrentIndex(6);
        return;
    }
    else{
        ui->textEdit_4->setText("");
        QString distance = QString("Flight Cost : %1").arg(dist[dest]);
        ui->textEdit_4->insertPlainText(distance + "\n");
        QString cost = QString("The minimum cost plan is for : %1INR").arg(2*dist[dest]+20000);
        ui->textEdit_4->insertPlainText(cost + "\n");
        print_path(dest, ui);
        ui->textEdit_4->setReadOnly(true);
        ui->textEdit_4->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    return;
}
}

void TravelPlan::on_gosaf_clicked()
{
    ui->lineEdit->setText("");
    ui->stackedWidget->setCurrentIndex(2);
}

void TravelPlan::on_searchAnother_clicked()
{
    ui->lineEdit->setText("");
    ui->stackedWidget->setCurrentIndex(6);
}

void TravelPlan::on_BookPlan_clicked()
{
    ui->lineEdit_2->setText("");
    ui->stackedWidget->setCurrentIndex(12);


}

void TravelPlan::on_safeback_clicked()
{
    ui->textEdit_4->setText("");
    ui->stackedWidget->setCurrentIndex(11);
}



void TravelPlan::on_go_clicked()
{
    ui->lineEdit_3->setText("");
     ui->lineEdit_4->setText("");
      ui->lineEdit_5->setText("");
      ui->label_32->setText("");
      EMail=ui->lineEdit_2->text().toStdString();
    std::fstream f1;
        f1.open("C:\\Users\\abhay\\Desktop\\Travel Plan\\travel_plan__(windows-sys)\\TravelPlan\\customers.txt", ios::in);
        Customer C;
        bool p=false;
        while(!f1.eof()){
            f1.read( (char*)&C, sizeof(Customer) );
            std::string str(C.email);
            if(str==EMail){
                p = true;
                break;
            }
        }
        f1.close();
    if(p){

        ui->stackedWidget->setCurrentIndex(13);

        }
        else{

        QMessageBox::warning(this, "Login Failed", "Email-id does not exist. User has not registered with our agency. Redirecting you to create account");
        ui->stackedWidget->setCurrentIndex(5);
    }
}

void TravelPlan::on_SAFETY_clicked()
{
    ui->lineEdit_2->setText("");
    ui->stackedWidget->setCurrentIndex(12);
}


void TravelPlan::on_backsgo_clicked()
{
    ui->lineEdit_7->setText("");
    ui->stackedWidget->setCurrentIndex(7);
}



void TravelPlan::on_BookThisPlan_clicked()
{
    choice= ui->lineEdit_7->text().toInt();
    ui->lineEdit_2->setText("");
    if(choice>=1 && choice<=10)
        ui->stackedWidget->setCurrentIndex(14);
    else
    {
        QMessageBox::warning(this, "No Plan selected", "You have entered a wrong choice for plan. Please enter a valid choice number");
        ui->lineEdit_7->setText("");
    }

}



void TravelPlan::on_createAccount_2_clicked()
{
    ui->name_2->setText("");
    ui->address->setText("");
    ui->email->setText("");
    ui->Password->setText("");
    ui->number->setText("");
    ui->stackedWidget->setCurrentIndex(5);
}



void TravelPlan::on_yoyo_clicked()
{
    Booking B;
    QString name, Date, Agent_name;
    name = ui->lineEdit_12->text();
    memcpy(B.cust_name, name.toStdString().c_str(), name.size());
    qDebug() << (char *)B.cust_name;
    strcpy(B.cust_email, EMAIL.c_str());
    int i=0;
            for(string s: P[choice-1].l)
            {
                strcpy(B.places[i], s.c_str());
                i++;
            }
    Date=ui->lineEdit_9->text();
    memcpy(B.date, Date.toStdString().c_str(), Date.size());
    qDebug() << (char *)B.date;
    B.no_of_people=ui->lineEdit_10->text().toInt();
    B.amount = (P[choice-1].cost)*B.no_of_people;
    AMOUNT = B.amount;
    strcpy(B.agent_name, assign_Agent(Ag, 10).c_str());
    std::fstream f1;
    f1.open("C:\\Users\\abhay\\Desktop\\Travel Plan\\travel_plan__(windows-sys)\\TravelPlan\\bookings.txt", ios::app);
    f1.write( (char*)&B, sizeof(B) );
    f1.close();

    QMessageBox::information(this, "Booking Successful", "Your booking has been created successfully, redirecting you to the User Portal");
    ui->stackedWidget->setCurrentIndex(2);
}

void TravelPlan::on_viewBookings_clicked()
{
    ui->lineEdit_13->setText("");
    ui->lineEdit_14->setText("");
    ui->stackedWidget->setCurrentIndex(16);
}

void TravelPlan::on_staysafe_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void TravelPlan::on_go_2_clicked()
{
    ui->lineEdit_9->setText("");
    ui->lineEdit_10->setText("");
    ui->lineEdit_12->setText("");
    ui->label_41->setText("");
    EMAIL = ui->lineEdit_8->text().toStdString();
    EXISTS = CT.search(EMAIL);
    std::fstream f1;
        f1.open("C:\\Users\\abhay\\Desktop\\Travel Plan\\travel_plan__(windows-sys)\\TravelPlan\\customers.txt", ios::in);
        Customer C;
        bool p=false;
        while(!f1.eof()){
            f1.read( (char*)&C, sizeof(Customer) );
            if(C.email==EMAIL){
                p = true;
                break;
            }
        }
        f1.close();
        if(p==true)
            ui->stackedWidget->setCurrentIndex(15);
        else
            QMessageBox::warning(this, "No email found", "You have not created an account with our travel agency. Please create an account");
}

void TravelPlan::on_custLogin_clicked()
{
    //ui->lineEdit_13->setText("");
    //ui->lineEdit_14->setText("");
    Email=ui->lineEdit_13->text().toStdString();
    char EMAIL[50];
    strcpy(EMAIL, Email.c_str());
    string Password=ui->lineEdit_14->text().toStdString();
    std::fstream f1;
        f1.open("C:\\Users\\abhay\\Desktop\\Travel Plan\\travel_plan__(windows-sys)\\TravelPlan\\customers.txt", ios::in);
        Customer C;
        bool p=false;
        while(!f1.eof()){
            f1.read( (char*)&C, sizeof(Customer) );
            std::string str(C.email);
            std::string pass(C.password);
            if(str==Email && pass==Password){
                p = true;
                break;
            }
        }
        f1.close();
        if(p==false)
            QMessageBox::warning(this, "Authentication Failed", "Entered Email and/or Password is incorrect. Please provide a correct email and paasword combination.");
        else
        {
            ui->stackedWidget->setCurrentIndex(17);
            ui->textEdit_6->setText("");
            int i, j, k, l;

            int bookings=0;
                            std::fstream f1;
                            f1.open("C:\\Users\\abhay\\Desktop\\Travel Plan\\travel_plan__(windows-sys)\\TravelPlan\\bookings.txt", ios::in);
                            Booking B;
                            f1.read( (char*)&B, sizeof(Booking) );
                            while(!f1.eof()){
                                if(B.cust_email==Email){
                                    bookings++;

                                    char NAME[30]={'\0'};
                                    char EMAIL[50]={'\0'};
                                    char DATE[20]={'\0'};
                                    char AGENT_NAME[30]={'\0'};

                                    i=0, j=0, k=0, l=0;
                                    while(isalnum(B.cust_name[i]) || B.cust_name[i]==' ')
                                    {
                                        NAME[i]=B.cust_name[i];
                                        i++;
                                    }
                                    while(isalnum(B.cust_email[j]) || B.cust_email[j]=='@' || B.cust_email[j]=='.')
                                    {
                                        EMAIL[j]=B.cust_email[j];
                                        j++;
                                    }
                                    QString name = "Customer Name : " + QString::fromLocal8Bit(NAME, 30) + "\n";
                                    QString email = "Customer Email : " + QString::fromLocal8Bit(EMAIL, 50) + "\n";
                                    QString places;
                                    ui->textEdit_6->insertPlainText(name + email);

                                   //ui->textEdit_6->insertPlainText("Places to be visited : " + QString::fromStdString(dest));

                                    //ui->textEdit_6->insertPlainText("\n");

                                    while(isalnum(B.date[k]) || B.date[k]=='/' || B.date[k]=='.'  || B.date[k]==' ')
                                    {
                                        DATE[k]=B.date[k];
                                        k++;
                                    }
                                    while(isalnum(B.agent_name[l]) || B.agent_name[l]==' ')
                                    {
                                        AGENT_NAME[l]=B.agent_name[l];
                                        l++;
                                    }



                                    QString date = "Tentative date of Travel : " + QString::fromLocal8Bit(DATE, 20) + "\n";

                                    QString number = QString("Number of people in trip : %1\n").arg(B.no_of_people);
                                    QString total = QString("Total amount of trip : %1\n").arg(B.amount);
                                    QString agent = "Agent Name : " + QString::fromLocal8Bit(AGENT_NAME, 30);
                                    ui->textEdit_6->insertPlainText(date + number + total + agent + "\n\n");

                                }
                                f1.read( (char*)&B, sizeof(Booking) );
                            }
                            if(bookings==0){
                                ui->textEdit_6->setText("No bookings found for this account!\n");
                            }
                            f1.close();
                            ui->textEdit_6->setReadOnly(true);
                            ui->textEdit_6->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
        }
}

void TravelPlan::on_stayHome_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void TravelPlan::on_CreateAccount_2_clicked()
{
    ui->name_2->setText("");
    ui->address->setText("");
    ui->email->setText("");
    ui->Password->setText("");
    ui->number->setText("");
    ui->stackedWidget->setCurrentIndex(5);
}

void TravelPlan::on_ultrasafe_clicked()
{
    ui->lineEdit_8->setText("");
    ui->stackedWidget->setCurrentIndex(14);
}

void TravelPlan::on_bookThisPlan_2_clicked()
{
    Booking B;
    QString name, agent_name, Date;

    name=ui->lineEdit_3->text();
    memcpy(B.cust_name, name.toStdString().c_str(), name.size());
    qDebug() << (char *)B.cust_name;

        memcpy(B.cust_email, EMail.c_str(), EMail.size());
        qDebug() << (char *)B.cust_email;
        strcpy(B.places[0], dest.c_str());
        strcpy(B.agent_name, assign_Agent(Ag, 10).c_str());
        Date=ui->lineEdit_4->text();
        memcpy(B.date, Date.toStdString().c_str(), Date.size());
        qDebug() << (char *)B.date;
        B.no_of_people=ui->lineEdit_5->text().toInt();
        int cost = (dist[dest]*2) + 20000;
        B.amount = (cost)*B.no_of_people;

        std::fstream f1;
        f1.open("C:\\Users\\abhay\\Desktop\\Travel Plan\\travel_plan__(windows-sys)\\TravelPlan\\bookings.txt", ios::app);
        f1.write( (char*)&B, sizeof(B) );
        f1.close();
        QMessageBox::information(this, "Travel Plan Booked", "You have suceesfully booked a travel plan with our travel agency. Redirecting you to the user Portal");
        ui->stackedWidget->setCurrentIndex(2);
}

void TravelPlan::on_ultrasafe_2_clicked()
{
    Booking B;

    B.no_of_people=ui->lineEdit_10->text().toInt();
    B.amount = (P[choice-1].cost)*B.no_of_people;
    AMOUNT = B.amount;

    ui->label_41->setText("The total amount of this trip is : " + QString::number(AMOUNT));
}

void TravelPlan::on_findCostPlan_clicked()
{

    int no_of_people=ui->lineEdit_5->text().toInt();
    int cost = (dist[dest]*2) + 20000;
    int amount=cost*no_of_people;
    ui->label_32->setText("Total cost of the plan is : " + QString::number(amount));
}
