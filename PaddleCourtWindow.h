#ifndef PADDLECOURTWINDOW_H
#define PADDLECOURTWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>
#include "member.h"

class PaddleCourtWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PaddleCourtWindow(Member* currentMember, QWidget *parent = nullptr);
    ~PaddleCourtWindow();

private slots:
    void loadCourts();
    void bookCourt();
    void updateTable();

private:
    QTableWidget* courtsTable;
    QPushButton* bookButton;
    QLineEdit* courtIdInput;
    QLineEdit* dateInput;
    QLineEdit* timeInput;
    Member* currentMember;
    QMap<QString, QMap<QString, bool>> courtBookings; // courtId -> (datetime -> isBooked)
};

#endif // PADDLECOURTWINDOW_H 