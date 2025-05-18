#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QPushButton>
#include <QPixmap>
#include <QMessageBox>
#include <QFile>
#include <QMap>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QStackedWidget>
#include <QWidget>
#include <QLineEdit>
#include <QTableWidget>
#include "member.h"
#include "manger.h"
#include "staff.h"
#include "animations.h"
#include "Court.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    void setPixmapForWidgets();
    void updateTrainerCount();
    void updateCapacity();
    void updateSubscriptionInfo();
    void applyTableStyle(QTableWidget* table);
    void searchCourtAvailability();
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addMember();
    void removeMember();
    void updateMembersTable();
    void addClass();
    void removeClass();
    void updateClassesTable();
    void viewWaitlist();
    void toggleScheduleWaitlist();
    void addWorkout();
    void removeLastWorkout();
    void displayWorkouts();
    void clearWorkouts();
protected:
    bool eventFilter(QObject* obj, QEvent* event) override;
private:
    QMap<int, Member*> members;
    QMap<int, Staff*> staffMap;
    QMap<int, GymClass*> classesmap;
    QMap<QString, QMap<QString, QStringList>> padelBookings; // location -> (date -> slots)
    Member* currMember = nullptr;
    Staff* currStaff = nullptr;
    QVector<Court> courts;
    Ui::MainWindow *ui;
    QLineEdit* lineEditWaitlistClassId;
    bool showingWaitlist = false;
    
    // Animation objects
    Animations* animations;
    Animations* pageAnimator;
};
#endif // MAINWINDOW_H
