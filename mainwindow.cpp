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
    FileHandler::loadMembers("G:/cs_project/FullGymProject/members.txt", members, classesmap);
    ui->label_36->setText(QString::number(members.size()));
    FileHandler::loadStaff("G:/cs_project/FullGymProject/staffs.txt", staffMap);
    FileHandler::loadClasses("G:/cs_project/FullGymProject/classes.txt", classesmap, members, staffMap);
    ui->label_38->setText(QString::number(classesmap.size()));
    // int totalCapacity = 0;
    // for (const auto& gc : classesmap) {
    //     totalCapacity += gc->getCapacity();
    // }
    // ui->label_40->setText(QString::number(totalCapacity));
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
                        1, // to StaffPageStack index
                        newImagePath,
                        newExitStyle
                        );
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

    QList<QTableWidget*> tablewidgets = {ui->tableWidget_2,ui->tableWidget_4,ui->tableWidget_3};
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

    connect(ui->SortEnrolledClassesBtn,&QPushButton::clicked,this,[=]{
        QStringList filters = {
            ui->lineEdit_6->text().trimmed(),
            ui->lineEdit_7->text().trimmed(),
            ui->lineEdit_8->text().trimmed(),
            ui->lineEdit_9->text().trimmed(),
            ui->lineEdit_10->text().trimmed()
        };

        ui->tableWidget_2->clearContents();
        ui->tableWidget_2->setRowCount(0);

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

    /////////////// --- Add Users Button Logic ---/////////////////
    connect(ui->addUserBtn, &QPushButton::clicked, this, [=]() {
        QString name = ui->lineEditName->text().trimmed();
        QString email = ui->lineEditEmail->text().trimmed();
        QString password = ui->lineEditPassword->text().trimmed();
        QString ageStr = ui->lineEditAge->text().trimmed();
        QString phone = ui->lineEditPhone->text().trimmed();
        QString address = ui->lineEditAddress->text().trimmed();
        QString role = ui->comboBox->currentText().trimmed().toLower();

        // Basic validation
        if (name.isEmpty() || email.isEmpty() || password.isEmpty() || ageStr.isEmpty() || phone.isEmpty() || address.isEmpty() || role.isEmpty()) {
            QMessageBox::warning(this, "Validation Error", "Please fill in all fields.");
            return;
        }
        bool ok = false;
        int age = ageStr.toInt(&ok);
        if (!ok || age < 16 || age > 100) {
            QMessageBox::warning(this, "Validation Error", "Enter a valid age (16–100).\n");
            return;
        }
        if (role == "manager") {
            QMessageBox::warning(this, "Not Available", "Adding a manager is not available.");
            return;
        }
        if (role == "member") {
            // Append to members.txt
            QFile file("G:/cs_project/FullGymProject/members.txt");
            if (file.open(QIODevice::Append | QIODevice::Text)) {
                QTextStream out(&file);
                int newId = members.size() + 1;
                out << newId << ":" << name << ":" << email << ":" << password << ":male:false:" << phone << ":" << address << ":" << ageStr << "\n";
                file.close();
                // --- ADD TO IN-MEMORY MAP ---
                Member* m = new Member(name, email, password, "male", false, phone, address, age);
                members[newId] = m;
                QMessageBox::information(this, "Success", "Member added successfully.");
            } else {
                QMessageBox::warning(this, "File Error", "Could not open members.txt for writing.");
            }
            ui->lineEditName->clear();
            ui->lineEditEmail->clear();
            ui->lineEditPassword->clear();
            ui->lineEditAge->clear();
            ui->lineEditPhone->clear();
            ui->lineEditAddress->clear();
            ui->comboBox->setCurrentIndex(0);
            return;
        }
        
        QFile file("G:/cs_project/FullGymProject/staffs.txt");
        if (file.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&file);
            int newId = staffMap.size() + 1;
            out << newId << ":" << name << ":" << email << ":" << password << ":" << ageStr << ":" << address << ":" << phone << ":" << role << "\n";
            file.close();
            // --- ADD TO IN-MEMORY MAP ---
            Staff* s = nullptr;
            if (role == "coach") {
                s = new Coach(name, email, password, phone, address, age);
            } else if (role == "receptionis" || role == "receptionist") {
                s = new Receptionist(name, email, password, phone, address, age);
            }
            if (s) {
                staffMap[newId] = s;
                QMessageBox::information(this, "Success", "Staff added successfully.");
            } else {
                QMessageBox::warning(this, "Role Error", "Unknown staff role.");
            }
        } else {
            QMessageBox::warning(this, "File Error", "Could not open staffs.txt for writing.");
        }
        ui->lineEditName->clear();
        ui->lineEditEmail->clear();
        ui->lineEditPassword->clear();
        ui->lineEditAge->clear();
        ui->lineEditPhone->clear();
        ui->lineEditAddress->clear();
        ui->comboBox->setCurrentIndex(0);
    });
    
    
    connect(ui->TMbtn, &QPushButton::clicked, this, [=]() {
        // Only allow access if current staff is a manager
        Manger* manager = dynamic_cast<Manger*>(currStaff);
        if (!manager) {
            QMessageBox::warning(this, "Access Denied", "Only managers can access trainer management.");
            return;
        }

        // Show trainer management page
        ui->staffMainStackWidget->setCurrentIndex(2);  // Index of page_3

        // Show all available classes in the table
        ui->tableWidget_14->clearContents();
        ui->tableWidget_14->setRowCount(0);

        // Display all classes from the existing classesmap
        for (const auto& gc : classesmap) {
            int row = ui->tableWidget_14->rowCount();
            ui->tableWidget_14->insertRow(row);
            ui->tableWidget_14->setItem(row, 0, new QTableWidgetItem(QString::number(gc->getId())));
            ui->tableWidget_14->setItem(row, 1, new QTableWidgetItem(gc->getName()));
            ui->tableWidget_14->setItem(row, 2, new QTableWidgetItem(gc->getTime().toString("hh:mm AP")));
            // Get coach name if assigned
            QString coachName = "Not Assigned";
            if (gc->getCoach()) {
                coachName = gc->getCoach()->getName();
            }
            ui->tableWidget_14->setItem(row, 3, new QTableWidgetItem(coachName));
            ui->tableWidget_14->setItem(row, 4, new QTableWidgetItem(gc->getStatue()));
            ui->tableWidget_14->setItem(row, 5, new QTableWidgetItem(QString::number(gc->getCapacity())));
        }
        // --- Add coaches table population here ---
        ui->tableWidget_15->clearContents();
        ui->tableWidget_15->setRowCount(0);
        ui->tableWidget_15->setColumnCount(7);
        ui->tableWidget_15->setHorizontalHeaderLabels(QStringList()
            << "ID" << "Name" << "Email" << "Phone" << "Address" << "Age" << "Assigned Classes");
        for (auto staff : staffMap) {
            if (staff->getRole().toLower() == "coach") {
                Coach* coach = dynamic_cast<Coach*>(staff);
                if (!coach) continue;
                int row = ui->tableWidget_15->rowCount();
                ui->tableWidget_15->insertRow(row);
                ui->tableWidget_15->setItem(row, 0, new QTableWidgetItem(QString::number(coach->getId())));
                ui->tableWidget_15->setItem(row, 1, new QTableWidgetItem(coach->getName()));
                ui->tableWidget_15->setItem(row, 2, new QTableWidgetItem(coach->getEmail()));
                ui->tableWidget_15->setItem(row, 3, new QTableWidgetItem(coach->getPhone()));
                ui->tableWidget_15->setItem(row, 4, new QTableWidgetItem(coach->getAddress()));
                ui->tableWidget_15->setItem(row, 5, new QTableWidgetItem(QString::number(coach->getAge())));
                // Assigned classes as comma-separated names
                QStringList classNames;
                for (auto gc : coach->getClasses()) {
                    classNames << gc->getName();
                }
                ui->tableWidget_15->setItem(row, 6, new QTableWidgetItem(classNames.join(", ")));
            }
        }
    });

    // Handle assign class button click
    connect(ui->assignClassBtn, &QPushButton::clicked, this, [=]() {
        // Get input values
        QString classIdStr = ui->lineEditClassId->text();
        QString coachIdStr = ui->lineEditCoachId->text();

        // Validate inputs
        bool ok;
        int classId = classIdStr.toInt(&ok);
        if (!ok) {
            QMessageBox::warning(this, "Invalid Input", "Please enter a valid class ID.");
            return;
        }

        int coachId = coachIdStr.toInt(&ok);
        if (!ok) {
            QMessageBox::warning(this, "Invalid Input", "Please enter a valid coach ID.");
            return;
        }

        // Find the class and coach
        if (!classesmap.contains(classId)) {
            QMessageBox::warning(this, "Class Not Found", "The specified class ID does not exist.");
            return;
        }

        if (!staffMap.contains(coachId)) {
            QMessageBox::warning(this, "Coach Not Found", "The specified coach ID does not exist.");
            return;
        }

        GymClass* gymClass = classesmap[classId];
        Staff* staff = staffMap[coachId];

        // Verify the staff member is a coach
        if (staff->getRole().toLower() != "coach") {
            QMessageBox::warning(this, "Invalid Role", "The specified staff member is not a coach.");
            return;
        }

        Coach* coach = dynamic_cast<Coach*>(staff);
        if (!coach) {
            QMessageBox::warning(this, "Error", "Failed to cast staff to coach.");
            return;
        }

        // Assign the class to the coach
        coach->addClass(gymClass);
        gymClass->setCoach(coach);

        // Save changes
        FileHandler::saveStaff("G:/cs_project/FullGymProject/staffs.txt", staffMap);
        FileHandler::saveClasses("G:/cs_project/FullGymProject/classes.txt", classesmap);

        QMessageBox::information(this, "Success", "Class assigned successfully!");
    });

    // Connect member management buttons
    connect(ui->addMemberBtn_2, &QPushButton::clicked, this, &MainWindow::addMember);
    connect(ui->RemoveMemberBtn_3, &QPushButton::clicked, this, &MainWindow::removeMember);

    // Connect class management buttons
    connect(ui->addClassBtn_3, &QPushButton::clicked, this, &MainWindow::addClass);
    connect(ui->RemoveClassBtn_4, &QPushButton::clicked, this, &MainWindow::removeClass);

    // Setup members table
    ui->tableWidget_15->setColumnCount(3);
    ui->tableWidget_15->setHorizontalHeaderLabels(QStringList() << "ID" << "Name" << "Age");
    ui->tableWidget_15->setStyleSheet(R"(
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
    ui->tableWidget_15->setColumnWidth(0, 50);  // ID
    ui->tableWidget_15->setColumnWidth(1, 200); // Name
    ui->tableWidget_15->setColumnWidth(2, 50);  // Age
    
    // Enable alternating row colors
    ui->tableWidget_15->setAlternatingRowColors(true);
    
    // Make the table read-only
    ui->tableWidget_15->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    // Enable selection of entire rows
    ui->tableWidget_15->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    // Enable sorting
    ui->tableWidget_15->setSortingEnabled(true);

    // Initial population of members table
    updateMembersTable();
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
                ui->stackedWidget->setCurrentIndex(index-9);
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

//----------------------------------Class Management----------------------------------

void MainWindow::addClass() {
    QString className = ui->ClassName_3->text().trimmed();
    QString ClassID = ui->ClassID_30->text().trimmed();
    QString capacityStr = ui->Class_capacity_31->text().trimmed();
    QString ClassTime = ui->Class_Time_32->text().trimmed();

    if (className.isEmpty() || ClassID.isEmpty() || capacityStr.isEmpty() || ClassTime.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please fill in all fields.");
        return;
    }

    bool ok;
    int capacity = capacityStr.toInt(&ok);
    if (!ok || capacity <= 0) {
        QMessageBox::warning(this, "Input Error", "Capacity must be a positive integer greater than 0.");
        return;
    }

    // Validate time format (HH:MM AM/PM)
    QTime time = QTime::fromString(ClassTime, "hh:mm AP");
    if (!time.isValid()) {
        QMessageBox::warning(this, "Input Error", "Please enter time in format HH:MM AM/PM (e.g., 09:30 AM)");
        return;
    }

    if (classesmap.contains(ClassID.toInt())) {
        QMessageBox::warning(this, "Duplicate ID", "A class with this ID already exists.");
        return;
    }

    // Create new class with "Open" status
    GymClass* newClass = new GymClass(className, "Open", capacity);
    newClass->setId(ClassID.toInt());
    newClass->setTime(time);
    newClass->setEnrolled(0); // Initialize enrolled count to 0
    
    classesmap[ClassID.toInt()] = newClass;

    // Update the classes table
    updateClassesTable();

    // Save to file
    FileHandler::saveClasses("G:/cs_project/FullGymProject/classes.txt", classesmap);

    QMessageBox::information(this, "Success", "Class added successfully!");
    
    ui->ClassName_3->clear();
    ui->ClassID_30->clear();
    ui->Class_capacity_31->clear();
    ui->Class_Time_32->clear();
}

void MainWindow::removeClass() {
    QString ClassID = ui->ClassID_30->text().trimmed();

    if(ClassID.isEmpty()){
        QMessageBox::warning(this, "Input Error", "Please enter a class ID.");
        return;
    }

    if(!classesmap.contains(ClassID.toInt())){
        QMessageBox::warning(this, "Class Not Found", "A class with this ID does not exist.");
        return;
    }

    GymClass* classToRemove = classesmap[ClassID.toInt()];
    classesmap.remove(ClassID.toInt());

    FileHandler::saveClasses("G:/cs_project/FullGymProject/classes.txt", classesmap);

    QMessageBox::information(this, "Success", "Class removed successfully!");

    ui->ClassID_30->clear();
}

void MainWindow::updateClassesTable() {
    ui->tableWidget_2->clearContents();
    ui->tableWidget_2->setRowCount(0);
    ui->tableWidget_2->setColumnCount(7);
    ui->tableWidget_2->setHorizontalHeaderLabels(QStringList()
        << "ID" << "Class Name" << "Time" << "Status" << "Capacity" << "Enrolled" << "Coach");

    // Set column widths for better readability
    ui->tableWidget_2->setColumnWidth(0, 50);   // ID
    ui->tableWidget_2->setColumnWidth(1, 150);  // Class Name
    ui->tableWidget_2->setColumnWidth(2, 100);  // Time
    ui->tableWidget_2->setColumnWidth(3, 100);  // Status
    ui->tableWidget_2->setColumnWidth(4, 80);   // Capacity
    ui->tableWidget_2->setColumnWidth(5, 80);   // Enrolled
    ui->tableWidget_2->setColumnWidth(6, 150);  // Coach

    // Enable alternating row colors
    ui->tableWidget_2->setAlternatingRowColors(true);
    
    // Make the table read-only
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    // Enable selection of entire rows
    ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    // Enable sorting
    ui->tableWidget_2->setSortingEnabled(true);

    for (auto it = classesmap.begin(); it != classesmap.end(); ++it) {
        int row = ui->tableWidget_2->rowCount();
        ui->tableWidget_2->insertRow(row);
        
        // Add ID
        ui->tableWidget_2->setItem(row, 0, new QTableWidgetItem(QString::number(it.key())));
        // Add Name
        ui->tableWidget_2->setItem(row, 1, new QTableWidgetItem(it.value()->getName()));
        // Add Time
        ui->tableWidget_2->setItem(row, 2, new QTableWidgetItem(it.value()->getTime().toString("hh:mm AP")));
        // Add Status
        ui->tableWidget_2->setItem(row, 3, new QTableWidgetItem(it.value()->getStatue()));
        // Add Capacity
        ui->tableWidget_2->setItem(row, 4, new QTableWidgetItem(QString::number(it.value()->getCapacity())));
        // Add Enrolled
        ui->tableWidget_2->setItem(row, 5, new QTableWidgetItem(QString::number(it.value()->getEnrolled())));
        // Add Coach
        QString coachName = it.value()->getCoach() ? it.value()->getCoach()->getName() : "Not Assigned";
        ui->tableWidget_2->setItem(row, 6, new QTableWidgetItem(coachName));
    }

    // Apply stylesheet for better appearance
    ui->tableWidget_2->setStyleSheet(R"(
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
}
//----------------------------------Member Management----------------------------------

void MainWindow::updateMembersTable() {
    ui->tableWidget_15->clearContents();
    ui->tableWidget_15->setRowCount(0);

    for (auto it = members.begin(); it != members.end(); ++it) {
        int row = ui->tableWidget_15->rowCount();
        ui->tableWidget_15->insertRow(row);
        
        // Add ID
        ui->tableWidget_15->setItem(row, 0, new QTableWidgetItem(QString::number(it.key())));
        // Add Name
        ui->tableWidget_15->setItem(row, 1, new QTableWidgetItem(it.value()->getName()));
        // Add Age
        ui->tableWidget_15->setItem(row, 2, new QTableWidgetItem(QString::number(it.value()->getAge())));
    }
}

void MainWindow::addMember() {
    QString name = ui->MemberName_2->text().trimmed();
    QString email = ui->MemberEmail_2->text().trimmed();
    QString password = ui->MemberPassword_2->text().trimmed();
    QString ageStr = ui->MememberAge_2->text().trimmed();
    QString phone = ui->MemberPhone_2->text().trimmed();
    QString address = ui->MemberAddress_2->text().trimmed();
    bool isMale = ui->radioButton->isChecked();
    bool isFemale = ui->radioButton_2->isChecked();

    // Basic validation
    if (name.isEmpty() || name.length() < 3) {
        QMessageBox::warning(this, "Validation Error", "Name must be at least 3 characters.");
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

    QString gender = isMale ? "male" : "female";

    // Check if email already exists
    for (const auto& member : members) {
        if (member->getEmail() == email) {
            QMessageBox::warning(this, "Duplicate Email", "A member with this email already exists.");
            return;
        }
    }

    // Create new member
    int newId = members.size() + 1;
    Member* newMember = new Member(name, email, password, gender, false, phone, address, age);
    members[newId] = newMember;

    // Save to file
    FileHandler::saveMembers("G:/cs_project/FullGymProject/members.txt", members);

    // Update the members table
    updateMembersTable();

    // Clear input fields
    ui->MemberName_2->clear();
    ui->MemberEmail_2->clear();
    ui->MemberPassword_2->clear();
    ui->MememberAge_2->clear();
    ui->MemberPhone_2->clear();
    ui->MemberAddress_2->clear();
    ui->radioButton->setChecked(false);
    ui->radioButton_2->setChecked(false);

    QMessageBox::information(this, "Success", "Member added successfully!");
}

void MainWindow::removeMember() {
    QString email = ui->MemberEmail_2->text().trimmed();
    QString password = ui->MemberPassword_2->text().trimmed();

    if (email.isEmpty() && password.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter both email and password to remove a member.");
        return;
    }

    // Find member by email and verify password
    int memberIdToRemove = -1;
    for (auto it = members.begin(); it != members.end(); ++it) {
        if (it.value()->getEmail() == email && it.value()->getPassword() == password) {
            memberIdToRemove = it.key();
            break;
        }
    }

    if (memberIdToRemove == -1) {
        QMessageBox::warning(this, "Authentication Failed", "Invalid email or password combination.");
        return;
    }

    // Confirm deletion
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm Deletion",
        "Are you sure you want to remove this member?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // Remove member from all classes
        Member* memberToRemove = members[memberIdToRemove];
        for (auto gc : memberToRemove->getClasses()) {
            gc->removeMember(memberToRemove);
            gc->setEnrolled(gc->getEnrolled() - 1);
        }

        // Remove member from members map
        delete members[memberIdToRemove];
        members.remove(memberIdToRemove);

        // Save changes to files
        FileHandler::saveMembers("G:/cs_project/FullGymProject/members.txt", members);
        FileHandler::saveClasses("G:/cs_project/FullGymProject/classes.txt", classesmap);

        // Update the members table
        updateMembersTable();

        // Clear input fields
        ui->MemberEmail_2->clear();
        ui->MemberPassword_2->clear();
        ui->MemberName_2->clear();
        ui->MememberAge_2->clear();
        ui->MemberPhone_2->clear();
        ui->MemberAddress_2->clear();
        ui->radioButton->setChecked(false);
        ui->radioButton_2->setChecked(false);

        QMessageBox::information(this, "Success", "Member removed successfully!");
    }
}

