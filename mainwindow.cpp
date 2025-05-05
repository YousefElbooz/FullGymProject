// mainwindow.cpp (complete + organized)
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "FileHandler.h"
#include "animations.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->setWindowFlag(Qt::FramelessWindowHint);
    setFixedSize(1355,720);
    this->setAttribute(Qt::WA_TranslucentBackground);
    ui->setupUi(this);

    Animations* animations = new Animations(this);
    animations->setUI(ui->LoginPageStackedWidget, ui->label, ui->Exit);
    Animations* pageAnimator = new Animations(this);
    pageAnimator->setUI(ui->FullWiedgit, ui->label, ui->Exit);
    ui->FullWiedgit->setCurrentIndex(1);
    this->showMaximized();
    ui->FullWiedgit->setCurrentIndex(0);
    ui->LoginPageStackedWidget->setCurrentIndex(0);
    ui->staffMainStackWidget->setCurrentIndex(1);
    setPixmapForWidgets();
    FileHandler::loadMembers("C:/Users/Yousef/Documents/FullGymProject/FullGymProject/members.txt", members, classesmap);
    FileHandler::loadStaff("C:/Users/Yousef/Documents/FullGymProject/FullGymProject/staffs.txt", staffMap);
    FileHandler::loadClasses("C:/Users/Yousef/Documents/FullGymProject/FullGymProject/classes.txt", classesmap, members);
    // Move updateEnrolledClassesTable here so it is in scope for all later code
    auto updateEnrolledClassesTable = [=]() {
        ui->tableWidget_3->clearContents();
        ui->tableWidget_3->setRowCount(0);
        ui->tableWidget_3->setColumnCount(6);
        ui->tableWidget_3->setHorizontalHeaderLabels(QStringList()
            << "ID" << "Class Name" << "Time" << "Trainer" << "Status" << "Capacity");

        for (const auto& gc : currMember->getClasses()) {
            int row = ui->tableWidget_3->rowCount();
            ui->tableWidget_3->insertRow(row);

            ui->tableWidget_3->setItem(row, 0, new QTableWidgetItem(QString::number(gc->getId())));
            ui->tableWidget_3->setItem(row, 1, new QTableWidgetItem(gc->getName()));
            ui->tableWidget_3->setItem(row, 2, new QTableWidgetItem(gc->getTime().toString("hh:mm AP")));
            QString coachName = gc->getCoach() ? gc->getCoach()->getName() : "Unknown";
            ui->tableWidget_3->setItem(row, 3, new QTableWidgetItem(coachName));
            ui->tableWidget_3->setItem(row, 4, new QTableWidgetItem(gc->getStatue()));
            ui->tableWidget_3->setItem(row, 5, new QTableWidgetItem(QString::number(gc->getCapacity() - gc->getEnrolled())));
        }
    };
    connect(ui->loginbtn, &QPushButton::clicked, this, [=] {
        QString usrEmail = ui->LineEditEmail->text();
        QString usrPassword = ui->LineEditPassword->text();
        bool loggedIn = false;

        for (auto mem : members) {
            if (mem->getEmail() == usrEmail && mem->getPassword() == usrPassword) {
                currMember = mem;
                updateEnrolledClassesTable();
                QMessageBox::information(this, "Success", "You have logged in successfully.");
                QString newImagePath = ":/img/images/member-background.png"; // Change to your target background if needed
                QString newExitStyle = R"(
                        QPushButton {
                            background-color: #008FC1;
                            color: white;
                            border-radius: 10px;
                            padding: 6px 12px;
                            font: bold 10pt "Yeasty Flavors";
                        }
                    )";
                pageAnimator->animatedSwitchAdvanced(
                    ui->FullWiedgit->currentIndex(), // from LoginPageStack
                    2, // to MemberPageStack index
                    newImagePath,
                    newExitStyle
                    );
                loggedIn = true;
                break;  // Stop checking further
            }
        }

        if(!loggedIn){
            for(auto stf: staffMap){
                if(stf->getEmail()==usrEmail&&stf->getPassword()==usrPassword){
                    currStaff = stf;
                    QMessageBox::information(this, "Success", "You have logged in successfully.");
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
        Member * m = new Member(username,email,password,gender,false,phone,address,ageStr.toInt());
        currMember = m;
        members[m->getId()]=m;
        QMessageBox::information(this, "Success", "Sign-up completed successfully.");
    });

    connect(ui->toggleButton, &QPushButton::clicked, this, [=]() {
        QString newStyle = R"(
        QPushButton {
            background-color:#008FC1;
            padding: 10px;
            color: rgb(0, 0, 0);
            font: 9pt "Yeasty Flavors";
            border: none;
            border-radius: 25px;
        }
        QPushButton:hover{
            background-color: rgb(233, 222, 203);
            border-radius:25px;
            border:1px solid #008FC1;
            color: #008FC1;
            border-color:#008FC1;
        }
    )";
        animations->animatedSwitchAdvanced(0, 1, ":/img/images/newpadelrounded.png", newStyle);
    });

    connect(ui->toggleButton_2, &QPushButton::clicked, this, [=]() {
        QString newStyle = R"(
        QPushButton {
            background-color:rgb(198, 143, 59);
            padding: 10px;
            color: rgb(0, 0, 0);
            font: 9pt "Yeasty Flavors";
            border: none;
            border-radius: 25px;
        }
        QPushButton:hover{
            background-color: rgb(233, 222, 203);
            border-radius:25px;
            border:1px solid rgb(198, 143, 59);
            color: rgb(198, 143, 59);
            border-color: rgb(198, 143, 59);
        }
    )";
        animations->animatedSwitchAdvanced(1, 0, ":/img/images/newGymrounded.png", newStyle);
    });
    connect(ui->Exit,&QPushButton::clicked,this,&MainWindow::close);

    QList<QTableWidget*> tablewidgets = {ui->tableWidget,ui->tableWidget_4,ui->tableWidget_3,ui->tableWidget_2};
    for (auto tableWidget : tablewidgets) {
        tableWidget->clearContents();
        tableWidget->setRowCount(0);
        tableWidget->setColumnCount(6);

        tableWidget->setHorizontalHeaderLabels(QStringList()
                                               << "ID" << "Class Name" << "Time" << "Trainer" << "Status" << "Capacity");

        for (auto gc : classesmap) {
            if (!gc) continue; // safety check

            int row = tableWidget->rowCount();
            tableWidget->insertRow(row);

            QString trainer = gc->getCoach() ? gc->getCoach()->getName() : "Unassigned";

            tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(gc->getId())));
            tableWidget->setItem(row, 1, new QTableWidgetItem(gc->getName()));
            tableWidget->setItem(row, 2, new QTableWidgetItem(gc->getTime().toString("hh:mm AP")));
            QString coachName = gc->getCoach() ? gc->getCoach()->getName() : "Unknown";
            tableWidget->setItem(row, 3, new QTableWidgetItem(coachName));
            tableWidget->setItem(row, 4, new QTableWidgetItem(gc->getStatue()));
            tableWidget->setItem(row, 5, new QTableWidgetItem(QString::number(gc->getCapacity() - gc->getEnrolled())));
        }
    }

    connect(ui->SortClassesbt,&QPushButton::clicked,this,[=]{
        QStringList filters = {
            ui->lineEdit_6->text().trimmed(),
            ui->lineEdit_7->text().trimmed(),
            ui->lineEdit_8->text().trimmed(),
            ui->lineEdit_9->text().trimmed(),
            ui->lineEdit_10->text().trimmed()
        };

        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(0);

        auto match = [](const QString& filter, const QString& value) {
            return filter.isEmpty() || filter.compare("any", Qt::CaseInsensitive) == 0 || filter == value;
        };

        for (auto cls : classesmap) {
            QStringList classData = {
                QString::number(cls->getId()),                   // 0 - ID
                cls->getName(),                                  // 1 - Class Name
                cls->getTime().toString("hh:mm AP"),             // 2 - Time
                cls->getCoach() ? cls->getCoach()->getName() : "Unknown", // 3 - Trainer
                cls->getStatue(),                                // 4 - Status
                QString::number(cls->getCapacity())              // 5 - Capacity
            };

            bool matched = true;
            for (int i = 0; i < filters.size(); ++i) {
                if (!match(filters[i], classData[i + 1])) { // skip ID column
                    matched = false;
                    break;
                }
            }

            if (matched) {
                int row = ui->tableWidget_2->rowCount();
                ui->tableWidget_2->insertRow(row);
                for (int col = 0; col < 6; ++col) {
                    ui->tableWidget_2->setItem(row, col, new QTableWidgetItem(classData[col]));
                }
            }
        }
    });

    // Variable to store the selected class ID
    int selectedClassId = -1;

    // Connect cell click ONCE (after UI setup)
    connect(ui->tableWidget_2, &QTableWidget::cellClicked, this, [=, &selectedClassId](int row, int column) {
        selectedClassId = ui->tableWidget_2->item(row, 0)->text().toInt();
    });

    ///////////// Enroll button section //////////////
    connect(ui->EnrollClassBtn, &QPushButton::clicked, this, [=]() {
        // Show available classes table
        ui->tableWidget_2->clearContents();
        ui->tableWidget_2->setRowCount(0);
        ui->tableWidget_2->setColumnCount(6);
        ui->tableWidget_2->setHorizontalHeaderLabels(QStringList()
            << "ID" << "Class Name" << "Time" << "Trainer" << "Status" << "Capacity");

        // Populate available classes
        for (const auto& gc : classesmap) {
            if (gc->getEnrolled() < gc->getCapacity() && 
                !currMember->getClasses().contains(gc)) {
                int row = ui->tableWidget_2->rowCount();
                ui->tableWidget_2->insertRow(row);
                ui->tableWidget_2->setItem(row, 0, new QTableWidgetItem(QString::number(gc->getId())));
                ui->tableWidget_2->setItem(row, 1, new QTableWidgetItem(gc->getName()));
                ui->tableWidget_2->setItem(row, 2, new QTableWidgetItem(gc->getTime().toString("hh:mm AP")));
                QString coachName = gc->getCoach() ? gc->getCoach()->getName() : "Unknown";
                ui->tableWidget_2->setItem(row, 3, new QTableWidgetItem(coachName));
                ui->tableWidget_2->setItem(row, 4, new QTableWidgetItem(gc->getStatue()));
                ui->tableWidget_2->setItem(row, 5, new QTableWidgetItem(QString::number(gc->getCapacity() - gc->getEnrolled())));
            }
        }
    });

    // Connect the Request Class button to perform enrollment
    connect(ui->requestClassbtn, &QPushButton::clicked, this, [=]() {
        QString requestedClassName = ui->lineEditClassNameRequest->text().trimmed();
        if (requestedClassName.isEmpty()) {
            QMessageBox::warning(this, "No Class Name", "Please enter a class name to request.");
            return;
        }

        GymClass* selectedClass = nullptr;
        for (const auto& gc : classesmap) {
            if (gc->getName().compare(requestedClassName, Qt::CaseInsensitive) == 0) {
                selectedClass = gc;
                break;
            }
        }

        if (!selectedClass) {
            QMessageBox::warning(this, "Class Not Found", "No class found with that name.");
            return;
        }

        if (selectedClass->getEnrolled() >= selectedClass->getCapacity()) {
            QMessageBox::warning(this, "Cannot Enroll", "Class is full.");
            return;
        }
        if (currMember->getClasses().contains(selectedClass)) {
            QMessageBox::warning(this, "Cannot Enroll", "You are already enrolled in this class.");
            return;
        }

        selectedClass->addMember(currMember);
        selectedClass->setEnrolled(selectedClass->getEnrolled() + 1);
        updateEnrolledClassesTable();
        QMessageBox::information(this, "Success", "Successfully enrolled in " + selectedClass->getName());

        ui->lineEditClassNameRequest->clear();
    });

    // Cancel Class logic
    connect(ui->CancelClassBtn, &QPushButton::clicked, this, [=]() {
        // Show member's enrolled classes in tableWidget_5
        ui->tableWidget_5->clearContents();
        ui->tableWidget_5->setRowCount(0);
        ui->tableWidget_5->setColumnCount(6);
        ui->tableWidget_5->setHorizontalHeaderLabels(QStringList()
            << "ID" << "Class Name" << "Time" << "Trainer" << "Status" << "Capacity");
        
        // Set table styling to match enrolled window
        ui->tableWidget_5->setStyleSheet(R"(
            QTableWidget {
                background-color: white;
                alternate-background-color: #f6f6f6;
                gridline-color: #d3d3d3;
                border: 1px solid #d3d3d3;
                border-radius: 5px;
            }
            QTableWidget::item {
                padding: 5px;
                border-bottom: 1px solid #d3d3d3;
            }
            QHeaderView::section {
                background-color: #f1c27d;
                color: black;
                padding: 5px;
                border: 1px solid #d3d3d3;
                font-weight: bold;
            }
            QTableWidget::item:selected {
                background-color: #f1c27d;
                color: black;
            }
        )");
        
        // Set column widths
        ui->tableWidget_5->setColumnWidth(0, 50);  // ID
        ui->tableWidget_5->setColumnWidth(1, 200); // Class Name
        ui->tableWidget_5->setColumnWidth(2, 100); // Time
        ui->tableWidget_5->setColumnWidth(3, 150); // Trainer
        ui->tableWidget_5->setColumnWidth(4, 100); // Status
        ui->tableWidget_5->setColumnWidth(5, 100); // Capacity
        
        // Enable alternating row colors
        ui->tableWidget_5->setAlternatingRowColors(true);
        
        // Make the table read-only
        ui->tableWidget_5->setEditTriggers(QAbstractItemView::NoEditTriggers);
        
        // Enable selection of entire rows
        ui->tableWidget_5->setSelectionBehavior(QAbstractItemView::SelectRows);
        
        // Enable sorting
        ui->tableWidget_5->setSortingEnabled(true);

        for (const auto& gc : currMember->getClasses()) {
            int row = ui->tableWidget_5->rowCount();
            ui->tableWidget_5->insertRow(row);
            ui->tableWidget_5->setItem(row, 0, new QTableWidgetItem(QString::number(gc->getId())));
            ui->tableWidget_5->setItem(row, 1, new QTableWidgetItem(gc->getName()));
            ui->tableWidget_5->setItem(row, 2, new QTableWidgetItem(gc->getTime().toString("hh:mm AP")));
            QString coachName = gc->getCoach() ? gc->getCoach()->getName() : "Unknown";
            ui->tableWidget_5->setItem(row, 3, new QTableWidgetItem(coachName));
            ui->tableWidget_5->setItem(row, 4, new QTableWidgetItem(gc->getStatue()));
            ui->tableWidget_5->setItem(row, 5, new QTableWidgetItem(QString::number(gc->getCapacity() - gc->getEnrolled())));
        }
    });

    ////////////////Cancel button secton //////////////////

    connect(ui->CancelClassConfirmBtn, &QPushButton::clicked, this, [=]() {
        QString classIdStr = ui->lineEditCancelClassId->text().trimmed();
        QString className = ui->lineEditCancelClassName->text().trimmed();
        if (classIdStr.isEmpty() || className.isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Please enter both class ID and class name.");
            return;
        }
        bool ok = false;
        int classId = classIdStr.toInt(&ok);
        if (!ok) {
            QMessageBox::warning(this, "Input Error", "Class ID must be a number.");
            return;
        }
        GymClass* foundClass = nullptr;
        for (auto gc : currMember->getClasses()) {
            if (gc->getId() == classId && gc->getName().compare(className, Qt::CaseInsensitive) == 0) {
                foundClass = gc;
                break;
            }
        }
        if (!foundClass) {
            QMessageBox::warning(this, "Not Enrolled", "You are not enrolled in this class.");
            return;
        }
        // Remove from member's classes
        currMember->removeClass(foundClass);
        foundClass->removeMember(currMember);
        // Update enrolled count
        foundClass->setEnrolled(foundClass->getEnrolled() - 1);
        // Save changes
        FileHandler::saveMembers("G:/cs_project/FullGymProject/members.txt", members);
        FileHandler::saveClasses("G:/cs_project/FullGymProject/classes.txt", classesmap);
        // Update UI
        updateEnrolledClassesTable();
        ui->tableWidget_5->clearContents();
        ui->tableWidget_5->setRowCount(0);
        for (const auto& gc : currMember->getClasses()) {
            int row = ui->tableWidget_5->rowCount();
            ui->tableWidget_5->insertRow(row);
            ui->tableWidget_5->setItem(row, 0, new QTableWidgetItem(QString::number(gc->getId())));
            ui->tableWidget_5->setItem(row, 1, new QTableWidgetItem(gc->getName()));
            ui->tableWidget_5->setItem(row, 2, new QTableWidgetItem(gc->getTime().toString("hh:mm AP")));
            QString coachName = gc->getCoach() ? gc->getCoach()->getName() : "Unknown";
            ui->tableWidget_5->setItem(row, 3, new QTableWidgetItem(coachName));
            ui->tableWidget_5->setItem(row, 4, new QTableWidgetItem(gc->getStatue()));
            ui->tableWidget_5->setItem(row, 5, new QTableWidgetItem(QString::number(gc->getCapacity() - gc->getEnrolled())));
        }
        QMessageBox::information(this, "Success", "You have successfully canceled the class.");
        ui->lineEditCancelClassId->clear();
        ui->lineEditCancelClassName->clear();
    });

    connect(ui->AvailableClassesBtn, &QPushButton::clicked, this, [=]() {
        int availableClassesPageIndex = ui->stackedWidget->count() - 1;
        ui->stackedWidget->setCurrentIndex(availableClassesPageIndex); // Switch to Available Classes page
        ui->tableWidget_6->clearContents();
        ui->tableWidget_6->setRowCount(0);
        ui->tableWidget_6->setColumnCount(6);
        ui->tableWidget_6->setHorizontalHeaderLabels(QStringList()
            << "ID" << "Class Name" << "Time" << "Trainer" << "Status" << "Capacity");
        for (const auto& gc : classesmap) {
            int row = ui->tableWidget_6->rowCount();
            ui->tableWidget_6->insertRow(row);
            ui->tableWidget_6->setItem(row, 0, new QTableWidgetItem(QString::number(gc->getId())));
            ui->tableWidget_6->setItem(row, 1, new QTableWidgetItem(gc->getName()));
            ui->tableWidget_6->setItem(row, 2, new QTableWidgetItem(gc->getTime().toString("hh:mm AP")));
            QString coachName = gc->getCoach() ? gc->getCoach()->getName() : "Unknown";
            ui->tableWidget_6->setItem(row, 3, new QTableWidgetItem(coachName));
            ui->tableWidget_6->setItem(row, 4, new QTableWidgetItem(gc->getStatue()));
            ui->tableWidget_6->setItem(row, 5, new QTableWidgetItem(QString::number(gc->getCapacity() - gc->getEnrolled())));
        }
    });
}

MainWindow::~MainWindow() {
    delete ui;
    FileHandler::saveMembers("G:/cs_project/FullGymProject/members.txt", members);
    FileHandler::saveStaff("G:/cs_project/FullGymProject/staffs.txt", staffMap);
    FileHandler::saveClasses("G:/cs_project/FullGymProject/classes.txt", classesmap);
    qDeleteAll(members);
    qDeleteAll(staffMap);
    qDeleteAll(classesmap);
}


void MainWindow::setPixmapForWidgets() {
    QString imagePaths[] = {
        ":/img/images/newGymrounded.png", // 0 - label image (skip or use elsewhere)
        ":/img/images/logo.png", // 1 - logo (still used)
        ":/icons/images/icons/dashboard.svg", // 2
        ":/icons/images/icons/gymDumble.svg", // 3
        ":/icons/images/icons/teamMangment.svg", // 4
        ":/icons/images/icons/add-user.svg", // 5
        ":/icons/images/icons/user-admin.svg", // 6
        ":/icons/images/icons/notification.svg", // 7
        ":/icons/images/icons/bill.svg", // 8
        ":/icons/images/icons/profile.svg", // 9
        ":/icons/images/icons/settings.svg", // 10
        ":/icons/images/icons/logout.svg", // 11
        ":/icons/images/icons/chart.svg", // 12
        ":/icons/images/icons/enter.svg", // 13
        ":/icons/images/icons/cancel.svg", // 14
        ":/icons/images/icons/calendar.svg", // 15
        ":/icons/images/icons/home-workouts.svg", // 16
        ":/icons/images/icons/auto-renew.svg", // 17
        ":/icons/images/icons/court-playground.svg",        // 18
        ":/icons/images/icons/user-admin.svg",     // 19
        ":/icons/images/icons/sand-clock.svg",     // 20
        ":/icons/images/icons/stretching.svg",     //21
        ":/icons/images/icons/coach.svg", //22
        ":/icons/images/icons/money.svg",      //23
        ":/icons/images/icons/chart.svg",       //24
        ":/icons/images/icons/enter.svg",                  //25
        ":/icons/images/icons/cancel.svg",           //26
        ":/icons/images/icons/calendar.svg",   //27
        ":/icons/images/icons/home-workouts.svg",      //28
        ":/icons/images/icons/auto-renew.svg",     //29
        ":/icons/images/icons/court-playground.svg"            //30
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
    ui->CancelClassBtn->setIcon(QIcon(imagePaths[14]));
    ui->AvailableClassesBtn->setIcon(QIcon(imagePaths[4]));
    ui->WorkoutBtn->setIcon(QIcon(imagePaths[16]));
    ui->PadekCourtBtn->setIcon(QIcon(imagePaths[18]));
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
        font: 12pt "Yeasty Flavors";
        text-align: left;
        border-radius: 5px;
        padding-left:10px;
    }
    QPushButton:hover {

        border-radius: 5px;
        border: 2px solid #f1c27d;
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
                color: black;
                font: 18pt "Yeasty Flavors";
                text-align: left;
                background-color: #f1c27d;
                padding-left:10px;
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
            else{
                Animations* pageAnimator = new Animations(this);
                pageAnimator->setUI(ui->FullWiedgit, ui->label, ui->Exit);
                pageAnimator->animatedSwitchAdvanced(
                    ui->FullWiedgit->currentIndex(),
                    0,
                    ":/img/images/newGymrounded.png",
                    R"(
                QPushButton {
                    background-color: rgb(198, 143, 59);
                    color: black;
                    border-radius: 10px;
                    font: bold 10pt "Yeasty Flavors";
                    padding: 6px 12px;
                }
                )");
                ui->LogOutBtn_3->setStyleSheet(defaultStyle);
            }
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


