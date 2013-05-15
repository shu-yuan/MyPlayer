#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QFrame>
#include <qaction>

#include <Phonon>
#include "myplaylist.h"
#include <QList>

class MyPlaylist;

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

    void sourceChanged(const Phonon::MediaSource &source);
    void aboutToFinish();
    void metaStateChanged(Phonon::State newState, Phonon::State oldState);
    void tableClicked(int);
    void clearSources();

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

    MyPlaylist *playlist;
    Phonon::MediaObject * metaInformationResolver;
    QList <Phonon::MediaSource> sources;
    void changeActionState();
};

#endif // MYWIDGET_H
