#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <QObject>
#include <QStackedWidget>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QGraphicsOpacityEffect>
#include <QPushButton>
#include <QLabel>
#include <QEasingCurve>

class Animations : public QObject {
    Q_OBJECT

public:
    explicit Animations(QObject* parent = nullptr);

    // Pass UI pointers to allow cross-class access
    void setUI(QStackedWidget* stackedWidget, QLabel* backgroundLabel, QPushButton* exitButton);

    void animateStackedWidgetTransition(int fromIndex, int toIndex);
    void fadeBetweenPages(int fromIndex, int toIndex);
    void animatedSwitch(int fromIndex, int toIndex, const QString& newImagePath);
    void animatedSwitchAdvanced(int fromIndex, int toIndex, const QString& newImagePath, const QString& newExitStyle);

private:
    Animations* pageAnimator;
    QStackedWidget* stackedWidget = nullptr;
    QLabel* backgroundLabel = nullptr;
    QPushButton* exitButton = nullptr;
};

#endif // ANIMATIONS_H
