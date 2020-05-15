#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QList>
#include <QItemSelectionModel>
#include <QColorDialog>

#include "themeitem.h"
#include "themeloader.h"
#include "themetablemodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void initMainMenu();
    QList<ThemeItem *> *theme = nullptr;
    QList<int> *findedPositions = nullptr;
    int currentSearchPosition = -1;
    int findedCount = 0;
    ThemeTableModel *model = nullptr;
    ThemeItem *mCurrentThemeItem = nullptr;
    QItemSelectionModel *selectionModelTableViewColors = nullptr;
    QString openedThemePath;
    QString workingDirectory = ".";

    void initTableViewColors();
    int qColorToRaw(QColor color);
    void saveTheme(QString filePath);
    void gotoSearchPosition();

private slots:
    void openTheme();
    void saveThemeAs();
    void saveTheme();
    void doubleClicked1(const QModelIndex &index);
    void searchName(const QString text);
    void prevSearch();
    void nextSearch();
};

#endif // MAINWINDOW_H
