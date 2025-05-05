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
#include "member.h"
#include "manger.h"
#include "staff.h"
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
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
private:
    QMap<int, Member*> members;
    QMap<int, Staff*> staffMap;
    QMap<int, GymClass*> classesmap;
    Member* currMember;
    Staff * currStaff;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
