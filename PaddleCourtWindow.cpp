#include "PaddleCourtWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QHeaderView>

PaddleCourtWindow::PaddleCourtWindow(Member* currentMember, QWidget *parent)
    : QMainWindow(parent), currentMember(currentMember)
{
    setWindowTitle("Paddle Court Booking");
    setFixedSize(800, 600);

    // Create central widget and layout
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    // Create table
    courtsTable = new QTableWidget(this);
    courtsTable->setColumnCount(5);
    courtsTable->setHorizontalHeaderLabels({"Court ID", "Location", "Type", "Opening Time", "Closing Time"});
    courtsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    courtsTable->setStyleSheet(R"(
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
    )");

    // Create booking section
    QWidget* bookingWidget = new QWidget(this);
    QHBoxLayout* bookingLayout = new QHBoxLayout(bookingWidget);

    courtIdInput = new QLineEdit(this);
    courtIdInput->setPlaceholderText("Court ID");
    courtIdInput->setStyleSheet("border-radius: 10px; padding: 6px; background: #fffbe7; color: #3a546b; font-size: 16px;");

    dateInput = new QLineEdit(this);
    dateInput->setPlaceholderText("Date (YYYY-MM-DD)");
    dateInput->setStyleSheet("border-radius: 10px; padding: 6px; background: #fffbe7; color: #3a546b; font-size: 16px;");

    timeInput = new QLineEdit(this);
    timeInput->setPlaceholderText("Time (HH:MM)");
    timeInput->setStyleSheet("border-radius: 10px; padding: 6px; background: #fffbe7; color: #3a546b; font-size: 16px;");

    bookButton = new QPushButton("Book Court", this);
    bookButton->setStyleSheet(R"(
        QPushButton {
            background: #f5e9c6;
            color: #3a546b;
            border-radius: 15px;
            font-size: 18px;
            font-weight: bold;
            padding: 10px;
        }
        QPushButton:hover {
            background: #e6d9b8;
        }
    )");

    bookingLayout->addWidget(courtIdInput);
    bookingLayout->addWidget(dateInput);
    bookingLayout->addWidget(timeInput);
    bookingLayout->addWidget(bookButton);

    // Add widgets to main layout
    mainLayout->addWidget(courtsTable);
    mainLayout->addWidget(bookingWidget);

    // Connect signals
    connect(bookButton, &QPushButton::clicked, this, &PaddleCourtWindow::bookCourt);

    // Load courts
    loadCourts();
}

PaddleCourtWindow::~PaddleCourtWindow()
{
}

void PaddleCourtWindow::loadCourts()
{
    QFile file("courts.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Could not open courts.txt");
        return;
    }

    QTextStream in(&file);
    courtsTable->setRowCount(0);

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList parts = line.split("|");
        if (parts.size() >= 5) {
            int row = courtsTable->rowCount();
            courtsTable->insertRow(row);
            
            courtsTable->setItem(row, 0, new QTableWidgetItem(parts[0])); // Court ID
            courtsTable->setItem(row, 1, new QTableWidgetItem(parts[1])); // Location
            courtsTable->setItem(row, 2, new QTableWidgetItem(parts[2])); // Type
            courtsTable->setItem(row, 3, new QTableWidgetItem(parts[3])); // Opening Time
            courtsTable->setItem(row, 4, new QTableWidgetItem(parts[4])); // Closing Time
        }
    }

    file.close();
}

void PaddleCourtWindow::bookCourt()
{
    QString courtId = courtIdInput->text().trimmed();
    QString date = dateInput->text().trimmed();
    QString time = timeInput->text().trimmed();

    if (courtId.isEmpty() || date.isEmpty() || time.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please fill in all fields");
        return;
    }

    // Validate date format
    QDate bookingDate = QDate::fromString(date, "yyyy-MM-dd");
    if (!bookingDate.isValid()) {
        QMessageBox::warning(this, "Invalid Date", "Please enter date in YYYY-MM-DD format");
        return;
    }

    // Validate time format
    QTime bookingTime = QTime::fromString(time, "HH:mm");
    if (!bookingTime.isValid()) {
        QMessageBox::warning(this, "Invalid Time", "Please enter time in HH:MM format");
        return;
    }

    // Check if court exists
    bool courtExists = false;
    QString courtLocation, courtType, openingTime, closingTime;
    for (int i = 0; i < courtsTable->rowCount(); i++) {
        if (courtsTable->item(i, 0)->text() == courtId) {
            courtExists = true;
            courtLocation = courtsTable->item(i, 1)->text();
            courtType = courtsTable->item(i, 2)->text();
            openingTime = courtsTable->item(i, 3)->text();
            closingTime = courtsTable->item(i, 4)->text();
            break;
        }
    }

    if (!courtExists) {
        QMessageBox::warning(this, "Invalid Court", "Court ID does not exist");
        return;
    }

    // Check if time is within court hours
    QTime courtOpen = QTime::fromString(openingTime, "HH:mm");
    QTime courtClose = QTime::fromString(closingTime, "HH:mm");
    if (bookingTime < courtOpen || bookingTime > courtClose) {
        QMessageBox::warning(this, "Invalid Time", "Booking time must be within court hours");
        return;
    }

    // Check if court is already booked
    QString bookingKey = date + " " + time;
    if (courtBookings[courtId].contains(bookingKey) && courtBookings[courtId][bookingKey]) {
        QMessageBox::warning(this, "Already Booked", "This court is already booked for the selected time");
        return;
    }

    // Book the court
    courtBookings[courtId][bookingKey] = true;

    // Save booking to file
    QFile bookingsFile("bookings.txt");
    if (bookingsFile.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&bookingsFile);
        out << courtId << "|" << date << "|" << time << "|" << currentMember->getId() << "\n";
        bookingsFile.close();
    }

    QMessageBox::information(this, "Success", "Court booked successfully!");
    
    // Clear inputs
    courtIdInput->clear();
    dateInput->clear();
    timeInput->clear();
}

void PaddleCourtWindow::updateTable()
{
    loadCourts();
} 