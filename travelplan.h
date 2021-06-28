#ifndef TRAVELPLAN_H
#define TRAVELPLAN_H

#include <QMainWindow>
//#include <QWidget>
QT_BEGIN_NAMESPACE
namespace Ui { class TravelPlan; }
QT_END_NAMESPACE

class TravelPlan : public QMainWindow
{
    Q_OBJECT

public:
    TravelPlan(QMainWindow *parent = nullptr);
    ~TravelPlan();

private slots:


    void on_quitButton_clicked();
    void on_adminPortal_clicked();
    void on_backToSafety_clicked();

    void on_userPortal_clicked();

    void on_backSafety_clicked();

    void on_login_clicked();

    void on_safety_clicked();

    void on_addLocation_clicked();

    void on_safe_clicked();

    void on_back_clicked();

    void on_createAccount_clicked();

    void on_goback_clicked();

    void on_findAPlan_clicked();

    void on_gosafe_clicked();

    void on_bookAPackage_clicked();

    void on_gosafety_clicked();

    void on_allLocations_clicked();

    void on_Go_clicked();

    void on_viewCustomers_clicked();

    void on_backs_clicked();



    void on_safey_clicked();

    void on_viewAllBookings_clicked();

    void on_Aage_clicked();

    void on_gosaf_clicked();

    void on_searchAnother_clicked();



    void on_safeback_clicked();

void on_GO_clicked();

    void on_go_clicked();

    void on_SAFETY_clicked();

    void on_BookThisPlan_clicked();

    void on_backsgo_clicked();

    void on_BookPlan_clicked();

   // void on_BookThisPlan_clicked();

    void on_createAccount_2_clicked();

    void on_go_2_clicked();

    void on_yoyo_clicked();

    void on_viewBookings_clicked();

    void on_staysafe_clicked();

    void on_custLogin_clicked();

    void on_stayHome_clicked();

    void on_CreateAccount_2_clicked();

    void on_ultrasafe_clicked();

    void on_bookThisPlan_2_clicked();

    void on_ultrasafe_2_clicked();

    void on_findCostPlan_clicked();

private:
    Ui::TravelPlan *ui;

};

#endif // TRAVELPLAN_H
