#ifndef CONTEXTMENUTABLE_H
#define CONTEXTMENUTABLE_H

#include <QTableView>
#include <QWidget>
#include <QMenu>
#include <QDebug>
#include <QAction>
#include <QContextMenuEvent>

class ContextMenuTable : public QTableView
{
    Q_OBJECT
public:
    ContextMenuTable(QWidget *parent);
    ~ContextMenuTable();
public slots:
    void copyViewName(QAction* pAction);
private:
    QMenu* m_pmnu = nullptr;
protected:
    virtual void contextMenuEvent(QContextMenuEvent* pe);
};

#endif // CONTEXTMENUTABLE_H
