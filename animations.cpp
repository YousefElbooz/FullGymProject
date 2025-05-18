#include "animations.h"

Animations::Animations(QObject* parent) : QObject(parent) {}

void Animations::setUI(QStackedWidget* stacked, QLabel* label, QPushButton* exitBtn) {
    stackedWidget = stacked;
    backgroundLabel = label;
    exitButton = exitBtn;
}

void Animations::animateStackedWidgetTransition(int fromIndex, int toIndex) {
    QWidget* fromWidget = stackedWidget->widget(fromIndex);
    QWidget* toWidget = stackedWidget->widget(toIndex);
    int width = stackedWidget->width();

    toWidget->move((toIndex > fromIndex ? +1 : -1) * width, 0);
    toWidget->show();

    QPropertyAnimation* anim1 = new QPropertyAnimation(fromWidget, "pos");
    anim1->setDuration(400);
    anim1->setStartValue(fromWidget->pos());
    anim1->setEndValue(QPoint((toIndex > fromIndex ? -1 : 1) * width, 0));

    QPropertyAnimation* anim2 = new QPropertyAnimation(toWidget, "pos");
    anim2->setDuration(400);
    anim2->setStartValue(toWidget->pos());
    anim2->setEndValue(QPoint(0, 0));

    QParallelAnimationGroup* group = new QParallelAnimationGroup;
    group->addAnimation(anim1);
    group->addAnimation(anim2);

    connect(group, &QParallelAnimationGroup::finished, this, [=]() {
        stackedWidget->setCurrentIndex(toIndex);
        fromWidget->hide();
    });

    group->start(QAbstractAnimation::DeleteWhenStopped);
}

void Animations::fadeBetweenPages(int fromIndex, int toIndex) {
    QWidget* fromWidget = stackedWidget->widget(fromIndex);
    QWidget* toWidget = stackedWidget->widget(toIndex);

    QGraphicsOpacityEffect* fadeOut = new QGraphicsOpacityEffect(fromWidget);
    QGraphicsOpacityEffect* fadeIn = new QGraphicsOpacityEffect(toWidget);
    fromWidget->setGraphicsEffect(fadeOut);
    toWidget->setGraphicsEffect(fadeIn);
    toWidget->show();

    QPropertyAnimation* animOut = new QPropertyAnimation(fadeOut, "opacity");
    animOut->setDuration(400);
    animOut->setStartValue(1.0);
    animOut->setEndValue(0.0);

    QPropertyAnimation* animIn = new QPropertyAnimation(fadeIn, "opacity");
    animIn->setDuration(400);
    animIn->setStartValue(0.0);
    animIn->setEndValue(1.0);

    QParallelAnimationGroup* group = new QParallelAnimationGroup;
    group->addAnimation(animOut);
    group->addAnimation(animIn);

    connect(group, &QParallelAnimationGroup::finished, this, [=]() {
        stackedWidget->setCurrentIndex(toIndex);
        fromWidget->setGraphicsEffect(nullptr);
        toWidget->setGraphicsEffect(nullptr);
    });

    group->start(QAbstractAnimation::DeleteWhenStopped);
}

void Animations::animatedSwitch(int fromIndex, int toIndex, const QString& newImagePath) {
    QWidget* fromWidget = stackedWidget->widget(fromIndex);
    QWidget* toWidget = stackedWidget->widget(toIndex);
    int w = stackedWidget->width();

    toWidget->move(w, 0);
    toWidget->show();

    QGraphicsOpacityEffect* fadeOutEffect = new QGraphicsOpacityEffect(fromWidget);
    fromWidget->setGraphicsEffect(fadeOutEffect);
    QPropertyAnimation* fadeOut = new QPropertyAnimation(fadeOutEffect, "opacity");
    fadeOut->setDuration(400);
    fadeOut->setStartValue(1.0);
    fadeOut->setEndValue(0.0);
    fadeOut->setEasingCurve(QEasingCurve::OutQuad);

    QPropertyAnimation* slideIn = new QPropertyAnimation(toWidget, "pos");
    slideIn->setDuration(400);
    slideIn->setStartValue(QPoint(w, 0));
    slideIn->setEndValue(QPoint(0, 0));
    slideIn->setEasingCurve(QEasingCurve::OutCubic);

    QGraphicsOpacityEffect* fadeInEffect = new QGraphicsOpacityEffect(toWidget);
    toWidget->setGraphicsEffect(fadeInEffect);
    fadeInEffect->setOpacity(0.0);

    QPropertyAnimation* fadeIn = new QPropertyAnimation(fadeInEffect, "opacity");
    fadeIn->setDuration(400);
    fadeIn->setStartValue(0.0);
    fadeIn->setEndValue(1.0);
    fadeIn->setEasingCurve(QEasingCurve::InQuad);

    QLabel* tempImageOverlay = new QLabel(backgroundLabel->parentWidget());
    tempImageOverlay->setPixmap(QPixmap(newImagePath));
    tempImageOverlay->setScaledContents(true);
    tempImageOverlay->setGeometry(backgroundLabel->geometry());
    tempImageOverlay->raise();

    QGraphicsOpacityEffect* imageEffect = new QGraphicsOpacityEffect(tempImageOverlay);
    tempImageOverlay->setGraphicsEffect(imageEffect);
    imageEffect->setOpacity(0.0);

    QPropertyAnimation* imageFade = new QPropertyAnimation(imageEffect, "opacity");
    imageFade->setDuration(400);
    imageFade->setStartValue(0.0);
    imageFade->setEndValue(1.0);
    imageFade->setEasingCurve(QEasingCurve::OutQuad);

    QParallelAnimationGroup* group = new QParallelAnimationGroup;
    group->addAnimation(fadeOut);
    group->addAnimation(fadeIn);
    group->addAnimation(slideIn);
    group->addAnimation(imageFade);

    connect(group, &QParallelAnimationGroup::finished, this, [=]() {
        stackedWidget->setCurrentIndex(toIndex);
        fromWidget->setGraphicsEffect(nullptr);
        toWidget->setGraphicsEffect(nullptr);
        fromWidget->hide();
        backgroundLabel->setPixmap(QPixmap(newImagePath));
        tempImageOverlay->deleteLater();
    });

    group->start(QAbstractAnimation::DeleteWhenStopped);
}

// void Animations::animatedSwitchAdvanced(int fromIndex, int toIndex, const QString& newImagePath, const QString& newExitStyle) {
//     QWidget* fromWidget = stackedWidget->widget(fromIndex);
//     QWidget* toWidget = stackedWidget->widget(toIndex);
//     int w = stackedWidget->width();

//     toWidget->move(w, 0);
//     toWidget->setGraphicsEffect(new QGraphicsOpacityEffect);
//     fromWidget->setGraphicsEffect(new QGraphicsOpacityEffect);
//     toWidget->graphicsEffect()->setEnabled(true);
//     fromWidget->graphicsEffect()->setEnabled(true);
//     toWidget->show();

//     QPropertyAnimation* fadeOut = new QPropertyAnimation(fromWidget->graphicsEffect(), "opacity");
//     fadeOut->setDuration(500);
//     fadeOut->setStartValue(1.0);
//     fadeOut->setEndValue(0.0);
//     fadeOut->setEasingCurve(QEasingCurve::InOutCubic);

//     QPropertyAnimation* fadeIn = new QPropertyAnimation(toWidget->graphicsEffect(), "opacity");
//     fadeIn->setDuration(500);
//     fadeIn->setStartValue(0.0);
//     fadeIn->setEndValue(1.0);
//     fadeIn->setEasingCurve(QEasingCurve::InOutCubic);

//     QPropertyAnimation* slideIn = new QPropertyAnimation(toWidget, "pos");
//     slideIn->setDuration(600);
//     slideIn->setStartValue(QPoint(w, 0));
//     slideIn->setEndValue(QPoint(0, 0));
//     slideIn->setEasingCurve(QEasingCurve::OutBack);

//     QPropertyAnimation* zoomOut = new QPropertyAnimation(fromWidget, "geometry");
//     QRect outStart = fromWidget->geometry();
//     QRect outEnd = outStart.adjusted(20, 20, -20, -20);
//     zoomOut->setDuration(400);
//     zoomOut->setStartValue(outStart);
//     zoomOut->setEndValue(outEnd);
//     zoomOut->setEasingCurve(QEasingCurve::InBack);

//     QPropertyAnimation* zoomIn = new QPropertyAnimation(toWidget, "geometry");
//     QRect inEnd = toWidget->geometry();
//     QRect inStart = inEnd.adjusted(-30, -30, 30, 30);
//     zoomIn->setDuration(400);
//     zoomIn->setStartValue(inStart);
//     zoomIn->setEndValue(inEnd);
//     zoomIn->setEasingCurve(QEasingCurve::OutCubic);

//     QPixmap oldPixmap = backgroundLabel->pixmap();
//     QLabel* oldImageOverlay = new QLabel(backgroundLabel->parentWidget());
//     oldImageOverlay->setPixmap(oldPixmap);
//     oldImageOverlay->setGeometry(backgroundLabel->geometry());
//     oldImageOverlay->setScaledContents(true);
//     oldImageOverlay->raise();

//     QGraphicsOpacityEffect* oldEffect = new QGraphicsOpacityEffect(oldImageOverlay);
//     oldImageOverlay->setGraphicsEffect(oldEffect);
//     oldEffect->setOpacity(1.0);

//     QPropertyAnimation* oldFade = new QPropertyAnimation(oldEffect, "opacity");
//     oldFade->setDuration(500);
//     oldFade->setStartValue(1.0);
//     oldFade->setEndValue(0.0);
//     oldFade->setEasingCurve(QEasingCurve::InOutCubic);

//     QLabel* newImageOverlay = new QLabel(backgroundLabel->parentWidget());
//     newImageOverlay->setPixmap(QPixmap(newImagePath));
//     newImageOverlay->setGeometry(backgroundLabel->geometry());
//     newImageOverlay->setScaledContents(true);
//     newImageOverlay->raise();

//     QGraphicsOpacityEffect* newEffect = new QGraphicsOpacityEffect(newImageOverlay);
//     newImageOverlay->setGraphicsEffect(newEffect);
//     newEffect->setOpacity(0.0);

//     QPropertyAnimation* newFade = new QPropertyAnimation(newEffect, "opacity");
//     newFade->setDuration(500);
//     newFade->setStartValue(0.0);
//     newFade->setEndValue(1.0);
//     newFade->setEasingCurve(QEasingCurve::InOutCubic);

//     QParallelAnimationGroup* photoGroup = new QParallelAnimationGroup;
//     photoGroup->addAnimation(oldFade);
//     photoGroup->addAnimation(newFade);

//     QGraphicsOpacityEffect* exitEffect = new QGraphicsOpacityEffect(exitButton);
//     exitButton->setGraphicsEffect(exitEffect);

//     QPropertyAnimation* exitFadeOut = new QPropertyAnimation(exitEffect, "opacity");
//     exitFadeOut->setDuration(200);
//     exitFadeOut->setStartValue(1.0);
//     exitFadeOut->setEndValue(0.0);

//     QPropertyAnimation* exitFadeIn = new QPropertyAnimation(exitEffect, "opacity");
//     exitFadeIn->setDuration(200);
//     exitFadeIn->setStartValue(0.0);
//     exitFadeIn->setEndValue(1.0);

//     connect(exitFadeOut, &QPropertyAnimation::finished, this, [=]() {
//         exitButton->setStyleSheet(newExitStyle);
//         exitFadeIn->start();
//     });

//     QParallelAnimationGroup* masterGroup = new QParallelAnimationGroup;
//     masterGroup->addAnimation(fadeOut);
//     masterGroup->addAnimation(fadeIn);
//     masterGroup->addAnimation(slideIn);
//     masterGroup->addAnimation(zoomOut);
//     masterGroup->addAnimation(zoomIn);
//     masterGroup->addAnimation(photoGroup);
//     masterGroup->addAnimation(exitFadeOut);

//     connect(masterGroup, &QParallelAnimationGroup::finished, this, [=]() {
//         stackedWidget->setCurrentIndex(toIndex);
//         fromWidget->setGraphicsEffect(nullptr);
//         toWidget->setGraphicsEffect(nullptr);
//         backgroundLabel->setPixmap(QPixmap(newImagePath));
//         oldImageOverlay->deleteLater();
//         newImageOverlay->deleteLater();
//         exitButton->setGraphicsEffect(nullptr);
//     });

//     masterGroup->start(QAbstractAnimation::DeleteWhenStopped);
// }
void Animations::animatedSwitchAdvanced(int fromIndex, int toIndex, const QString& newImagePath, const QString& newExitStyle) {
    if (!stackedWidget || fromIndex == toIndex) return;

    QWidget* fromWidget = stackedWidget->widget(fromIndex);
    QWidget* toWidget = stackedWidget->widget(toIndex);
    const int w = stackedWidget->width();

    toWidget->setGraphicsEffect(new QGraphicsOpacityEffect);
    fromWidget->setGraphicsEffect(new QGraphicsOpacityEffect);
    toWidget->graphicsEffect()->setEnabled(true);
    fromWidget->graphicsEffect()->setEnabled(true);

    // Prepare new widget position and geometry
    toWidget->move(w, 0);
    toWidget->resize(stackedWidget->size());
    toWidget->show();

    // Fade animations
    auto* fadeOut = new QPropertyAnimation(fromWidget->graphicsEffect(), "opacity");
    fadeOut->setDuration(400);
    fadeOut->setStartValue(1.0);
    fadeOut->setEndValue(0.0);
    fadeOut->setEasingCurve(QEasingCurve::OutQuad);

    auto* fadeIn = new QPropertyAnimation(toWidget->graphicsEffect(), "opacity");
    fadeIn->setDuration(400);
    fadeIn->setStartValue(0.0);
    fadeIn->setEndValue(1.0);
    fadeIn->setEasingCurve(QEasingCurve::InQuad);

    // Slide in the new widget
    auto* slideIn = new QPropertyAnimation(toWidget, "pos");
    slideIn->setDuration(500);
    slideIn->setStartValue(QPoint(w, 0));
    slideIn->setEndValue(QPoint(0, 0));
    slideIn->setEasingCurve(QEasingCurve::OutCubic);

    // Background cross-fade
    QLabel* overlayOld = new QLabel(backgroundLabel->parentWidget());
    overlayOld->setPixmap(backgroundLabel->pixmap().scaled(backgroundLabel->size()));
    overlayOld->setGeometry(backgroundLabel->geometry());
    overlayOld->raise();

    QLabel* overlayNew = new QLabel(backgroundLabel->parentWidget());
    overlayNew->setPixmap(QPixmap(newImagePath).scaled(backgroundLabel->size()));
    overlayNew->setGeometry(backgroundLabel->geometry());
    overlayNew->setGraphicsEffect(new QGraphicsOpacityEffect);
    overlayNew->graphicsEffect()->setEnabled(true);
    overlayNew->graphicsEffect()->setProperty("opacity", 0.0);
    overlayNew->raise();

    auto* fadeOverlay = new QPropertyAnimation(overlayNew->graphicsEffect(), "opacity");
    fadeOverlay->setDuration(600);
    fadeOverlay->setStartValue(0.0);
    fadeOverlay->setEndValue(1.0);
    fadeOverlay->setEasingCurve(QEasingCurve::InOutCubic);

    // Exit button fade style switch
    QGraphicsOpacityEffect* exitEffect = new QGraphicsOpacityEffect(exitButton);
    exitButton->setGraphicsEffect(exitEffect);

    auto* exitFadeOut = new QPropertyAnimation(exitEffect, "opacity");
    exitFadeOut->setDuration(200);
    exitFadeOut->setStartValue(1.0);
    exitFadeOut->setEndValue(0.0);

    auto* exitFadeIn = new QPropertyAnimation(exitEffect, "opacity");
    exitFadeIn->setDuration(200);
    exitFadeIn->setStartValue(0.0);
    exitFadeIn->setEndValue(1.0);

    connect(exitFadeOut, &QPropertyAnimation::finished, this, [=]() {
        exitButton->setStyleSheet(newExitStyle);
        exitFadeIn->start();
    });

    // Animation group
    auto* group = new QParallelAnimationGroup;
    group->addAnimation(fadeOut);
    group->addAnimation(fadeIn);
    group->addAnimation(slideIn);
    group->addAnimation(fadeOverlay);
    group->addAnimation(exitFadeOut);

    connect(group, &QParallelAnimationGroup::finished, this, [=]() {
        stackedWidget->setCurrentIndex(toIndex);
        fromWidget->setGraphicsEffect(nullptr);
        toWidget->setGraphicsEffect(nullptr);
        overlayOld->deleteLater();
        overlayNew->deleteLater();
        backgroundLabel->setPixmap(QPixmap(newImagePath));
        exitButton->setGraphicsEffect(nullptr);
        fromWidget->hide();
    });

    group->start(QAbstractAnimation::DeleteWhenStopped);
}

