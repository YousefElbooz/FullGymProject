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

    this->setWindowFlag(Qt::FramelessWindowHint);
    setFixedSize(1350,710);
    staffMap[manger->getId()]= manger;
    ui->setupUi(this);
    ui->FullWiedgit->setCurrentIndex(2);
    ui->LoginPageStackedWidget->setCurrentIndex(0);
    ui->staffMainStackWidget->setCurrentIndex(0);
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
                ui->FullWiedgit->setCurrentIndex(2);
                loggedIn = true;
                break;  // Stop checking further
            }
        }

        if(!loggedIn){
            for(auto stf: staffMap){
                if(stf->getEmail()==usrEmail&&stf->getPassword()==usrPassword){
                    currStaff = stf;
                    QMessageBox::information(this, "Success", "You have logged in successfully.");
                    ui->FullWiedgit->setCurrentIndex(1);
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
        QString username = ui->LineEditUserNameSignUp->text();
        QString email = ui->LineEditEmailSignUp->text();
        QString password = ui->LineEditPasswordSignUp->text();
        QString phone = ui->LineEditPhoneSignUp->text();
        QString ageStr = ui->LineEditAgeSignUp->text();
        QString address = ui->LineEditAddressSignUp->text();
        bool isMale = ui->MaleRadio->isChecked();
        bool isFemale = ui->FemaleRadio->isChecked();

        // Basic validation
        if (username.isEmpty() || username.length() < 3) {
            QMessageBox::warning(this, "Validation Error", "Username must be at least 3 characters.");
            return;
        }

        QRegularExpression emailRegex(R"([a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\.[a-zA-Z0-9-.]+)");
        if (!emailRegex.match(email).hasMatch()) {
            QMessageBox::warning(this, "Validation Error", "Please enter a valid email.");
            return;
        }

        if (password.length() < 6) {
            QMessageBox::warning(this, "Validation Error", "Password must be at least 6 characters.");
            return;
        }

        if (!phone.contains(QRegularExpression("^[0-9]{10,}$"))) {
            QMessageBox::warning(this, "Validation Error", "Enter a valid phone number (10+ digits).");
            return;
        }

        bool ok;
        int age = ageStr.toInt(&ok);
        if (!ok || age < 10 || age > 100) {
            QMessageBox::warning(this, "Validation Error", "Enter a valid age (10–100).");
            return;
        }

        if (!isMale && !isFemale) {
            QMessageBox::warning(this, "Validation Error", "Please select a gender.");
            return;
        }
        QString gender;
        if(isMale){
            gender = "male";
        }else{
            gender = "female";
        }
        // Passed all validations
        QMessageBox::information(this, "Success", "Sign-up completed successfully.");
        Member * m = new Member(username,email,password,gender,false,phone,address,ageStr.toInt());
        currMember = m;
        members[m->getId()]=m;
    });
    connect(ui->toggleButton,&QPushButton::clicked,this,[=](){ui->LoginPageStackedWidget->setCurrentIndex(1);});
    connect(ui->toggleButton_2,&QPushButton::clicked,this,[=](){ui->LoginPageStackedWidget->setCurrentIndex(0);});
    connect(ui->Exit,&QPushButton::clicked,this,&MainWindow::close);
    QList<QTableWidget*> tablewidgets = {ui->tableWidget,ui->tableWidget_4};
    for (QTableWidget* tableWidget : tablewidgets) {
        tableWidget->clearContents();
        tableWidget->setRowCount(0);
        tableWidget->setColumnCount(6);

        tableWidget->setHorizontalHeaderLabels(QStringList()
                                               << "ID" << "Class Name" << "Time" << "Trainer" << "Status" << "Capacity");

        for (const auto& gc : classesmap) {
            int row = tableWidget->rowCount();
            tableWidget->insertRow(row);

            tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(gc->getId())));
            tableWidget->setItem(row, 1, new QTableWidgetItem(gc->getClassName()));
            tableWidget->setItem(row, 2, new QTableWidgetItem(gc->get.toString("hh:mm AP")));
            tableWidget->setItem(row, 3, new QTableWidgetItem(gc->getTrainer()));
            tableWidget->setItem(row, 4, new QTableWidgetItem(gc->getStatus()));
            tableWidget->setItem(row, 5, new QTableWidgetItem(QString::number(gc->getCapacity()-gc->getEnrolled())));
        }
    }
    connect(ui->SortClassesbt,&QPushButton::clicked,this,[=]{
        QStringList filters = {
            ui->lineEdit->text().trimmed(),
            ui->lineEdit_2->text().trimmed(),
            ui->lineEdit_3->text().trimmed(),
            ui->lineEdit_4->text().trimmed(),
            ui->lineEdit_5->text().trimmed()
        };
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(0);

        auto match = [](const QString& filter, const QString& value) {
            return filter.isEmpty() || filter.compare("any", Qt::CaseInsensitive) == 0 || filter == value;
        };

        for (const auto& cls : classesmap) {
            QStringList classData = {
                QString::number(cls->getId()),         // 0 - ID
                cls->getClassName(),                   // 1 - Class Name
                cls->getTime().toString("hh:mm AP"),   // 2 - Time
                cls->getTrainer(),                     // 3 - Trainer
                cls->getStatus(),                      // 4 - Status
                QString::number(cls->getCapacity())    // 5 - Capacity
            };

            bool matched = true;
            for (int i = 0; i < filters.size(); ++i) {
                if (!match(filters[i], classData[i + 1])) { // skip ID column
                    matched = false;
                    break;
                }
            }

            if (matched) {
                int row = tableWidget->rowCount();
                tableWidget->insertRow(row);
                for (int col = 0; col < 6; ++col) {
                    tableWidget->setItem(row, col, new QTableWidgetItem(classData[col]));
                }
            }
        }
    })
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
    ui->EnrollClassBtn->setIcon(QIcon(imagePaths[2]));
    ui->CancelClassBtn->setIcon(QIcon(imagePaths[3]));
    ui->AvailableClassesBtn->setIcon(QIcon(imagePaths[4]));
    ui->WorkoutBtn->setIcon(QIcon(imagePaths[5]));
    ui->PadekCourtBtn->setIcon(QIcon(imagePaths[5]));
    ui->NotifiMemberBtn->setIcon(QIcon(imagePaths[7]));
    ui->Billbtn_3->setIcon(QIcon(imagePaths[8]));
    ui->MemberProfileBtn->setIcon(QIcon(imagePaths[9]));
    ui->LogOutBtn_3->setIcon(QIcon(imagePaths[11]));
    ui->Subscriptionbtn->setIcon(QIcon(imagePaths[29]));

    // Optional: set icon size (applies to all buttons)
    QList<QPushButton*> allButtons = {
        ui->Dashboardbtn, ui->GymMangbtn, ui->TMbtn, ui->Aubtn,ui->AdminBtn, ui->Notibtn,
        ui->Billbtn, ui->ProfileBtn, ui->SettingBtn,
        ui->EnrollClassBtn, ui->CancelClassBtn, ui->AvailableClassesBtn, ui->WorkoutBtn, ui->PadekCourtBtn,
        ui->Subscriptionbtn ,ui->NotifiMemberBtn,ui->Billbtn_3, ui->MemberProfileBtn,
        ui->LogOutBtn_3,ui->LogOutBtn
    };
    // Step 2: Corresponding stacked widget page indexes
    QList<int> pageIndexes = {
        0, 1, 2, 3, 4, 5,
        6, 7, 8,
        9, 10, 11,12, 13,
        14, 15, 16, 17,
        18,18
    };

    // Step 3: Define styles
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

    // Step 4: Assign styles and connect buttons
    for (int i = 0; i < allButtons.size(); ++i) {
        QPushButton* btn = allButtons[i];
        int index = pageIndexes[i];

        btn->setIconSize(QSize(32, 32));
        btn->setStyleSheet(defaultStyle);

        connect(btn, &QPushButton::clicked, this, [=]() {
            // Reset all button styles to default
            for (QPushButton* otherBtn : allButtons) {
                otherBtn->setStyleSheet(defaultStyle);
            }

            // Set clicked button to active style
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
            if(i<9)
                ui->staffMainStackWidget->setCurrentIndex(index);
            else if(i<18)
                ui->stackedWidget_3->setCurrentIndex(index-9);
            else
                ui->FullWiedgit->setCurrentIndex(0);
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


