#include "mywidget.h"
#include <QToolBar>
#include <QVBoxLayout>
#include <QTime>
#include <QDebug>
#include <QMessageBox>
#include <QFileInfo>

#include "myplaylist.h"
#include <QFileDialog>
#include <QDesktopServices>


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

    playlist = new MyPlaylist;

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

    playlist = new MyPlaylist(this);
    connect(playlist, SIGNAL(cellClicked(int,int)), this, SLOT(tableClicked(int)));
    connect(playlist, SIGNAL(playListClear()), this, SLOT(clearSources()));

    connect(mediaObject, SIGNAL(stateChanged(Phonon::State,Phonon::State)),
            this, SLOT(stateChanged(Phonon::State, Phonon::State)));

    metaInformationResolver = new Phonon::MediaObject(this);

    Phonon::AudioOutput *metaInformationAudioOutput =
            new Phonon::AudioOutput(Phonon::MusicCategory, this);
    Phonon::createPath(metaInformationResolver, metaInformationAudioOutput);
    connect(metaInformationResolver, SIGNAL(stateChanged(Phonon::State,Phonon::State)),
            this, SLOT(metaStateChanged(Phonon::State,Phonon::State)));
    connect(mediaObject, SIGNAL(currentSourceChanged(Phonon::MediaSource)),
            this, SLOT(sourceChanged(Phonon::MediaSource)));
    connect(mediaObject, SIGNAL(aboutToFinish()), this, SLOT(aboutToFinish()));

    //init
    playAction->setEnabled(false);
    stopAction->setEnabled(false);
    skipBackwardAction->setEnabled(false);
    skipForwardAction->setEnabled(false);
    topLabel->setFocus();
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
    if (playlist->isHidden()) {
        playlist->move(frameGeometry().bottomLeft());
        playlist->show();
    } else {
        playlist->hide();
    }
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
    int index = sources.indexOf(mediaObject->currentSource());
    mediaObject->setCurrentSource(index - 1);
    mediaObject->play();
}

void MyWidget::skipForward()
{
    int index = sources.indexOf(mediaObject->currentSource());
    mediaObject->setCurrentSource(index + 1);
    mediaObject->play();
}

void MyWidget::openFile()
{
    QStringList list = QFileDialog::getOpenFileNames(this, tr("打开音乐文件"),
                                                     QDesktopServices::storageLocation(QDesktopServices::MusicLocation));
    if (list.isEmpty()) {
        return;
    }

    int index = sources.size();

    foreach (QString string, list) {
        Phonon::MediaSource source(string);
        sources.append(source);
    }

    if (!sources.isEmpty()) {
        metaInformationResolver->setCurrentSource(sources.at(index));
    }
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

void MyWidget::metaStateChanged(Phonon::State newState, Phonon::State oldState)
{
    if (newState == Phonon::ErrorState) {
        QMessageBox::warning(this, tr("打开文件出错"), metaInformationResolver->errorString());
        while (!sources.isEmpty() && !(sources.takeLast() ==
                                       metaInformationResolver->currentSource()))
        {};
        return;
    }

    if (newState != Phonon::StoppedState && newState != Phonon::PausedState)
        return;

    if (metaInformationResolver->currentSource().type() == Phonon::MediaSource::Invalid)
        return;

    QMap<QString, QString> metaData = metaInformationResolver->metaData();

    QString title = metaData.value("TITLE");
    if (title == "") {
        QString str = metaInformationResolver->currentSource().fileName();
        title = QFileInfo(str).baseName();
    }

    QTableWidgetItem *titleItem = new QTableWidgetItem(title);

    titleItem->setFlags(titleItem->flags() ^ Qt::ItemIsEditable);

    QTableWidgetItem *artistItem = new QTableWidgetItem(metaData.value("ARTIST"));
    artistItem->setFlags(artistItem->flags() ^ Qt::ItemIsEditable);

    qint64 totalTime = metaInformationResolver->totalTime();
    QTime time(0, (totalTime / 60000) % 60, (totalTime / 1000) % 60);
    QTableWidgetItem *timeItem = new QTableWidgetItem(time.toString("mm:ss"));


    int currentRow = playlist->rowCount();
    playlist->insertRow(currentRow);
    playlist->setItem(currentRow, 0, titleItem);
    playlist->setItem(currentRow, 1, artistItem);
    playlist->setItem(currentRow, 2, timeItem);

    int index = sources.indexOf(metaInformationResolver->currentSource()) + 1;
    if (sources.size() > index) {
        metaInformationResolver->setCurrentSource(sources.at(index));
    } else {
        if (playlist->selectedItems().isEmpty()) {
            if (mediaObject->state() != Phonon::PlayingState &&
                    mediaObject->state() != Phonon::PausedState) {
                mediaObject->setCurrentSource(sources.at(0));
            } else {
                playlist->selectRow(0);
                changeActionState();
            }
        } else {
            changeActionState();
        }
    }
}

void MyWidget::changeActionState()
{
    if (sources.count() == 0) {
        if (mediaObject->state() != Phonon::PlayingState &&
                mediaObject->state() != Phonon::PausedState) {
            playAction->setEnabled(false);
            stopAction->setEnabled(false);
        }
        skipBackwardAction->setEnabled(false);
        skipForwardAction->setEnabled(false);
    } else {
        playAction->setEnabled(true);
        stopAction->setEnabled(true);
        if (sources.count() == 1) {
            skipBackwardAction->setEnabled(false);
            skipForwardAction->setEnabled(false);
        } else {
            skipBackwardAction->setEnabled(true);
            skipForwardAction->setEnabled(true);
            int index = playlist->currentRow();

            if (index = 0)
                skipBackwardAction->setEnabled(false);

            if (index + 1 == sources.count())
                skipForwardAction->setEnabled(true);
        }
    }
}

void MyWidget::sourceChanged(const Phonon::MediaSource &source)
{
    int index = sources.indexOf(source);
    playlist->selectRow(index);
    changeActionState();
}

void MyWidget::aboutToFinish()
{
    int index = sources.indexOf(mediaObject->currentSource()) + 1;
    if (sources.size() > index) {
        mediaObject->enqueue(sources.at(index));
        mediaObject->seek(mediaObject->totalTime());
    } else {
        mediaObject->stop();
    }
}

void MyWidget::tableClicked(int row)
{
    bool wasPlaying = mediaObject->state() == Phonon::PlayingState;
    mediaObject->stop();
    mediaObject->clearQueue();

    if (row >= sources.size())
        return;

    mediaObject->setCurrentSource(sources.at(row));

    if (wasPlaying)
        mediaObject->play();
}

void MyWidget::clearSources()
{
    sources.clear();
    changeActionState();
}
