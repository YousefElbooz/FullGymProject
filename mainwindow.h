#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include "member.h"
#include "staff.h"
#include "gymclass.h"
#include "manger.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddMember();
    void onAddStaff();
    void onAddClass();
    void onAssignClass();
    void loadPhotos();
    void onSignUp();
    void showEvent(QShowEvent *event) override;

private:
    Ui::MainWindow *ui;
    QMap<int, Member*> members;
    QMap<int, Staff*> staffMap;
    QMap<int, GymClass*> classesmap;
    Manger manger;
    void loadData();
    void updateUI();
};

#endif // MAINWINDOW_H
