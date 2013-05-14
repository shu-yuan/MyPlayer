#include "mywidget.h"
#include <QToolBar>
#include <QVBoxLayout>
#include <QTime>
#include <QDebug>
#include <QMessageBox>
#include <QFileInfo>

MyWidget::MyWidget(QWidget *parent)
    : QWidget(parent)
{
    initPlayer();
}

MyWidget::~MyWidget()
{
    
}

void MyWidget::initPlayer()
{
    //init Widget
    setWindowTitle(tr("MyPlayer音乐播放器"));
    setWindowIcon(QIcon(":/images/icon.png"));
    setMinimumSize(320, 160);
    setMaximumSize(320, 160);
    //init UI
    //--top label
    topLabel = new QLabel(tr("Beyond..."), this);
    topLabel->setTextFormat(Qt::RichText);
    topLabel->setOpenExternalLinks(true);
    topLabel->setAlignment(Qt::AlignCenter);

    //creator media object
    mediaObject = new Phonon::MediaObject(this);
    Phonon::AudioOutput *audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    Phonon::createPath(mediaObject, audioOutput);

    //--progress slider
    Phonon::SeekSlider *seekSlider = new Phonon::SeekSlider(mediaObject, this);

    connect(mediaObject, SIGNAL(tick(qint64)), this, SLOT(updateTime(qint64)));

    //--time label
    timeLabel = new QLabel(tr("00:00 / 00:00"), this);
    timeLabel->setToolTip(tr("当前时间 / 总时间"));
    timeLabel->setAlignment(Qt::AlignCenter);
    timeLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    //--PL display action
    QAction *PLAction = new QAction(tr("PL"), this);
    PLAction->setShortcut(QKeySequence("F4"));
    PLAction->setToolTip(tr("播放列表(F4)"));
    connect(PLAction, SIGNAL(triggered()), this, SLOT(setPlaylistShown()));

    //--LRC display action
    QAction *LRCAction = new QAction(tr("LRC"), this);
    LRCAction->setShortcut(QKeySequence("F2"));
    LRCAction->setToolTip(tr("桌面歌词(F2)"));
    connect(LRCAction, SIGNAL(triggered()), this, SLOT(setLrcShown()));

    QToolBar *widgetBar = new QToolBar(this);
    widgetBar->addAction(PLAction);
    widgetBar->addSeparator();
    widgetBar->addWidget(timeLabel);
    widgetBar->addSeparator();
    widgetBar->addAction(LRCAction);

    //--play action
    playAction = new QAction(this);
    playAction->setIcon(QIcon(":/images/play.png"));
    playAction->setShortcut(QKeySequence("F5"));
    playAction->setToolTip(tr("播放(F5)"));
    connect(playAction, SIGNAL(triggered()), this, SLOT(setPaused()));

    //--stop action
    stopAction = new QAction(this);
    stopAction->setIcon(QIcon(":/images/stop.png"));
    stopAction->setShortcut(QKeySequence("F6"));
    stopAction->setToolTip(tr("停止(F6)"));
    connect(stopAction, SIGNAL(triggered()), mediaObject, SLOT(stop()));

    //--skip back action
    skipBackwardAction = new QAction(this);
    skipBackwardAction->setIcon(QIcon(":/images/skipBackward.png"));
    skipBackwardAction->setShortcut(QKeySequence("Ctrl+Left"));
    skipBackwardAction->setToolTip(tr("上一首(Ctrl+Left)"));
    connect(skipBackwardAction, SIGNAL(triggered()), this, SLOT(skipBackward()));

    //--skip forward action
    skipForwardAction = new QAction(this);
    skipForwardAction->setIcon(QIcon(":/images/skipForward.png"));
    skipForwardAction->setShortcut(QKeySequence("Ctrl+Right"));
    skipForwardAction->setToolTip(tr("下一首(Ctrl+Right)"));
    connect(skipForwardAction, SIGNAL(triggered()), this, SLOT(skipForward()));

    //--open file
    QAction *openAction = new QAction(this);
    openAction->setIcon(QIcon(":/images/open.png"));
    openAction->setShortcut(QKeySequence("Ctrl+o"));
    openAction->setText(tr("播放文件(Ctrl+O)"));
    connect(openAction, SIGNAL(triggered()), this, SLOT(openFile()));

    //add volumn slider
    Phonon::VolumeSlider *volumeSlider = new Phonon::VolumeSlider(audioOutput, this);
    volumeSlider->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    QToolBar *toolBar = new QToolBar(this);
    toolBar->addAction(playAction);
    toolBar->addSeparator();
    toolBar->addAction(stopAction);
    toolBar->addSeparator();
    toolBar->addAction(skipBackwardAction);
    toolBar->addSeparator();
    toolBar->addAction(skipForwardAction);
    toolBar->addSeparator();
    toolBar->addWidget(volumeSlider);
    toolBar->addSeparator();
    toolBar->addAction(openAction);

    //main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(topLabel);
    mainLayout->addWidget(seekSlider);
    mainLayout->addWidget(widgetBar);
    mainLayout->addWidget(toolBar);

    mediaObject->setCurrentSource(Phonon::MediaSource("../MyPlayer/wenbie.mp3"));

    connect(mediaObject, SIGNAL(stateChanged(Phonon::State,Phonon::State)),
            this, SLOT(stateChanged(Phonon::State, Phonon::State)));

}

void MyWidget::updateTime(qint64 time)
{
    qint64 totalTimeValue = mediaObject->totalTime();
    QTime totalTime(0, (totalTimeValue / 60000) % 60, (totalTimeValue / 10000) % 60);
    QTime currentTime(0, (time / 60000) % 60, (time / 1000) % 60);
    QString str = currentTime.toString("mm:ss") + " / " + totalTime.toString("mm:ss");
    timeLabel->setText(str);
}

void MyWidget::setPlaylistShown()
{

}


void MyWidget::setLrcShown()
{

}

void MyWidget::setPaused()
{
    if (mediaObject->state() == Phonon::PlayingState)
        mediaObject->pause();
    else
        mediaObject->play();
}

void MyWidget::skipBackward()
{

}

void MyWidget::skipForward()
{

}

void MyWidget::openFile()
{

}

void MyWidget::stateChanged(Phonon::State newState, Phonon::State oldState)
{
    switch (newState) {
    case Phonon::ErrorState :
        if(mediaObject->errorType() == Phonon::FatalError) {
            QMessageBox::warning(this, tr("致命错误"), mediaObject->errorString());
        } else {
            QMessageBox::warning(this, tr("错误"), mediaObject->errorString());
        }
        break;
    case Phonon::PlayingState :
        stopAction->setEnabled(true);
        playAction->setIcon(QIcon(tr(":/images/pause.png")));
        playAction->setText(tr(" 暂停(F5)"));
        topLabel->setText(QFileInfo(mediaObject->currentSource().fileName()).baseName());
        break;
    case Phonon::StoppedState :
        stopAction->setEnabled(false);
        playAction->setIcon(QIcon(":/images/play.png"));
        playAction->setText(tr("播放(F5)"));
        topLabel->setText(tr("beyond..."));
        timeLabel->setText(tr("00:00 / 00:00"));
        break;
    case Phonon::PausedState :
        stopAction->setEnabled(false);
        playAction->setIcon(QIcon(":/images/play.png"));
        playAction->setText(tr("播放(F5)"));
        topLabel->setText(QFileInfo(mediaObject->currentSource().fileName()).baseName() + tr("已暂停！"));
        break;
    case Phonon::BufferingState :
        break;
    default:
        break;
    }
}
