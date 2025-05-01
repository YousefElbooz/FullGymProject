// mainwindow.cpp (complete + organized)
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "FileHandler.h"
#include <QPixmap>
#include <QMessageBox>
#include <QFile>
#include <QMap>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    staffMap[manger->getId()]= manger;
    ui->setupUi(this);
    ui->FullWiedgit->setCurrentIndex(1);
    ui->LoginPageStackedWidget->setCurrentIndex(0);
    setPixmapForWidgets();
    FileHandler::loadMembers("C:/Users/Yousef/Documents/FullGymProject/FullGymProject/members.txt",members);
    FileHandler::loadStaff("C:/Users/Yousef/Documents/FullGymProject/FullGymProject/staffs.txt",staffMap);
    FileHandler::loadClasses("C:/Users/Yousef/Documents/FullGymProject/FullGymProject/classes.txt",classesmap);
    connect(ui->loginbtn, &QPushButton::clicked, this, [=] {
        QString usrEmail = ui->LineEditEmail->text();
        QString usrPassword = ui->LineEditPassword->text();
        bool loggedIn = false;

        for (auto mem : members) {
            if (mem->getEmail() == usrEmail && mem->getPassword() == usrPassword) {
                currMember = mem;
                QMessageBox::information(this, "Success", "You have logged in successfully.");
                ui->FullWiedgit->setCurrentIndex(1);
                loggedIn = true;
                break;  // Stop checking further
            }
        }

        if(!loggedIn){
            for(auto stf: staffMap){
                if(stf->getEmail()==usrEmail&&stf->getPassword()==usrPassword){
                    currStaff = stf;
                    QMessageBox::information(this, "Success", "You have logged in successfully.");
                    ui->FullWiedgit->setCurrentIndex(3);
                    loggedIn = true;
                    break;  // Stop checking further
                }
            }
        }

        if (!loggedIn) {
            QMessageBox::warning(this, "Invalid Email or Password", "Please re-enter the information correctly.");
        }
    });
    connect(ui->SignUp,&QPushButton::clicked,this,[=]{

    });
    connect(ui->toggleButton,&QPushButton::clicked,this,[=](){ui->LoginPageStackedWidget->setCurrentIndex(1);});
    connect(ui->toggleButton_2,&QPushButton::clicked,this,[=](){ui->LoginPageStackedWidget->setCurrentIndex(0);});
}

MainWindow::~MainWindow() {
    delete ui;
    FileHandler::saveMembers("C:/Users/Yousef/Documents/FullGymProject/FullGymProject/members.txt",members);
    FileHandler::saveStaff("C:/Users/Yousef/Documents/FullGymProject/FullGymProject/staffs.txt",staffMap);
    FileHandler::saveClasses("C:/Users/Yousef/Documents/FullGymProject/FullGymProject/classes.txt", classesmap);
    qDeleteAll(members);
    qDeleteAll(staffMap);
    qDeleteAll(classesmap);
}

void MainWindow::setPixmapForWidgets() {
    QString imagePaths[] = {
        "C:/Users/Yousef/Pictures/newGym.png", // 0 - label image (skip or use elsewhere)
        "C:/Users/Yousef/Downloads/ChatGPT Image Apr 15, 2025, 10_53_31 PM.png", // 1 - logo (still used)
        "C:/Users/Yousef/Downloads/dashboard-svgrepo-com.svg", // 2
        "C:/Users/Yousef/Downloads/gym-dumbbell-svgrepo-com.svg", // 3
        "C:/Users/Yousef/Downloads/group-of-businessmen-svgrepo-com.svg", // 4
        "C:/Users/Yousef/Downloads/add-user-svgrepo-com.svg", // 5
        "C:/Users/Yousef/Downloads/user-admin-svgrepo-com.svg", // 6
        "C:/Users/Yousef/Downloads/notification-bell-1397-svgrepo-com.svg", // 7
        "C:/Users/Yousef/Downloads/invoice-bill-svgrepo-com.svg", // 8
        "C:/Users/Yousef/Downloads/profile-round-1342-svgrepo-com.svg", // 9
        "C:/Users/Yousef/Downloads/setting-4-svgrepo-com.svg", // 10
        "C:/Users/Yousef/Downloads/logout-svgrepo-com.svg", // 11
        "C:/Users/Yousef/Downloads/line-chart-up-02-svgrepo-com.svg", // 12
        "C:/Users/Yousef/Downloads/enter-svgrepo-com.svg", // 13
        "C:/Users/Yousef/Downloads/cancel-photo-svgrepo-com.svg", // 14
        "C:/Users/Yousef/Downloads/calendar-event-available-svgrepo-com.svg", // 15
        "C:/Users/Yousef/Downloads/home-workouts-svgrepo-com.svg", // 16
        "C:/Users/Yousef/Downloads/auto-renewal-2-circle-fill-svgrepo-com.svg", // 17
        "C:/Users/Yousef/Downloads/court-playground-svgrepo-com.svg",        // 18
        "C:/Users/Yousef/Downloads/user-admin-svgrepo-com.svg",     // 19
        "C:/Users/Yousef/Downloads/sand-clock-svgrepo-com.svg",     // 20
        "C:/Users/Yousef/Downloads/stretching-svgrepo-com.svg",     //21
        "C:/Users/Yousef/Downloads/coach-coaching-physical-trainer-svgrepo-com.svg", //22
        "C:/Users/Yousef/Downloads/money-bag-svgrepo-com.svg",      //23
        "C:/Users/Yousef/Downloads/line-chart-up-02-svgrepo-com.svg",       //24
        "C:/Users/Yousef/Downloads/enter-svgrepo-com.svg",                  //25
        "C:/Users/Yousef/Downloads/cancel-photo-svgrepo-com.svg",           //26
        "C:/Users/Yousef/Downloads/calendar-event-available-svgrepo-com.svg",   //27
        "C:/Users/Yousef/Downloads/home-workouts-svgrepo-com.svg",      //28
        "C:/Users/Yousef/Downloads/auto-renewal-2-circle-fill-svgrepo-com.svg",     //29
        "C:/Users/Yousef/Downloads/court-playground-svgrepo-com.svg"            //30
    };

    // Logo only (still QLabel)
    ui->logo->setPixmap(QPixmap(imagePaths[1]));
    ui->logo_3->setPixmap(QPixmap(imagePaths[1]));
    ui->label->setPixmap(QPixmap(imagePaths[0]));
    // Buttons with icons
    ui->Dashboardbtn->setIcon(QIcon(imagePaths[2]));
    ui->GymMangbtn->setIcon(QIcon(imagePaths[3]));
    ui->TMbtn->setIcon(QIcon(imagePaths[4]));
    ui->Aubtn->setIcon(QIcon(imagePaths[5]));
    ui->Notibtn->setIcon(QIcon(imagePaths[7]));
    ui->Billbtn->setIcon(QIcon(imagePaths[8]));
    ui->ProfileBtn->setIcon(QIcon(imagePaths[9]));
    ui->SettingBtn->setIcon(QIcon(imagePaths[10]));
    ui->LogOutBtn->setIcon(QIcon(imagePaths[11]));
    ui->AdminBtn->setIcon(QIcon(imagePaths[19]));

    // Member buttons
    ui->Dashboardbtn_3->setIcon(QIcon(imagePaths[2]));
    ui->GymMangbtn_3->setIcon(QIcon(imagePaths[3]));
    ui->TMbtn_3->setIcon(QIcon(imagePaths[4]));
    ui->Aubtn_3->setIcon(QIcon(imagePaths[5]));
    ui->Aubtn_4->setIcon(QIcon(imagePaths[5]));
    ui->Notibtn_3->setIcon(QIcon(imagePaths[7]));
    ui->Billbtn_3->setIcon(QIcon(imagePaths[8]));
    ui->ProfileBtn_3->setIcon(QIcon(imagePaths[9]));
    ui->LogOutBtn_3->setIcon(QIcon(imagePaths[11]));
    ui->Subscriptionbtn->setIcon(QIcon(imagePaths[29]));

    // Optional: set icon size (applies to all buttons)
    QList<QPushButton*> allButtons = {
        ui->Dashboardbtn, ui->GymMangbtn, ui->TMbtn, ui->Aubtn, ui->Notibtn, ui->Billbtn,
        ui->ProfileBtn, ui->SettingBtn, ui->LogOutBtn,
        ui->Dashboardbtn_3, ui->GymMangbtn_3, ui->TMbtn_3, ui->Aubtn_3, ui->Aubtn_4,
        ui->Notibtn_3, ui->Billbtn_3, ui->ProfileBtn_3, ui->LogOutBtn_3,ui->AdminBtn
    };
    QString defaultStyle = R"(
    QPushButton {
        background-color: transparent;
        color: black;
        font: 16pt "Yeasty Flavors";
        text-align: left;
        border-radius: 5px;
    }
    QPushButton:hover {
        background-color: #dcdcdc;
    }
)";

    for (QPushButton* btn : allButtons) {
        btn->setIconSize(QSize(32, 32));
        btn->setStyleSheet(defaultStyle);
        connect(btn, &QPushButton::clicked, this, [=]() {
            for (QPushButton* otherBtn : allButtons) {
                otherBtn->setStyleSheet(defaultStyle);
            }

            QString activeStyle = QString(R"(
            QPushButton {
                background-color: transparent;
                color: black;
                font: 16pt "Yeasty Flavors";
                text-align: left;
                border-radius: 5px;
                border: 2px solid #f1c27d;
            }
            QPushButton:hover {
                background-color: %1;
            }
            )").arg("#f1c27d");

            btn->setStyleSheet(activeStyle);
            if(btn->objectName()=="LogOutBtn"){
                ui->FullWiedgit->setCurrentIndex(0);
            };
        });
    }

    ui->TrainerNumICon->setPixmap(QPixmap(imagePaths[22]));
    ui->SandClockICon->setPixmap(QPixmap(imagePaths[20]));
    ui->SessionICon->setPixmap(QPixmap(imagePaths[21]));
    ui->NotificationDashIcon->setPixmap(QPixmap(imagePaths[7]));
    ui->TotalMemberIcon->setPixmap(QPixmap(imagePaths[9]));
    ui->CashIcon->setPixmap(QPixmap(imagePaths[22]));
    ui->Chart->setPixmap(QPixmap(imagePaths[23]));
}


