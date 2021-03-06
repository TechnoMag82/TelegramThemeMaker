#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QList>
#include <QItemSelectionModel>
#include <QColorDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QMenu>
#include <QClipboard>

#include "themeitem.h"
#include "themeloader.h"
#include "themetablemodel.h"
#include "dialogs/aboutdialog.h"
#include "utils.h"
#include "themeitemdelegate.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Ui::MainWindow *ui;
    QList<ThemeItem *> *theme = nullptr;
    QList<ThemeItem *> *findedThemeItems = nullptr;
    QMenu *popupMenuTableColors = nullptr;
    QLabel *mStatusBarText = nullptr;
    bool wallpaperChanged = false;
    ThemeTableModel *model = nullptr;
    ThemeItem *mCurrentThemeItem = nullptr;
    QItemSelectionModel *selectionModelTableViewColors = nullptr;
    QString openedThemePath;
    QString workingDirectory = ".";
    QString wpPath = "";

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private:
        void initMainMenu();
        void initTableViewColors();
        void saveTheme(QString filePath);
        void setThemeChangeStatus();
        void createTableColorsPopupMenu();
        void initStatusBar();
        bool isAnyModified();

    private slots:
        void openTheme();
        void saveThemeAs();
        void saveTheme();
        void doubleClicked1(const QModelIndex &index);
        void searchName(const QString text);
        void aboutDialog();
        void deleteWallpaper();
        void selectWallpaper();
        void addViewColor();
        void deleteViewColor();
        void clicked1(const QModelIndex &index);
        void exitApp();
        void customMenuRequested(QPoint pos);
        void copyViewName();
        void copyViewColor();
        void resetColorToDefault();
};

#endif // MAINWINDOW_H
