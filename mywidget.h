#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QFrame>
#include <qaction>

#include <Phonon>

class MyWidget : public QWidget
{
    Q_OBJECT
    
public:
    MyWidget(QWidget *parent = 0);
    ~MyWidget();


private slots:
    void setPlaylistShown();
    void setLrcShown();
    void setPaused();
    void skipBackward();
    void skipForward();
    void openFile();

    void updateTime(qint64 time);
    void stateChanged(Phonon::State, Phonon::State);

private:
    void initPlayer();

private:
    Phonon::MediaObject *mediaObject;
    QAction *playAction;
    QAction *stopAction;
    QAction *skipBackwardAction;
    QAction *skipForwardAction;
    QLabel *topLabel;
    QLabel *timeLabel;
};

#endif // MYWIDGET_H
