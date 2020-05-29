#include "contextmenutable.h"

ContextMenuTable::ContextMenuTable(QWidget *parent)
    : QTableView(parent)
{
    m_pmnu = new QMenu(this);
    QAction *copyViewName = m_pmnu->addAction("Copy view name");
    m_pmnu->addAction("Copy view color");

    connect(copyViewName, SIGNAL(triggered(QAction*)), SLOT(copyViewName(QAction*)));
}

ContextMenuTable::~ContextMenuTable()
{
    delete m_pmnu;
}

void ContextMenuTable::copyViewName(QAction *pAction)
{
    qDebug() << "copy";
}

void ContextMenuTable::contextMenuEvent(QContextMenuEvent *pe)
{
    m_pmnu->exec(pe->globalPos());
}
