// mainwindow.cpp (complete + organized)
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "FileHandler.h"
#include "animations.h"
#include <QFileDialog>
#include <QDir>
#include <QCoreApplication>
#include <QFileInfo>
#include <QPainter>
#include <QBitmap>
#include <QPainterPath>
#include <QInputDialog>
#include <QStack>
#include <algorithm>
#include "CourtLoader.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->setWindowFlag(Qt::FramelessWindowHint);
    setFixedSize(1355,720);
    this->setAttribute(Qt::WA_TranslucentBackground);
    ui->setupUi(this);

    // Initialize animation objects as member variables
    animations = new Animations(this);
    animations->setUI(ui->LoginPageStackedWidget, ui->label, ui->Exit);
    
    pageAnimator = new Animations(this);
    pageAnimator->setUI(ui->FullWiedgit, ui->label, ui->Exit);

    try {
        ui->FullWiedgit->setCurrentIndex(1);
        ui->FullWiedgit->setCurrentIndex(0);
        ui->LoginPageStackedWidget->setCurrentIndex(0);
        
        // Important: Make sure staff UI is initialized to a safe state
        if (ui->staffMainStackWidget) {
            ui->staffMainStackWidget->setCurrentIndex(0);  // Set to dashboard (safer default)
        }
        
        setPixmapForWidgets();
        
        // Load data with safety checks
        FileHandler::loadMembers("D:/temp windows/New folder/New folder/FullGymProject/members.txt", members, classesmap);
        if (ui->label_36) {
            ui->label_36->setText(QString::number(members.size()));
        }
        
        FileHandler::loadStaff("D:/temp windows/New folder/New folder/FullGymProject/staffs.txt", staffMap);
        updateTrainerCount();
        
        FileHandler::loadClasses("D:/temp windows/New folder/New folder/FullGymProject/classes.txt", classesmap, members, staffMap);
        if (ui->label_38) {
            ui->label_38->setText(QString::number(classesmap.size()));
        }
        
        updateCapacity();
        
        if (ui->stackedWidgetProfile) {
            ui->stackedWidgetProfile->setCurrentIndex(0);
        }
        
        // Apply the consistent style to all table widgets
        QList<QTableWidget*> allTables = findChildren<QTableWidget*>();
        for (QTableWidget* table : allTables) {
            applyTableStyle(table);
        }
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Initialization Error", QString("Error during initialization: %1").arg(e.what()));
    }
    catch (...) {
        QMessageBox::critical(this, "Initialization Error", "An unexpected error occurred during application initialization.");
    }

    // Set up subscription button styles
    if (ui->subBtn1m) {
        ui->subBtn1m->setText("1 Month Plan\n$50.00");
    }
    if (ui->subBtn3m) {
        ui->subBtn3m->setText("3 Month Plan\n$130.00\nSave $20");
    }
    if (ui->subBtn6m) {
        ui->subBtn6m->setText("6 Month Plan\n$250.00\nSave $50");
    }
    if (ui->subBtn12m) {
        ui->subBtn12m->setText("12 Month Plan\n$480.00\nSave $120");
    }
    ui->FullWiedgit->setCurrentIndex(1);
    this->showMaximized();
    ui->FullWiedgit->setCurrentIndex(0);
    ui->LoginPageStackedWidget->setCurrentIndex(0);
    ui->staffMainStackWidget->setCurrentIndex(1);
    setPixmapForWidgets();
    FileHandler::loadMembers("G:/cs_project/ahhhhhhhhh/FullGymProject/members.txt", members, classesmap);
    ui->label_36->setText(QString::number(members.size()));
    FileHandler::loadStaff("G:/cs_project/ahhhhhhhhh/FullGymProject/staffs.txt", staffMap);
    updateTrainerCount();
    FileHandler::loadClasses("G:/cs_project/ahhhhhhhhh/FullGymProject/classes.txt", classesmap, members, staffMap);
    ui->label_38->setText(QString::number(classesmap.size()));
    updateCapacity();

    // Load padel bookings from members.txt
    QFile file("G:/cs_project/ahhhhhhhhh/FullGymProject/members.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        while (!file.atEnd()) {
            QString line = file.readLine().trimmed();
            if (line.isEmpty()) continue;
            
            QStringList parts = line.split(":");
            if (parts.size() >= 9) {
                int memberId = parts[0].toInt();
                QString nextLine = file.readLine().trimmed();
                if (nextLine.startsWith("Padel:")) {
                    QStringList bookingParts = nextLine.mid(6).split("|");
                    if (bookingParts.size() == 3) {
                        QString location = bookingParts[0].trimmed().toLower();
                        QString date = bookingParts[1].trimmed();
                        QString time = bookingParts[2].trimmed();
                        
                        // Initialize slots for this location and date if not exists
                        if (!padelBookings.contains(location)) {
                            padelBookings[location] = QMap<QString, QStringList>();
                        }
                        if (!padelBookings[location].contains(date)) {
                            padelBookings[location][date] = QStringList{"0", "0", "0", "0"};
                        }
                        
                        // Find the slot index
                        QStringList slotLabels = {"03:00", "05:00", "07:00", "09:00"};
                        int slotIndex = slotLabels.indexOf(time);
                        if (slotIndex != -1) {
                            padelBookings[location][date][slotIndex] = QString::number(memberId);
                        }
                    }
                }
            }
        }
        file.close();
    }

    // Load courts
    courts = CourtLoader::loadCourtsFromFile("G:/cs_project/ahhhhhhhhh/FullGymProject/courts.txt");

    // Connect Padel Court button to display courts
    connect(ui->PadekCourtBtn, &QPushButton::clicked, this, [=]() {
        // Clear and setup the table
        ui->widget_55->clearContents();
        ui->widget_55->setRowCount(0);
        ui->widget_55->setColumnCount(5);
        ui->widget_55->setHorizontalHeaderLabels(QStringList()
            << "Court Name" << "Location" << "Type" << "Start Time" << "End Time");

        // Populate the table with court data
        for (const auto& court : courts) {
            int row = ui->widget_55->rowCount();
            ui->widget_55->insertRow(row);
            ui->widget_55->setItem(row, 0, new QTableWidgetItem(court.m_courtName));
            ui->widget_55->setItem(row, 1, new QTableWidgetItem(court.m_location));
            ui->widget_55->setItem(row, 2, new QTableWidgetItem(court.m_type));
            ui->widget_55->setItem(row, 3, new QTableWidgetItem(court.m_startTime));
            ui->widget_55->setItem(row, 4, new QTableWidgetItem(court.m_endTime));
        }

        // Style the table to match the class schedule design
        ui->widget_55->setStyleSheet(R"(
            QTableWidget {
                background-color: #3b5a6a;
                color: #fffbe6;
                font: 13pt 'Yeasty Flavors';
                border-radius: 20px;
                border: none;
                gridline-color: #fffbe6;
            }
            QHeaderView::section {
                background-color: #3b5a6a;
                color: #fffbe6;
                font: bold 14pt 'Yeasty Flavors';
                border: none;
                padding: 8px;
            }
            QTableWidget::item {
                border-bottom: 2px solid #fffbe6;
                padding: 8px;
            }
            QTableWidget::item:selected {
                background-color: #f1c27d;
                color: #3b5a6a;
            }
        )");
        ui->widget_55->setShowGrid(false);
        ui->widget_55->verticalHeader()->setVisible(false);
        ui->widget_55->horizontalHeader()->setHighlightSections(false);
        ui->widget_55->setAlternatingRowColors(false);
        ui->widget_55->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->widget_55->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->widget_55->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->widget_55->horizontalHeader()->setStretchLastSection(true);
        for (int row = 0; row < ui->widget_55->rowCount(); ++row) {
            ui->widget_55->setRowHeight(row, 40);
        }
        ui->widget_55->resizeColumnsToContents();
    });

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
                displayWorkouts();  // Also update workouts here if member logs in

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
                this->pageAnimator->animatedSwitchAdvanced(
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
                    // Set the current staff member safely
                    try {
                        currStaff = stf;
                        
                        // Success message
                        QMessageBox::information(this, "Success", "You have logged in successfully.");
                        
                        // Configure style
                        QString newImagePath = ":/img/images/member-background.png";
                        QString newExitStyle = R"(
                                QPushButton {
                                    background-color: #008FC1;
                                    color: white;
                                    border-radius: 10px;
                                    padding: 6px 12px;
                                    font: bold 10pt "Yeasty Flavors";
                                }
                            )";
                        
                        // IMPORTANT: Use a safer approach to initialize the staff UI
                        // First set relevant UI elements to safe states
                        ui->staffMainStackWidget->setCurrentIndex(0); // Set to dashboard
                        
                        // Ensure UI is properly prepared before animation
                        QApplication::processEvents();
                        
                        // Switch pages with animation
                        this->pageAnimator->animatedSwitchAdvanced(
                            ui->FullWiedgit->currentIndex(),
                            1, // Staff page index
                            newImagePath,
                            newExitStyle
                        );
                        
                        // Mark as logged in
                        loggedIn = true;
                    }
                    catch (const std::exception& e) {
                        QMessageBox::critical(this, "Error", QString("An error occurred: %1").arg(e.what()));
                    }
                    catch (...) {
                        QMessageBox::critical(this, "Error", "An unexpected error occurred during login.");
                    }
                    
                    break; // Stop checking other staff
                }
            }
        }

        if (!loggedIn) {
            QMessageBox::warning(this, "Invalid Email or Password", "Please re-enter the information correctly.");
        }
    });

    // ---- PROFILE PAGE LOGIC ----
    // PROFILE PAGE CONNECTIONS (all as lambdas)
    // Helper: displayProfilePage logic as a lambda (can be reused)
    auto displayProfilePage = [=]() {
        if (!currMember) return;
        ui->labelName->setText(currMember->getName());
        ui->labelUsername->setText("@" + currMember->getEmail().split("@").first());
        // Load profile picture
        QString picPath = currMember->getProfilePicturePath();
        QString absPicPath = picPath;
        if (!picPath.isEmpty() && !QFile::exists(picPath)) {
            absPicPath = QCoreApplication::applicationDirPath() + "/" + picPath;
        }
        if (!picPath.isEmpty() && QFile::exists(absPicPath)) {
            QPixmap pix(absPicPath);
            int size = qMin(pix.width(), pix.height());
            QPixmap scaled = pix.scaled(180, 180, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
            QPixmap circular(200, 200);
            circular.fill(Qt::transparent);
            QPainter painter(&circular);
            painter.setRenderHint(QPainter::Antialiasing);
            QPainterPath path;
            path.addEllipse(0, 0,200, 200);
            painter.setClipPath(path);
            painter.drawPixmap(0, 0, scaled);
            // Draw border
            QPen pen(QColor("#c68f3b"), 4);
            painter.setPen(pen);
            painter.setBrush(Qt::NoBrush);
            painter.drawEllipse(2, 2,200, 200);
            painter.end();
            ui->labelProfilePic->setPixmap(circular);
        } else {
            ui->labelProfilePic->setPixmap(QPixmap(":/icons/images/icons/profile.svg").scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
        ui->stackedWidgetProfile->setCurrentWidget(ui->profileViewPage);
    };

    connect(ui->btnEditProfile, &QPushButton::clicked, this, [=] {
        if (!currMember) return;
        ui->editName->setText(currMember->getName());
        ui->editEmail->setText(currMember->getEmail());
        ui->editUsername->setText("@" + currMember->getEmail().split("@").first());
        ui->editPassword->setText(currMember->getPassword());
        ui->editPhone->setText(currMember->getPhone());
        // Load profile picture in edit mode
        QString picPath = currMember->getProfilePicturePath();
        QString absPicPath = picPath;
        if (!picPath.isEmpty() && !QFile::exists(picPath)) {
            absPicPath = QCoreApplication::applicationDirPath() + "/" + picPath;
        }
        if (!picPath.isEmpty() && QFile::exists(absPicPath)) {
            QPixmap pix(absPicPath);
            ui->labelEditProfilePic->setPixmap(pix.scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        } else {
            ui->labelEditProfilePic->setPixmap(QPixmap(":/icons/images/icons/profile.svg").scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
        ui->stackedWidgetProfile->setCurrentWidget(ui->editProfilePage);
    });

    connect(ui->btnSaveProfile, &QPushButton::clicked, this, [=] {
        if (!currMember) return;
        QString name = ui->editName->text().trimmed();
        QString email = ui->editEmail->text().trimmed();
        QString password = ui->editPassword->text();
        QString phone = ui->editPhone->text().trimmed();
        if (name.isEmpty() || email.isEmpty() || password.isEmpty()) {
            QMessageBox::warning(this, "Validation Error", "Name, email, and password cannot be empty.");
            return;
        }
        currMember->setName(name);
        currMember->setEmail(email);
        currMember->setPhone(phone);
        currMember->getPassword() = password; // If password is public, else add a setter
        displayProfilePage();
    });

    connect(ui->btnCancelEdit, &QPushButton::clicked, this, [=] {
        displayProfilePage();
    });

    connect(ui->btnTogglePassword, &QToolButton::clicked, this, [=] {
        if (ui->editPassword->echoMode() == QLineEdit::Password)
            ui->editPassword->setEchoMode(QLineEdit::Normal);
        else
            ui->editPassword->setEchoMode(QLineEdit::Password);
    });

    // Profile picture change in edit mode
    connect(ui->btnChangeProfilePic, &QPushButton::clicked, this, [=] {
        if (!currMember) return;
        QString fileName = QFileDialog::getOpenFileName(this, "Select Profile Picture", "", "Images (*.png *.jpg *.jpeg *.bmp)");
        if (!fileName.isEmpty()) {
            QDir dir(QCoreApplication::applicationDirPath() + "/profile_pics");
            if (!dir.exists()) dir.mkpath(".");
            QFileInfo fi(fileName);
            QString ext = fi.suffix();
            QString newFileName = QString("profile_pics/member_%1.%2").arg(currMember->getId()).arg(ext);
            QString absNewFileName = QCoreApplication::applicationDirPath() + "/" + newFileName;
            QFile::remove(absNewFileName);
            QFile::copy(fileName, absNewFileName);
            currMember->setProfilePicturePath(newFileName);
            QPixmap pix(absNewFileName);
            int size = qMin(pix.width(), pix.height());
            QPixmap scaled = pix.scaled(100, 100, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
            QPixmap circular(100, 100);
            circular.fill(Qt::transparent);
            QPainter painter(&circular);
            painter.setRenderHint(QPainter::Antialiasing);
            QPainterPath path;
            path.addEllipse(0, 0, 200, 200);
            painter.setClipPath(path);
            painter.drawPixmap(0, 0, scaled);
            QPen pen(QColor("#c68f3b"), 4);
            painter.setPen(pen);
            painter.setBrush(Qt::NoBrush);
            painter.drawEllipse(2, 2, 96, 96);
            painter.end();
            ui->labelEditProfilePic->setPixmap(circular);
        }
    });

    // Event filter for profile pic click in edit mode
    ui->labelEditProfilePic->installEventFilter(this);


    // Connect subscription buttons
    connect(ui->subBtn1m, &QPushButton::clicked, this, [=]() {
        if (!currMember) {
            QMessageBox::warning(this, "Error", "No member logged in");
            return;
        }

        QString message;
        bool isVip = ui->vipCheckBox->isChecked();
        double price = isVip ? 70.00 : 50.00;

        if (currMember->isSubscriptionActive()) {
            // Extending existing subscription
            QDate newEndDate = currMember->getSubscriptionEndDate().addMonths(1);
            message = QString("You currently have an active subscription ending on %1.\n\n"
                              "Subscribing for 1 month will extend your membership to %2.\n\n"
                              "Price: $%3.00%4")
                          .arg(currMember->getSubscriptionEndDate().toString("yyyy-MM-dd"))
                          .arg(newEndDate.toString("yyyy-MM-dd"))
                          .arg(price)
                          .arg(isVip ? " (VIP)" : "");
        } else {
            // New subscription
            message = QString("Would you like to subscribe for 1 month?\n\nPrice: $%1.00%2")
                          .arg(price)
                          .arg(isVip ? " (VIP)" : "");
        }

        QMessageBox::StandardButton confirm = QMessageBox::question(this,
                                                                    "Confirm Subscription", message, QMessageBox::Yes | QMessageBox::No);

        if (confirm == QMessageBox::Yes) {
            currMember->setSubscription("1m");
            currMember->setIsVip(isVip);
            updateSubscriptionInfo();

            QString successMessage;
            if (currMember->getSubscriptionStartDate() == QDate::currentDate()) {
                successMessage = "You have successfully subscribed for 1 month.";
            } else {
                successMessage = "You have successfully extended your subscription for 1 month.";
            }

            QMessageBox::information(this, "Subscription Activated",
                                     successMessage + "\nYour membership is active until " +
                                         currMember->getSubscriptionEndDate().toString("yyyy-MM-dd"));
        }
    });

    connect(ui->subBtn3m, &QPushButton::clicked, this, [=]() {
        if (!currMember) {
            QMessageBox::warning(this, "Error", "No member logged in");
            return;
        }

        QString message;
        bool isVip = ui->vipCheckBox->isChecked();
        double price = isVip ? 150.00 : 130.00;
        double savings = isVip ? 60.00 : 20.00;

        if (currMember->isSubscriptionActive()) {
            // Extending existing subscription
            QDate newEndDate = currMember->getSubscriptionEndDate().addMonths(3);
            message = QString("You currently have an active subscription ending on %1.\n\n"
                              "Subscribing for 3 months will extend your membership to %2.\n\n"
                              "Price: $%3.00 (Save $%4.00)%5")
                          .arg(currMember->getSubscriptionEndDate().toString("yyyy-MM-dd"))
                          .arg(newEndDate.toString("yyyy-MM-dd"))
                          .arg(price)
                          .arg(savings)
                          .arg(isVip ? " (VIP)" : "");
        } else {
            // New subscription
            message = QString("Would you like to subscribe for 3 months?\n\nPrice: $%1.00 (Save $%2.00)%3")
                          .arg(price)
                          .arg(savings)
                          .arg(isVip ? " (VIP)" : "");
        }

        QMessageBox::StandardButton confirm = QMessageBox::question(this,
                                                                    "Confirm Subscription", message, QMessageBox::Yes | QMessageBox::No);

        if (confirm == QMessageBox::Yes) {
            currMember->setSubscription("3m");
            currMember->setIsVip(isVip);
            updateSubscriptionInfo();

            QString successMessage;
            if (currMember->getSubscriptionStartDate() == QDate::currentDate()) {
                successMessage = "You have successfully subscribed for 3 months.";
            } else {
                successMessage = "You have successfully extended your subscription for 3 months.";
            }

            QMessageBox::information(this, "Subscription Activated",
                                     successMessage + "\nYour membership is active until " +
                                         currMember->getSubscriptionEndDate().toString("yyyy-MM-dd"));
        }
    });

    connect(ui->subBtn6m, &QPushButton::clicked, this, [=]() {
        if (!currMember) {
            QMessageBox::warning(this, "Error", "No member logged in");
            return;
        }

        QString message;
        bool isVip = ui->vipCheckBox->isChecked();
        double price = isVip ? 270.00 : 250.00;
        double savings = isVip ? 90.00 : 50.00;

        if (currMember->isSubscriptionActive()) {
            // Extending existing subscription
            QDate newEndDate = currMember->getSubscriptionEndDate().addMonths(6);
            message = QString("You currently have an active subscription ending on %1.\n\n"
                              "Subscribing for 6 months will extend your membership to %2.\n\n"
                              "Price: $%3.00 (Save $%4.00)%5")
                          .arg(currMember->getSubscriptionEndDate().toString("yyyy-MM-dd"))
                          .arg(newEndDate.toString("yyyy-MM-dd"))
                          .arg(price)
                          .arg(savings)
                          .arg(isVip ? " (VIP)" : "");
        } else {
            // New subscription
            message = QString("Would you like to subscribe for 6 months?\n\nPrice: $%1.00 (Save $%2.00)%3")
                          .arg(price)
                          .arg(savings)
                          .arg(isVip ? " (VIP)" : "");
        }

        QMessageBox::StandardButton confirm = QMessageBox::question(this,
                                                                    "Confirm Subscription", message, QMessageBox::Yes | QMessageBox::No);

        if (confirm == QMessageBox::Yes) {
            currMember->setSubscription("6m");
            currMember->setIsVip(isVip);
            updateSubscriptionInfo();

            QString successMessage;
            if (currMember->getSubscriptionStartDate() == QDate::currentDate()) {
                successMessage = "You have successfully subscribed for 6 months.";
            } else {
                successMessage = "You have successfully extended your subscription for 6 months.";
            }

            QMessageBox::information(this, "Subscription Activated",
                                     successMessage + "\nYour membership is active until " +
                                         currMember->getSubscriptionEndDate().toString("yyyy-MM-dd"));
        }
    });

    connect(ui->subBtn12m, &QPushButton::clicked, this, [=]() {
        if (!currMember) {
            QMessageBox::warning(this, "Error", "No member logged in");
            return;
        }

        QString message;
        bool isVip = ui->vipCheckBox->isChecked();
        double price = isVip ? 500.00 : 480.00;
        double savings = isVip ? 160.00 : 120.00;

        if (currMember->isSubscriptionActive()) {
            // Extending existing subscription
            QDate newEndDate = currMember->getSubscriptionEndDate().addMonths(12);
            message = QString("You currently have an active subscription ending on %1.\n\n"
                              "Subscribing for 12 months will extend your membership to %2.\n\n"
                              "Price: $%3.00 (Save $%4.00)%5")
                          .arg(currMember->getSubscriptionEndDate().toString("yyyy-MM-dd"))
                          .arg(newEndDate.toString("yyyy-MM-dd"))
                          .arg(price)
                          .arg(savings)
                          .arg(isVip ? " (VIP)" : "");
        } else {
            // New subscription
            message = QString("Would you like to subscribe for 12 months (1 year)?\n\nPrice: $%1.00 (Save $%2.00)%3")
                          .arg(price)
                          .arg(savings)
                          .arg(isVip ? " (VIP)" : "");
        }

        QMessageBox::StandardButton confirm = QMessageBox::question(this,
                                                                    "Confirm Subscription", message, QMessageBox::Yes | QMessageBox::No);

        if (confirm == QMessageBox::Yes) {
            currMember->setSubscription("12m");
            currMember->setIsVip(isVip);
            updateSubscriptionInfo();

            QString successMessage;
            if (currMember->getSubscriptionStartDate() == QDate::currentDate()) {
                successMessage = "You have successfully subscribed for 12 months.";
            } else {
                successMessage = "You have successfully extended your subscription for 12 months.";
            }

            QMessageBox::information(this, "Subscription Activated",
                                     successMessage + "\nYour membership is active until " +
                                         currMember->getSubscriptionEndDate().toString("yyyy-MM-dd"));
        }
    });

    // Connect to notification button to update subscription info when clicked
    connect(ui->NotifiMemberBtn, &QPushButton::clicked, this, &MainWindow::updateSubscriptionInfo);

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
        bool isVip = ui->Vip_5->isChecked(); // true if VIP, false if Normal
        Member * m = new Member(username, email, password, gender, isVip, phone, address, ageStr.toInt());
        currMember = m;
        members[m->getId()] = m;
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
        this->animations->animatedSwitchAdvanced(0, 1, ":/img/images/newpadelrounded.png", newStyle);
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
        this->animations->animatedSwitchAdvanced(1, 0, ":/img/images/newGymrounded.png", newStyle);
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

    connect(ui->NotifiMemberBtn, &QPushButton::clicked, this, [=]() {
        // Find the Notification page in the stackedWidget
        for (int i = 0; i < ui->stackedWidget->count(); i++) {
            if (ui->stackedWidget->widget(i)->objectName() == "Notification") {
                ui->stackedWidget->setCurrentIndex(i);
                // Update subscription info when showing the page
                updateSubscriptionInfo();
                break;
            }
        }
    });


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

        if (currMember->getClasses().contains(selectedClass)) {
            QMessageBox::warning(this, "Cannot Enroll", "You are already enrolled in this class.");
            return;
        }

        // Try to add member to class (this will handle waitlist if class is full)
        selectedClass->addMember(currMember);

        // Check if member was added to waitlist
        if (!currMember->getClasses().contains(selectedClass)) {
            QString message = QString("The class is currently full. You have been added to the waitlist. %1")
                .arg(currMember->getIsVip() ? "As a VIP member, you will be prioritized." : "");
            QMessageBox::information(this, "Added to Waitlist", message);
        } else {
            QMessageBox::information(this, "Success", "Successfully enrolled in " + selectedClass->getName());
        }

        updateClassesTable();
        updateEnrolledClassesTable();
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

        // Set column widths
        ui->tableWidget_5->setColumnWidth(0, 50);  // ID
        ui->tableWidget_5->setColumnWidth(1, 200); // Class Name
        ui->tableWidget_5->setColumnWidth(2, 100); // Time
        ui->tableWidget_5->setColumnWidth(3, 150); // Trainer
        ui->tableWidget_5->setColumnWidth(4, 100); // Status
        ui->tableWidget_5->setColumnWidth(5, 100); // Capacity

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
        
        // Apply the consistent styling
        applyTableStyle(ui->tableWidget_5);
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
        // Update class status if not full
        if (foundClass->getEnrolled() < foundClass->getCapacity()) {
            foundClass->setStatue("Open");
        }
        // Save changes
        // Update UI
        updateEnrolledClassesTable();
        updateClassesTable();
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
        
        // Apply consistent styling
        applyTableStyle(ui->tableWidget_6);
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
            QFile file("D:/temp windows/New folder/New folder/FullGymProject/members.txt");

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
        QFile file("D:/temp windows/New folder/New folder/FullGymProject/staffs.txt");
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
                updateTrainerCount();
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

    // Set column widths
    ui->tableWidget_15->setColumnWidth(0, 50);  // ID
    ui->tableWidget_15->setColumnWidth(1, 200); // Name
    ui->tableWidget_15->setColumnWidth(2, 50);  // Age

    // Apply the consistent styling
    applyTableStyle(ui->tableWidget_15);

    // Initial population of members table
    updateMembersTable();

    connect(ui->viewWaitlistBtn, &QPushButton::clicked, this, &MainWindow::viewWaitlist);

    // Connect toggleScheduleWaitlist button
    connect(ui->toggleScheduleWaitlistBtn, &QPushButton::clicked, this, &MainWindow::toggleScheduleWaitlist);
    connect(ui->addWorkoutButton, &QPushButton::clicked, this, &MainWindow::addWorkout);
    connect(ui->removeLastWorkoutButton, &QPushButton::clicked, this, &MainWindow::removeLastWorkout);
    connect(ui->clearWorkoutsButton, &QPushButton::clicked, this, &MainWindow::clearWorkouts);
    connect(ui->workoutLineEdit, &QLineEdit::returnPressed, this, &MainWindow::addWorkout);

    // Update workouts display when member logs in
    connect(ui->loginbtn, &QPushButton::clicked, this, [=] {
        if (currMember) {
            displayWorkouts();
        }
    });

    // Connect the WorkoutBtn to navigate to workout page
    connect(ui->WorkoutBtn, &QPushButton::clicked, this, [=]() {
        if (currMember) {
            // Find the workout page index
            int workoutPageIndex = -1;
            for (int i = 0; i < ui->stackedWidget->count(); i++) {
                if (ui->stackedWidget->widget(i)->objectName() == "workoutPage") {
                    workoutPageIndex = i;
                    break;
                }
            }

            if (workoutPageIndex >= 0) {
                ui->stackedWidget->setCurrentIndex(workoutPageIndex);
                displayWorkouts(); // Update the workout display
            }
        } else {
            QMessageBox::warning(this, "Error", "Please log in first");
        }
    });

    // Connect profile and subscription buttons
    connect(ui->Subscriptionbtn, &QPushButton::clicked, this, [=]() {
        // Find the Subscription page in the stackedWidget
        for (int i = 0; i < ui->stackedWidget->count(); i++) {
            if (ui->stackedWidget->widget(i)->objectName() == "Subscription") {
                ui->stackedWidget->setCurrentIndex(i);
                break;
            }
        }
    });

    connect(ui->NotifiMemberBtn, &QPushButton::clicked, this, [=]() {
        // Find the Notification page in the stackedWidget
        for (int i = 0; i < ui->stackedWidget->count(); i++) {
            if (ui->stackedWidget->widget(i)->objectName() == "Notification") {
                ui->stackedWidget->setCurrentIndex(i);
                // Update subscription info when showing the page
                updateSubscriptionInfo();
                break;
            }
        }
    });

    connect(ui->MemberProfileBtn, &QPushButton::clicked, this, [=]() {
        // Find the profile page in the stackedWidget
        for (int i = 0; i < ui->stackedWidget->count(); i++) {
            if (ui->stackedWidget->widget(i)->objectName() == "page_21") {
                ui->stackedWidget->setCurrentIndex(i);
                break;
            }
        }
    });

    // --- Padel Court Booking Logic ---
    auto updatePadalSlots = [=]() {
        QString location = ui->padal_location->text().trimmed().toLower();
        QString date = ui->padal_DATE_2->text().trimmed();
        QStringList slotLabels = {"03:00", "05:00", "07:00", "09:00"};

        // Reset all slots to available
        for (int i = 0; i < 4; ++i) {
            ui->comboBox_padal->setItemText(i, slotLabels[i]);
        }

        // Update slots based on bookings
        if (padelBookings.contains(location) && padelBookings[location].contains(date)) {
            const QStringList& slotList = padelBookings[location][date];
            for (int i = 0; i < 4 && i < slotList.size(); ++i) {
                if (slotList[i] != "0") {
                    ui->comboBox_padal->setItemText(i, slotLabels[i] + " (Booked)");
                }
            }
        }
    };

    // Also update the cancel slots function
    auto updateCancelSlots = [=]() {
        QString location = ui->padal_location_3->text().trimmed().toLower();
        QString date = ui->padal_DATE_4->text().trimmed();
        QStringList slotLabels = {"03:00", "05:00", "07:00", "09:00"};

        // Reset all slots to available
        for (int i = 0; i < 4; ++i) {
            ui->comboBox_padal_3->setItemText(i, slotLabels[i]);
        }

        // Update slots based on bookings
        if (padelBookings.contains(location) && padelBookings[location].contains(date)) {
            const QStringList& slotList = padelBookings[location][date];
            for (int i = 0; i < 4 && i < slotList.size(); ++i) {
                if (slotList[i] != "0") {
                    ui->comboBox_padal_3->setItemText(i, slotLabels[i] + " (Booked)");
                }
            }
        }
    };

    connect(ui->padal_location, &QLineEdit::textChanged, this, updatePadalSlots);
    connect(ui->padal_DATE_2, &QLineEdit::textChanged, this, updatePadalSlots);
    connect(ui->padal_location_3, &QLineEdit::textChanged, this, updateCancelSlots);
    connect(ui->padal_DATE_4, &QLineEdit::textChanged, this, updateCancelSlots);

    connect(ui->Book_Button, &QPushButton::clicked, this, [=]() {
        if (!currMember) {
            QMessageBox::warning(this, "Error", "Please log in first.");
            return;
        }

        QString location = ui->padal_location->text().trimmed().toLower();
        QString date = ui->padal_DATE_2->text().trimmed();
        int slotIndex = ui->comboBox_padal->currentIndex();
        QStringList slotLabels = {"03:00", "05:00", "07:00", "09:00"};

        // Validate date format and range
        QDate selectedDate = QDate::fromString(date, "yyyy-MM-dd");
        QDate today = QDate::currentDate();
        QDate maxDate = today.addMonths(1);

        if (!selectedDate.isValid()) {
            QMessageBox::warning(this, "Invalid Date", "Please enter a valid date in yyyy-MM-dd format.");
            return;
        }

        if (selectedDate < today) {
            QMessageBox::warning(this, "Invalid Date", "Cannot book dates in the past.");
            return;
        }

        if (selectedDate > maxDate) {
            QMessageBox::warning(this, "Invalid Date", 
                "Bookings are only allowed up to " + maxDate.toString("yyyy-MM-dd") + " (one month from today)");
            return;
        }

        // Check if booking already exists in file
        QFile file("G:/cs_project/ahhhhhhhhh/FullGymProject/members.txt");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(this, "Error", "Could not open members.txt");
            return;
        }

        bool bookingExists = false;
        while (!file.atEnd()) {
            QString line = file.readLine().trimmed();
            if (line.isEmpty()) continue;
            
            QStringList parts = line.split(":");
            if (parts.size() >= 9) {
                QString nextLine = file.readLine().trimmed();
                if (nextLine.startsWith("Padel:")) {
                    QStringList bookingParts = nextLine.mid(6).split("|");
                    if (bookingParts.size() == 3) {
                        QString existingLocation = bookingParts[0].trimmed().toLower();
                        QString existingDate = bookingParts[1].trimmed();
                        QString existingTime = bookingParts[2].trimmed();
                        
                        if (existingLocation == location && 
                            existingDate == date && 
                            existingTime == slotLabels[slotIndex]) {
                            bookingExists = true;
                            break;
                        }
                    }
                }
            }
        }
        file.close();

        if (bookingExists) {
            QMessageBox::warning(this, "Already Booked", "This time slot is already booked.");
            return;
        }

        // Initialize slots for this location and date if not exists
        if (!padelBookings.contains(location)) {
            padelBookings[location] = QMap<QString, QStringList>();
        }
        if (!padelBookings[location].contains(date)) {
            padelBookings[location][date] = QStringList{"0", "0", "0", "0"};
        }

        QStringList& slotList = padelBookings[location][date];
        if (slotList[slotIndex] == "0") {
            slotList[slotIndex] = QString::number(currMember->getId());
            
            // Update members.txt
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QMessageBox::warning(this, "Error", "Could not open members.txt");
                return;
            }

            QStringList lines;
            while (!file.atEnd()) {
                QString line = file.readLine().trimmed();
                if (line.isEmpty()) continue;
                
                QStringList parts = line.split(":");
                if (parts.size() >= 9 && parts[0].toInt() == currMember->getId()) {
                    // Add booking info to member's line
                    line += "\nPadel: " + location + "|" + date + "|" + slotLabels[slotIndex];
                }
                lines << line;
            }
            file.close();

            // Write back to file
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&file);
                for (const QString& l : lines) {
                    out << l << "\n";
                }
                file.close();
            }

            // Update bookings.txt
            QFile bookingsFile("G:/cs_project/ahhhhhhhhh/FullGymProject/bookings.txt");
            if (!bookingsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QMessageBox::warning(this, "Error", "Could not open bookings.txt");
                return;
            }

            QStringList bookingLines;
            bool found = false;
            while (!bookingsFile.atEnd()) {
                QString line = bookingsFile.readLine().trimmed();
                if (line.isEmpty()) continue;

                QStringList parts = line.split("|");
                if (parts.size() == 3) {
                    QString existingLocation = parts[0].trimmed().toLower();
                    QString existingDate = parts[1].trimmed();
                    if (existingLocation == location && existingDate == date) {
                        // Update the slot
                        QStringList slotValues = parts[2].split(" ");
                        if (slotValues.size() == 4) {
                            slotValues[slotIndex] = QString::number(currMember->getId());
                            line = location + "|" + date + "|" + slotValues.join(" ");
                            found = true;
                        }
                    }
                }
                bookingLines << line;
            }
            bookingsFile.close();

            // If no existing entry found, add new one
            if (!found) {
                QStringList slotValues = {"0", "0", "0", "0"};
                slotValues[slotIndex] = QString::number(currMember->getId());
                bookingLines << location + "|" + date + "|" + slotValues.join(" ");
            }

            // Write back to bookings.txt
            if (bookingsFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&bookingsFile);
                for (const QString& l : bookingLines) {
                    out << l << "\n";
                }
                bookingsFile.close();
            }

            QMessageBox::information(this, "Success", 
                QString("Court booked for %1 at %2!").arg(location, slotLabels[slotIndex]));
            updatePadalSlots();
        } else {
            QMessageBox::warning(this, "Unavailable", "This time slot is already booked.");
        }
    });

    connect(ui->cancelbutton, &QPushButton::clicked, this, [=]() {
        if (!currMember) {
            QMessageBox::warning(this, "Error", "Please log in first.");
            return;
        }

        QString location = ui->padal_location_3->text().trimmed().toLower();
        QString date = ui->padal_DATE_4->text().trimmed();
        int slotIndex = ui->comboBox_padal_3->currentIndex();
        QStringList slotLabels = {"03:00", "05:00", "07:00", "09:00"};

        // Get current date and time
        QDateTime currentDateTime = QDateTime::currentDateTime();
        QDate selectedDate = QDate::fromString(date, "yyyy-MM-dd");
        
        // Get the booking time
        QString timeStr = slotLabels[slotIndex];
        QTime bookingTime = QTime::fromString(timeStr, "hh:mm");
        QDateTime bookingDateTime(selectedDate, bookingTime);
        
        // Check if cancellation is at least 3 hours before booking
        if (currentDateTime.secsTo(bookingDateTime) < 3 * 3600) { // 3 hours in seconds
            QMessageBox::warning(this, "Cannot Cancel", 
                "Cancellations must be made at least 3 hours before the booking time.\n"
                "Your booking is at " + timeStr + " on " + date);
            return;
        }

        if (!padelBookings.contains(location) || 
            !padelBookings[location].contains(date) ||
            padelBookings[location][date][slotIndex] != QString::number(currMember->getId())) {
            QMessageBox::warning(this, "Not Booked", "You don't have a booking for this slot.");
            return;
        }

        // Cancel the booking
        padelBookings[location][date][slotIndex] = "0";

        // Update members.txt
        QFile file("G:/cs_project/ahhhhhhhhh/FullGymProject/members.txt");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(this, "Error", "Could not open members.txt");
            return;
        }

        QStringList lines;
        bool foundBooking = false;
        while (!file.atEnd()) {
            QString line = file.readLine().trimmed();
            if (line.isEmpty()) continue;
            
            QStringList parts = line.split(":");
            if (parts.size() >= 9 && parts[0].toInt() == currMember->getId()) {
                // Skip the booking line we want to remove
                QString nextLine = file.readLine().trimmed();
                if (nextLine.startsWith("Padel:") && 
                    nextLine.contains(location) && 
                    nextLine.contains(date) && 
                    nextLine.contains(slotLabels[slotIndex])) {
                    foundBooking = true;
                    continue;
                }
                lines << line;
                if (!nextLine.isEmpty()) lines << nextLine;
            } else {
                lines << line;
            }
        }
        file.close();

        // Write back to file
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            for (const QString& l : lines) {
                out << l << "\n";
            }
            file.close();
        }

        // Update bookings.txt
        QFile bookingsFile("G:/cs_project/ahhhhhhhhh/FullGymProject/bookings.txt");
        if (!bookingsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(this, "Error", "Could not open bookings.txt");
            return;
        }

        QStringList bookingLines;
        while (!bookingsFile.atEnd()) {
            QString line = bookingsFile.readLine().trimmed();
            if (line.isEmpty()) continue;

            QStringList parts = line.split("|");
            if (parts.size() == 3) {
                QString existingLocation = parts[0].trimmed().toLower();
                QString existingDate = parts[1].trimmed();
                if (existingLocation == location && existingDate == date) {
                    // Update the slot
                    QStringList slotValues = parts[2].split(" ");
                    if (slotValues.size() == 4) {
                        slotValues[slotIndex] = "0";
                        line = location + "|" + date + "|" + slotValues.join(" ");
                    }
                }
            }
            bookingLines << line;
        }
        bookingsFile.close();

        // Write back to bookings.txt
        if (bookingsFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&bookingsFile);
            for (const QString& l : bookingLines) {
                out << l << "\n";
            }
            bookingsFile.close();
        }

        QMessageBox::information(this, "Success", 
            QString("Booking canceled for %1 at %2!").arg(location, slotLabels[slotIndex]));
        updatePadalSlots();
    });

    // Move the connection inside the constructor
    connect(ui->searchButton, &QPushButton::clicked, this, &MainWindow::searchCourtAvailability);
}

void MainWindow::updateSubscriptionInfo() {
    if (!currMember) return;

    if (currMember->isSubscriptionActive()) {
        // Active subscription
        ui->subscriptionStatusLabel_2->setText("Active");

        // Display subscription type in a more user-friendly format
        QString friendlyType;
        if (currMember->getSubscriptionType() == "1m") {
            friendlyType = "1 Month";
        } else if (currMember->getSubscriptionType() == "3m") {
            friendlyType = "3 Months";
        } else if (currMember->getSubscriptionType() == "6m") {
            friendlyType = "6 Months";
        } else if (currMember->getSubscriptionType() == "12m") {
            friendlyType = "12 Months (1 Year)";
        } else {
            friendlyType = currMember->getSubscriptionType();
        }

        // Add VIP status to subscription type if applicable
        if (currMember->getIsVip()) {
            friendlyType += " (VIP)";
        }

        ui->subscriptionTypeLabel_2->setText(friendlyType);
        ui->subscriptionStartLabel_2->setText(currMember->getSubscriptionStartDate().toString("yyyy-MM-dd"));
        ui->subscriptionEndLabel_2->setText(currMember->getSubscriptionEndDate().toString("yyyy-MM-dd"));
        ui->subscriptionDaysLabel_2->setText(QString::number(currMember->getDaysRemaining()));

        // Set color for active subscription
        ui->subscriptionStatusLabel_2->setStyleSheet("color: green; font-weight: bold;");
    } else {
        // No active subscription
        ui->subscriptionStatusLabel_2->setText("Inactive");
        ui->subscriptionTypeLabel_2->setText("None");
        ui->subscriptionStartLabel_2->setText("N/A");
        ui->subscriptionEndLabel_2->setText("N/A");
        ui->subscriptionDaysLabel_2->setText("0");

        // Set color for inactive subscription
        ui->subscriptionStatusLabel_2->setStyleSheet("color: red; font-weight: bold;");
    }
}

// Event filter to handle profile pic click
bool MainWindow::eventFilter(QObject* obj, QEvent* event) {
    if (obj == ui->labelEditProfilePic && event->type() == QEvent::MouseButtonRelease) {
        if (!currMember) return true;
        QString fileName = QFileDialog::getOpenFileName(this, "Select Profile Picture", "", "Images (*.png *.jpg *.jpeg *.bmp)");
        if (!fileName.isEmpty()) {
            QDir dir(QCoreApplication::applicationDirPath() + "/profile_pics");
            if (!dir.exists()) dir.mkpath(".");
            QFileInfo fi(fileName);
            QString ext = fi.suffix();
            QString newFileName = QString("profile_pics/member_%1.%2").arg(currMember->getId()).arg(ext);
            QString absNewFileName = QCoreApplication::applicationDirPath() + "/" + newFileName;
            QFile::remove(absNewFileName);
            QFile::copy(fileName, absNewFileName);
            currMember->setProfilePicturePath(newFileName);
            QPixmap pix(absNewFileName);
            int size = qMin(pix.width(), pix.height());
            QPixmap scaled = pix.scaled(100, 100, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
            QPixmap circular(100, 100);
            circular.fill(Qt::transparent);
            QPainter painter(&circular);
            painter.setRenderHint(QPainter::Antialiasing);
            QPainterPath path;
            path.addEllipse(0, 0, 100, 100);
            painter.setClipPath(path);
            painter.drawPixmap(0, 0, scaled);
            QPen pen(QColor("#c68f3b"), 4);
            painter.setPen(pen);
            painter.setBrush(Qt::NoBrush);
            painter.drawEllipse(2, 2, 96, 96);
            painter.end();
            ui->labelEditProfilePic->setPixmap(circular);
        }
        return true;
    }
    return QMainWindow::eventFilter(obj, event);
}


MainWindow::~MainWindow() {
    // Save members with their padel bookings
    QFile file("G:/cs_project/ahhhhhhhhh/FullGymProject/members.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (auto it = members.begin(); it != members.end(); ++it) {
            Member* m = it.value();
            // Write member info
            out << it.key() << ":" << m->getName() << ":" << m->getEmail() << ":" 
                << m->getPassword() << ":" << m->getGender() << ":" 
                << (m->getIsVip() ? "true" : "false") << ":" << m->getPhone() 
                << ":" << m->getAddress() << ":" << m->getAge() << "\n";
            
            // Write member's padel bookings
            for (auto locIt = padelBookings.begin(); locIt != padelBookings.end(); ++locIt) {
                QString location = locIt.key();
                for (auto dateIt = locIt.value().begin(); dateIt != locIt.value().end(); ++dateIt) {
                    QString date = dateIt.key();
                    QStringList slotList = dateIt.value();
                    QStringList slotLabels = {"03:00", "05:00", "07:00", "09:00"};
                    
                    for (int i = 0; i < slotList.size(); ++i) {
                        if (slotList[i] == QString::number(m->getId())) {
                            out << "Padel: " << location << "|" << date << "|" << slotLabels[i] << "\n";
                        }
                    }
                }
            }
            
            // Write enrolled classes
            for (auto gc : m->getClasses()) {
                out << "Class: " << gc->getName() << "\n";
            }
            out << "\n"; // Blank line after each member
        }
        file.close();
    }

    delete ui;
    FileHandler::saveMembers("D:/temp windows/New folder/New folder/FullGymProject/members.txt", members);
    FileHandler::saveStaff("D:/temp windows/New folder/New folder/FullGymProject/staffs.txt", staffMap);
    FileHandler::saveClasses("D:/temp windows/New folder/New folder/FullGymProject/classes.txt", classesmap);
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
                // Using a local animation object for logout
                Animations* logoutAnimator = new Animations(this);
                logoutAnimator->setUI(ui->FullWiedgit, ui->label, ui->Exit);
                logoutAnimator->animatedSwitchAdvanced(
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
    QString capacityStr = ui->Class_capacity_31->text().trimmed();
    QString ClassTime = ui->Class_Time_32->text().trimmed();

    if (className.isEmpty() || capacityStr.isEmpty() || ClassTime.isEmpty()) {
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

    // Generate a new unique class ID
    int newClassId = 1;
    if (!classesmap.isEmpty()) {
        newClassId = classesmap.keys().last() + 1;
    }

    // Create new class with "Open" status
    GymClass* newClass = new GymClass(className, "Open", capacity);
    newClass->setId(newClassId);
    newClass->setTime(time);
    newClass->setEnrolled(0); // Initialize enrolled count to 0

    classesmap[newClassId] = newClass;

    // Update the classes table
    updateClassesTable();
    QMessageBox::information(this, "Success", "Class added successfully!");

    ui->ClassName_3->clear();
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

    // Apply the consistent styling
    applyTableStyle(ui->tableWidget_2);
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
    
    // Apply the consistent styling
    applyTableStyle(ui->tableWidget_15);
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
    bool isVip = ui->Vip_4->isChecked(); // true if VIP, false if Normal
    Member* newMember = new Member(name, email, password, gender, isVip, phone, address, age);
    members[newId] = newMember;

 

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

void MainWindow::updateTrainerCount() {
    int totalCoaches = 0;
    for (auto stf : staffMap) {
        if (stf->getRole().toLower() == "coach") {
            totalCoaches++;
        }
    }
    ui->label_40->setText(QString::number(totalCoaches));
}

void MainWindow::updateCapacity() {
        int totalCapacity = 0;
    for (const auto& gc : classesmap) {
        totalCapacity += gc->getCapacity();
    }
    ui->label_39->setText(QString::number(totalCapacity));
}

void MainWindow::viewWaitlist()
{
    // Get the class ID from the line edit
    QString classIdStr = ui->lineEditWaitlistClassId->text().trimmed();
    bool ok;
    int classId = classIdStr.toInt(&ok);

    if (!ok) {
        QMessageBox::warning(this, "Invalid Input", "Please enter a valid class ID.");
        return;
    }

    // Find the class in the classes map
    if (!classesmap.contains(classId)) {
        QMessageBox::warning(this, "Not Found", "Class with ID " + classIdStr + " not found.");
        return;
    }

    GymClass* gymClass = classesmap[classId];

    // Use the class schedule table (ui->tableWidget_2) to show the waitlist
    QTableWidget* waitlistTable = ui->tableWidget_2;
    waitlistTable->clearContents();
    waitlistTable->setRowCount(0);
    waitlistTable->setColumnCount(4);
    waitlistTable->setHorizontalHeaderLabels(QStringList()
        << "Name" << "Email" << "Phone" << "VIP Status");

    // Get waitlist as vector (already sorted by priority)
    QVector<Member*> waitlistMembers = gymClass->getWaitlistAsVector();
    
    // Add all members in priority order
    for (Member* member : waitlistMembers) {
        int row = waitlistTable->rowCount();
        waitlistTable->insertRow(row);
        waitlistTable->setItem(row, 0, new QTableWidgetItem(member->getName()));
        waitlistTable->setItem(row, 1, new QTableWidgetItem(member->getEmail()));
        waitlistTable->setItem(row, 2, new QTableWidgetItem(member->getPhone()));
        waitlistTable->setItem(row, 3, new QTableWidgetItem(member->getIsVip() ? "Yes" : "No"));
    }

    // Set column widths
    waitlistTable->setColumnWidth(0, 200);  // Name
    waitlistTable->setColumnWidth(1, 200);  // Email
    waitlistTable->setColumnWidth(2, 150);  // Phone
    waitlistTable->setColumnWidth(3, 100);  // VIP Status

    // Apply our consistent styling
    applyTableStyle(waitlistTable);
}

void MainWindow::toggleScheduleWaitlist() {
    try {
        // Safely check if we should toggle
        if (ui->tableWidget_2 && ui->toggleScheduleWaitlistBtn) {
            if (!showingWaitlist) {
                // Instead of showing waitlist, just show a message that it's not available
                QMessageBox::information(this, "Information", "Waitlist view is currently disabled.");
                
                // Just update the current table instead of using waitlist
                updateClassesTable();
                ui->toggleScheduleWaitlistBtn->setText("Show Waitlist");
                showingWaitlist = false;
            } else {
                // Show class schedule
                updateClassesTable();
                ui->toggleScheduleWaitlistBtn->setText("Show Waitlist");
                showingWaitlist = false;
            }
        }
    } catch (...) {
        // Silently handle any errors to prevent crashes
        showingWaitlist = false;
        // Try to reset the UI to a known state
        if (ui->tableWidget_2) {
            ui->tableWidget_2->clearContents();
            ui->tableWidget_2->setRowCount(0);
        }
    }
}



void MainWindow::addWorkout()
{
    if (!currMember) {
        QMessageBox::warning(this, "Error", "No member selected!");
        return;
    }

    QString workoutText = ui->workoutLineEdit->text().trimmed();
    if (workoutText.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter workout details!");
        return;
    }

    // Add workout to member's stack
    QStack<QPair<QString, QDate>> workouts = currMember->getWorkouts();
    workouts.push(qMakePair(workoutText, QDate::currentDate()));
    currMember->setWorkouts(workouts);

    // Clear input field
    ui->workoutLineEdit->clear();

    // Update display
    displayWorkouts();
}

void MainWindow::removeLastWorkout()
{
    if (!currMember) {
        QMessageBox::warning(this, "Error", "No member selected!");
        return;
    }

    QStack<QPair<QString, QDate>> workouts = currMember->getWorkouts();
    if (!workouts.isEmpty()) {
        workouts.pop();
        currMember->setWorkouts(workouts);
        displayWorkouts();
    } else {
        QMessageBox::information(this, "Info", "No workouts to remove.");
    }
}


void MainWindow::displayWorkouts()
{
    ui->workoutListWidget->clear();
    if (!currMember) {
        return;
    }

    QStack<QPair<QString, QDate>> workouts = currMember->getWorkouts();
    QStack<QPair<QString, QDate>> tempStack = workouts;

    QStringList workoutList;

    while (!tempStack.isEmpty()) {
        auto entry = tempStack.pop();
        QString workoutText = entry.first;
        QDate workoutDate = entry.second;

        QListWidgetItem* item = new QListWidgetItem();

        // Icon selection logic (reuse your current checks)
        QIcon icon;
        if (workoutText.contains("run", Qt::CaseInsensitive) || workoutText.contains("jog", Qt::CaseInsensitive)) {
            icon = QIcon(":/img/images/running.png");
        } else if (workoutText.contains("swim", Qt::CaseInsensitive)) {
            icon = QIcon(":/img/images/swimming.png");
        } else if (workoutText.contains("bike", Qt::CaseInsensitive) || workoutText.contains("cycle", Qt::CaseInsensitive)) {
            icon = QIcon(":/img/images/cycling.png");
        } else if (workoutText.contains("lift", Qt::CaseInsensitive) || workoutText.contains("weight", Qt::CaseInsensitive) ||
                   workoutText.contains("press", Qt::CaseInsensitive) || workoutText.contains("curl", Qt::CaseInsensitive)) {
            icon = QIcon(":/img/images/weight.png");
        } else {
            icon = QIcon(":/img/images/workout.png");
        }

        item->setIcon(icon);

        QString formattedText = QString("%1 - %2")
                                    .arg(workoutDate.toString("dd MMM yyyy"))
                                    .arg(workoutText);

        item->setText(formattedText);
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        item->setSizeHint(QSize(item->sizeHint().width(), 40));
        ui->workoutListWidget->addItem(item);
    }
}

void MainWindow::clearWorkouts()
{
    if (!currMember) {
        QMessageBox::warning(this, "Error", "No member selected!");
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm Clear",
                                                              "Are you sure you want to clear all workouts?",
                                                              QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        currMember->setWorkouts(QStack<QPair<QString, QDate>>());
        displayWorkouts();
    }
}

// Add this function to apply consistent styling to all tables
void MainWindow::applyTableStyle(QTableWidget* table) {
    if (!table) return;
    
    // Apply the requested styling
    table->setStyleSheet(R"(
        QTableWidget {
            background-color: rgb(55, 91, 106);
            font: 11pt "Yeasty Flavors";
            color: rgb(249, 234, 205);
            border-radius: 15px;
            gridline-color: rgb(249, 234, 205);
        }
        QHeaderView::section {
            background-color: rgb(55, 91, 106);
            color: rgb(249, 234, 205);
            font: bold 12pt "Yeasty Flavors";
            border: none;
            padding: 6px;
        }
        QTableWidget::item {
            selection-background-color: rgb(198, 143, 59);
            selection-color: black;
        }
    )");
    
    // Additional customizations
    table->setAlternatingRowColors(false); // Disable alternating colors since we have a dark theme
    table->setEditTriggers(QAbstractItemView::NoEditTriggers); // Make table read-only
    table->setSelectionBehavior(QAbstractItemView::SelectRows); // Select entire rows
    table->horizontalHeader()->setStretchLastSection(true); // Stretch last column
    table->verticalHeader()->setVisible(false); // Hide vertical headers
    table->setShowGrid(true); // Show grid lines
}

void MainWindow::toggleScheduleWaitlist() {
    if (!showingWaitlist) {
        // Show waitlist
        viewWaitlist();
        ui->toggleScheduleWaitlistBtn->setText("Show Schedule");
        showingWaitlist = true;
    } else {
        // Show class schedule
        updateClassesTable();
        ui->toggleScheduleWaitlistBtn->setText("Show Waitlist");
        showingWaitlist = false;
    }
}

// Add the function definition outside of the constructor
void MainWindow::searchCourtAvailability() {
    QString location = ui->padal_location_4->text().trimmed().toLower();
    QString date = ui->padal_DATE_5->text().trimmed();

    // Validate date format
    QDate selectedDate = QDate::fromString(date, "yyyy-MM-dd");
    if (!selectedDate.isValid()) {
        QMessageBox::warning(this, "Invalid Date", "Please enter a valid date in yyyy-MM-dd format.");
        return;
    }

    // Read bookings.txt to get slot information
    QFile bookingsFile("G:/cs_project/ahhhhhhhhh/FullGymProject/bookings.txt");
    if (!bookingsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Could not open bookings.txt");
        return;
    }

    QStringList slotLabels = {"03:00", "05:00", "07:00", "09:00"};
    QMap<QString, bool> slotAvailability;
    bool found = false;

    // Initialize all slots as available
    for (const QString& slot : slotLabels) {
        slotAvailability[slot] = true;
    }

    // Read and process bookings
    QString content = bookingsFile.readAll();
    bookingsFile.close();
    
    QStringList lines = content.split('\n', Qt::SkipEmptyParts);
    for (const QString& line : lines) {
        QStringList parts = line.split("|");
        if (parts.size() == 3) {
            QString existingLocation = parts[0].trimmed().toLower();
            QString existingDate = parts[1].trimmed();
            
            if (existingLocation == location && existingDate == date) {
                found = true;
                QStringList slotValues = parts[2].split(" ");
                for (int i = 0; i < slotValues.size() && i < slotLabels.size(); ++i) {
                    QString value = slotValues[i].trimmed();
                    slotAvailability[slotLabels[i]] = (value == QString("0"));
                }
                break;
            }
        }
    }

    // Setup the table widget
    ui->widget_56->clearContents();
    ui->widget_56->setRowCount(0);
    ui->widget_56->setColumnCount(2);
    ui->widget_56->setHorizontalHeaderLabels(QStringList() << "Time Slot" << "Status");

    // Style the table
    ui->widget_56->setStyleSheet(R"(
        QTableWidget {
            background-color: #3b5a6a;
            color: #fffbe6;
            font: 13pt 'Yeasty Flavors';
            border-radius: 20px;
            border: none;
            gridline-color: #fffbe6;
        }
        QHeaderView::section {
            background-color: #3b5a6a;
            color: #fffbe6;
            font: bold 14pt 'Yeasty Flavors';
            border: none;
            padding: 8px;
        }
        QTableWidget::item {
            border-bottom: 2px solid #fffbe6;
            padding: 8px;
        }
        QTableWidget::item:selected {
            background-color: #f1c27d;
            color: #3b5a6a;
        }
    )");

    // Set column widths
    ui->widget_56->setColumnWidth(0, 150);  // Time Slot
    ui->widget_56->setColumnWidth(1, 150);  // Status

    // Make the table read-only
    ui->widget_56->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    // Enable selection of entire rows
    ui->widget_56->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    // Hide vertical header
    ui->widget_56->verticalHeader()->setVisible(false);

    // Add rows for each time slot
    for (const QString& slot : slotLabels) {
        int row = ui->widget_56->rowCount();
        ui->widget_56->insertRow(row);
        
        // Add time slot
        ui->widget_56->setItem(row, 0, new QTableWidgetItem(slot));
        
        // Add status
        QString status = slotAvailability[slot] ? "Available" : "Booked";
        QTableWidgetItem* statusItem = new QTableWidgetItem(status);
        statusItem->setForeground(status == "Available" ? QColor("#4CAF50") : QColor("#f44336"));
        ui->widget_56->setItem(row, 1, statusItem);
    }

    // Set row heights
    for (int row = 0; row < ui->widget_56->rowCount(); ++row) {
        ui->widget_56->setRowHeight(row, 40);
    }
}