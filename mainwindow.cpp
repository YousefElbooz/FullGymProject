#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "filehandler.h"
#include "coach.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QScreen>
#include <QGraphicsBlurEffect>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), manger("Sys", "00", "00", 0) {
    ui->setupUi(this);
    loadData();
    updateUI();
    connect(ui->assignClass, &QPushButton::clicked, this, &MainWindow::onAssignClass);
    connect(ui->btnAddMember, &QPushButton::clicked, this, &MainWindow::onAddMember);
    connect(ui->btnAddCoach, &QPushButton::clicked, this, &MainWindow::onAddStaff);
    connect(ui->btnAddClass, &QPushButton::clicked, this, &MainWindow::onAddClass);
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::close);
    ui->FullWidget->setCurrentIndex(0);
    loadPhotos();
    this->setWindowFlag(Qt::FramelessWindowHint);
    QGraphicsBlurEffect* p_blur = new QGraphicsBlurEffect;
    p_blur->setBlurRadius(10);
    p_blur->setBlurHints(QGraphicsBlurEffect::QualityHint);

    ui->loginWifdget->setGraphicsEffect(p_blur);
}

MainWindow::~MainWindow() {
    FileHandler::saveMembers("members.txt", members);
    FileHandler::saveStaff("staffs.txt", staffMap);
    FileHandler::saveClasses("classes.txt", classesmap);
    qDeleteAll(members);
    qDeleteAll(staffMap);
    qDeleteAll(classesmap);
    delete ui;
}

void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);

    // Center the window
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;
    move(x, y);
}

void MainWindow::loadPhotos(){
    QString imagePaths[] = {
        "C:/Users/Yousef/Documents/FullGymProject/images/رونالدو.jpeg",
        "C:/Users/Yousef/Documents/FullGymProject/images/SheilYaTweelElOmer.jpg",
        "C:/Users/Yousef/Documents/FullGymProject/images/487923262_18358780738183487_3064122802807129279_n.jpg",
        "C:/Users/Yousef/Documents/FullGymProject/images/486954623_18458191684073759_6188361622420108090_n.jpg",
        "C:/Users/Yousef/Documents/FullGymProject/images/487106121_18358780660183487_738216244790693941_n.jpg",
        "C:/Users/Yousef/Documents/FullGymProject/images/Mosalah.jpg",
        "C:/Users/Yousef/Documents/FullGymProject/images/487794954_18037488260535615_7174140484398860324_n.jpg",
        "C:/Users/Yousef/Documents/FullGymProject/images/ChatGPT Image Apr 15, 2025, 12_44_13 PM.png",
        "C:/Users/Yousef/Documents/FullGymProject/images/486761338_18358780648183487_518183920224791822_n.jpg",
        "C:/Users/Yousef/Documents/FullGymProject/images/Maldini.jpg",
        "C:/Users/Yousef/Documents/FullGymProject/images/93bd66f367106eaa00a77764796b1f77.jpg",
        "C:/Users/Yousef/Documents/FullGymProject/images/487931916_18458191666073759_5571215937298845863_n.jpg",
        "C:/Users/Yousef/Documents/FullGymProject/images/Studio Ghibli aesthetics.jpeg",
        "C:/Users/Yousef/Documents/FullGymProject/images/487241769_18358780711183487_4467916180267804207_n.jpg",
        "C:/Users/Yousef/Documents/FullGymProject/images/exit.svg"
    };




    ui->bgp->setPixmap(QPixmap(imagePaths[0]));
    ui->bgp_2->setPixmap(QPixmap(imagePaths[1]));
    ui->bgp_3->setPixmap(QPixmap(imagePaths[2]));
    ui->bgp_4->setPixmap(QPixmap(imagePaths[3]));
    ui->bgp_5->setPixmap(QPixmap(imagePaths[4]));
    ui->bgp_6->setPixmap(QPixmap(imagePaths[5]));
    ui->bgp_7->setPixmap(QPixmap(imagePaths[6]));
    ui->bgp_8->setPixmap(QPixmap(imagePaths[7]));
    ui->bgp_9->setPixmap(QPixmap(imagePaths[8]));
    ui->bgp_10->setPixmap(QPixmap(imagePaths[9]));
    ui->bgp_11->setPixmap(QPixmap(imagePaths[10]));
    ui->bgp_12->setPixmap(QPixmap(imagePaths[11]));
    ui->bgp_13->setPixmap(QPixmap(imagePaths[12]));
    ui->bgp_14->setPixmap(QPixmap(imagePaths[13]));
    ui->pushButton->setIcon(QIcon(imagePaths[14]));

}

void MainWindow::loadData() {
    members = FileHandler::loadMembers("members.txt");
    staffMap = FileHandler::loadStaff("staffs.txt");
    classesmap = FileHandler::loadClasses("classes.txt");
    /////////////////////////////////////


}

void MainWindow::updateUI() {
    ui->listWidget->clear();
    for (auto member : members)
        ui->listWidget->addItem(member->toString());
    for (auto staff : staffMap)
        ui->listWidget->addItem(staff->toString());
    for (auto gymClass : classesmap)
        ui->listWidget->addItem(gymClass->toString());
}

void MainWindow::onAssignClass() {
    int coachId = ui->CoachId->text().toInt();
    int classId = ui->ClassId->text().toInt();

    if (staffMap.contains(coachId) && classesmap.contains(classId)) {
        Coach* coach = dynamic_cast<Coach*>(staffMap[coachId]);
        GymClass* gymClass = classesmap[classId];

        if (coach && gymClass) {
            coach->addClass(gymClass);
            gymClass->setCoach(coach);
            updateUI();
            QMessageBox::information(this, "Success", "Class assigned successfully.");
        } else {
            QMessageBox::warning(this, "Error", "Staff is not a Coach or class is invalid.");
        }
    } else {
        QMessageBox::warning(this, "Error", "Invalid Coach ID or Class ID.");
    }
}

void MainWindow::onAddMember() {
    QString name = QInputDialog::getText(this, "New Member", "Enter name:");
    if (name.isEmpty()) return;
    QString email = QInputDialog::getText(this, "New Member", "Enter email:");
    if (email.isEmpty()) return;
    QString gender = QInputDialog::getText(this, "New Member", "Enter gender:");
    if (gender.isEmpty()) return;
    bool isVip = QMessageBox::question(this, "VIP?", "Is VIP?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes;
    QString phone = QInputDialog::getText(this, "New Member", "Enter phone number:");
    if (phone.isEmpty()) return;
    QString address = QInputDialog::getText(this, "New Member", "Enter address:");
    if (address.isEmpty()) return;
    bool ok;
    int age = QInputDialog::getInt(this, "New Member", "Enter age:", 18, 1, 120, 1, &ok);
    if (!ok) return;
    Member* m = new Member(name, email, gender, isVip, phone, address, age);
    members[m->getId()] = m;
    updateUI();
}

void MainWindow::onAddStaff() {
    QString name = QInputDialog::getText(this, "New Staff", "Enter name:");
    if (name.isEmpty()) return;
    QString role = QInputDialog::getText(this, "New Staff", "Enter role (e.g., Coach, Manager, Receptionist):");
    if (role.isEmpty()) return;
    QString phone = QInputDialog::getText(this, "New Staff", "Enter phone number:");
    if (phone.isEmpty()) return;
    QString address = QInputDialog::getText(this, "New Staff", "Enter address:");
    if (address.isEmpty()) return;
    bool ok;
    int age = QInputDialog::getInt(this, "New Staff", "Enter age:", 18, 1, 120, 1, &ok);
    if (!ok) return;

    Staff* staff = nullptr;
    if (role.toLower() == "coach") {
        staff = new Coach(name, phone, address, age);
    } else if (role.toLower() == "manager") {
        staff = new Manger(name, phone, address, age);
    } else if (role.toLower() == "receptionist") {
        staff = new Receptionist(name, phone, address, age);
    }

    if (staff) {
        staffMap[staff->getId()] = staff;
    }
    updateUI();
}

void MainWindow::onAddClass() {
    QString className = QInputDialog::getText(this, "New Gym Class", "Enter class name:");
    if (className.isEmpty()) return;
    QString classSchedule = QInputDialog::getText(this, "New Gym Class", "Enter class schedule (e.g., 10:00 AM):");
    if (classSchedule.isEmpty()) return;
    bool ok;
    int classCapacity = QInputDialog::getInt(this, "New Gym Class", "Enter class capacity:", 1, 1, 100, 1, &ok);
    if (!ok) return;

    GymClass* newClass = new GymClass(className, classSchedule, classCapacity);
    classesmap[newClass->getId()] = newClass;
    updateUI();
}
