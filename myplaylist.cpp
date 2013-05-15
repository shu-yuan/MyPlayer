#include "myplaylist.h"
#include <QStringList>
#include <QContextMenuEvent>
#include <QMenu>

MyPlaylist::MyPlaylist(QWidget *parent) :
    QTableWidget(parent)
{
    setWindowTitle(tr("播放列表"));
    setWindowFlags(Qt::Window | Qt::WindowTitleHint);

    resize(320, 400);
    setMinimumWidth(320);
    setMaximumWidth(320);

    setRowCount(0);
    setColumnCount(3);

    QStringList list;
    list << tr("标题") << tr("艺术家") << tr("长度");
    setHorizontalHeaderLabels(list);

    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);

    setShowGrid(false);
}

void MyPlaylist::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu;
    menu.addAction(tr("清空列表"), this, SLOT(clearPlayList()));
    menu.exec(event->globalPos());
}

void MyPlaylist::clearPlayList()
{
    if (rowCount())
        removeRow(0);
}

void MyPlaylist::closeEvent(QCloseEvent *event)
{
    if (isVisible())
        hide();
    event->ignore();
}
