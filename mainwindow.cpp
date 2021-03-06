#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableViewColors->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableViewColors->verticalHeader()->setFixedWidth(60);
    ui->tableViewColors->setItemDelegate(new ThemeItemDelegate(ui->tableViewColors));
    ui->tableViewColors->setEditTriggers(QAbstractItemView::SelectedClicked);
    ui->labelWP->setVisible(false);
    ui->tableViewColors->setContextMenuPolicy(Qt::CustomContextMenu);
    theme = new QList<ThemeItem*>();
    findedThemeItems = new QList<ThemeItem*>();
    initMainMenu();
    initTableViewColors();
    createTableColorsPopupMenu();
    connect(ui->tableViewColors, SIGNAL(customContextMenuRequested(QPoint)),
               SLOT(customMenuRequested(QPoint)));
}


void MainWindow::initStatusBar()
{
    if (mStatusBarText == nullptr) {
        mStatusBarText = new QLabel(ui->statusBar);
        mStatusBarText->setFrameStyle(QFrame::Box);
        ui->statusBar->addPermanentWidget(mStatusBarText);
    }
    mStatusBarText->setText(QString(tr("Items: %1")).arg(theme->count()));
}

bool MainWindow::isAnyModified()
{
    int size = theme->count();
    for (int i = 0; i < size; i++) {
        if (theme->at(i)->isModified()) {
            return true;
        }
    }
    return false;
}

void MainWindow::customMenuRequested(QPoint pos){
    if (popupMenuTableColors != nullptr && model != nullptr) {
        QModelIndex index = ui->tableViewColors->indexAt(pos);
        mCurrentThemeItem = model->getByIndex(index);
        popupMenuTableColors->popup(ui->tableViewColors->viewport()->mapToGlobal(pos));
    }
}

void MainWindow::copyViewName()
{
    if (mCurrentThemeItem != nullptr) {
        QApplication::clipboard()->setText(mCurrentThemeItem->name);
    }
}

void MainWindow::copyViewColor()
{
    if (mCurrentThemeItem != nullptr) {
        QApplication::clipboard()->setText(mCurrentThemeItem->getColor().name());
    }
}

void MainWindow::resetColorToDefault()
{
    if (mCurrentThemeItem != nullptr) {
        mCurrentThemeItem->resetToDefault();
        setThemeChangeStatus();
    }
}

MainWindow::~MainWindow()
{
    ThemeLoader *loader = new ThemeLoader();
    loader->deleteTempImageWallpaper(wpPath);
    delete loader;
    theme->clear();
    findedThemeItems->clear();
    delete mStatusBarText;
    delete popupMenuTableColors;
    delete findedThemeItems;
    delete theme;
    if (model != nullptr) {
        delete model;
    }
    delete ui;
}

void MainWindow::initMainMenu()
{
    connect(ui->actionOpen, SIGNAL(triggered()),
                this, SLOT(openTheme()));

    connect(ui->actionSave, SIGNAL(triggered()),
                this, SLOT(saveTheme()));

    connect(ui->actionSaveAs, SIGNAL(triggered()),
                this, SLOT(saveThemeAs()));

    connect(ui->actionDeleteWallpaper, SIGNAL(triggered()),
                this, SLOT(deleteWallpaper()));

    connect(ui->actionSelectWallpaper, SIGNAL(triggered()),
                this, SLOT(selectWallpaper()));

    connect(ui->actionAbout, SIGNAL(triggered()),
                this, SLOT(aboutDialog()));

    connect(ui->actionAddColor, SIGNAL(triggered()),
                this, SLOT(addViewColor()));

    connect(ui->actionDeleteColor, SIGNAL(triggered()),
                this, SLOT(deleteViewColor()));

    connect(ui->actionExit, SIGNAL(triggered()),
                this, SLOT(exitApp()));
}

void MainWindow::openTheme()
{
    QString themePath = QFileDialog::getOpenFileName(this, tr("Open Theme"), workingDirectory, "*.attheme");
    if (!themePath.isNull() && !themePath.isEmpty()) {
        ThemeLoader *loader = new ThemeLoader();
        loader->deleteTempImageWallpaper(wpPath);
        loader->loadTheme(themePath, *theme);
        wpPath = loader->wallpaperPath();
        if (wpPath.length() > 0) {
            QPixmap img(wpPath);
            img.scaledToWidth(ui->labelWP->width());
            ui->labelWP->setPixmap(img);
            ui->labelWP->setVisible(true);
        } else {
            ui->labelWP->setText(tr("No Wallpaper"));
            ui->labelWP->setVisible(false);
        }
        if (model == nullptr) {
            model = new ThemeTableModel(ui->tableViewColors, QCoreApplication::applicationDirPath());
        }
        model->assignData(theme);
        ui->tableViewColors->setModel(model);
        ui->tableViewColors->setColumnWidth(0, 400);
        delete loader;
        QFileInfo file(themePath);
        QDir dir = file.absoluteDir();
        workingDirectory = dir.absolutePath();
        openedThemePath = themePath;
        wallpaperChanged = false;
        setThemeChangeStatus();
        initStatusBar();
    }
}

void MainWindow::saveThemeAs()
{
    if (theme != nullptr && theme->size() > 0) {
        QString themePath = QFileDialog::getSaveFileName(this, tr("Save Theme"), workingDirectory, "*.attheme");
        if (!themePath.isNull() && !themePath.isEmpty()) {
            if (!themePath.endsWith(".attheme")) {
                themePath.append(".attheme");
            }
            saveTheme(themePath);
            QFileInfo file(themePath);
            QDir dir = file.absoluteDir();
            workingDirectory = dir.absolutePath();
            openedThemePath = themePath;
            setWindowTitle(QString("%1 - [%2]").arg(QCoreApplication::applicationName(), file.fileName()));
        }
    }
}

void MainWindow::initTableViewColors()
{
    connect(ui->tableViewColors, SIGNAL(doubleClicked(const QModelIndex &)),
            this, SLOT(doubleClicked1(const QModelIndex &)));

    connect(ui->tableViewColors, SIGNAL(clicked(const QModelIndex &)),
            this, SLOT(clicked1(const QModelIndex &)));

    connect(ui->lineEditSearch, SIGNAL(textEdited(QString)),
            this, SLOT(searchName(QString)));
}

void MainWindow::saveTheme(QString filePath)
{
    ThemeLoader *loader = new ThemeLoader();
    if (wpPath.length() > 0) {
        loader->saveTheme(filePath, *theme, wpPath);
    } else {
        loader->saveTheme(filePath, *theme);
    }
    delete loader;
    wallpaperChanged = false;
    setThemeChangeStatus();
}

void MainWindow::setThemeChangeStatus()
{
    if (wallpaperChanged || isAnyModified()) {
        setWindowTitle(QString("%1 - [%2 *]").arg(QCoreApplication::applicationName(), openedThemePath));
    } else {
        setWindowTitle(QString("%1 - [%2]").arg(QCoreApplication::applicationName(), openedThemePath));
    }
}

void MainWindow::createTableColorsPopupMenu()
{
    popupMenuTableColors = new QMenu(this);
    QAction *action = new QAction(tr("Copy view name"), this);
    popupMenuTableColors->addAction(action);
    connect(action, &QAction::triggered, this, &MainWindow::copyViewName);
    action = new QAction(tr("Copy view color"), this);
    popupMenuTableColors->addAction(action);
    connect(action, &QAction::triggered, this, &MainWindow::copyViewColor);
    action = new QAction(tr("Reset color to default"), this);
    popupMenuTableColors->addAction(action);
    connect(action, &QAction::triggered, this, &MainWindow::resetColorToDefault);
}

void MainWindow::saveTheme()
{
    if (theme != nullptr && theme->size() > 0) {
        saveTheme(openedThemePath);
    }
}

void MainWindow::doubleClicked1(const QModelIndex &index)
{
    if (index.column() == 0) {
        mCurrentThemeItem = model->getByIndex(index);
        QColor color = QColorDialog::getColor(mCurrentThemeItem->getColor(), this,
                                              tr("Color for ") + mCurrentThemeItem->name,
                                              QColorDialog::ShowAlphaChannel);
        if (color.isValid()) {
            mCurrentThemeItem->setRawColor(qColorToRaw(color));
            model->refreshData();
            setThemeChangeStatus();
        }
    }
}

void MainWindow::searchName(const QString text)
{
    int themeSize = theme->size();
    if (theme != nullptr && themeSize > 0 && text.size() >= 3) {
        findedThemeItems->clear();
        for (int i = 0; i < themeSize; i++) {
            ThemeItem *item = theme->at(i);
            if (item->name.contains(text, Qt::CaseInsensitive)) {
                findedThemeItems->append(item);
            }
        }
        if (findedThemeItems->size() > 0) {
            ui->labelFinded->setText(QString(tr("found %1 items"))
                                     .arg(QString::number(findedThemeItems->size())));
            model->assignData(findedThemeItems);
        } else {
            model->assignData(theme);
            ui->labelFinded->setText(QString(tr("found 0 items")));
        }
    } else {
        model->assignData(theme);
        ui->labelFinded->setText(QString(tr("found 0 items")));
    }
}

void MainWindow::aboutDialog()
{
    AboutDialog *aboutDialog = new AboutDialog(this);
    aboutDialog->exec();
    delete aboutDialog;
}

void MainWindow::deleteWallpaper()
{
    if (theme != nullptr && theme->size() > 0) {
        ThemeLoader *loader = new ThemeLoader();
        loader->deleteTempImageWallpaper(wpPath);
        delete loader;
        wpPath = "";
        ui->labelWP->setText(tr("No Wallpaper"));
        ui->labelWP->setVisible(false);
        wallpaperChanged = true;
        setThemeChangeStatus();
    }
}

void MainWindow::selectWallpaper()
{
    if (theme != nullptr && theme->size() > 0) {
        QString mTempPath = wpPath;
        wpPath = QFileDialog::getOpenFileName(this, tr("Open Wallpaper"), workingDirectory, "*.jpg");
        if (!wpPath.isNull() && !wpPath.isEmpty()) {
            QPixmap img(wpPath);
            img.scaledToWidth(ui->labelWP->width());
            ui->labelWP->setPixmap(img);
            ui->labelWP->setVisible(true);
            wallpaperChanged = true;
            setThemeChangeStatus();
        } else {
            wpPath = mTempPath;
        }
    }
}

void MainWindow::addViewColor()
{
    if (theme != nullptr && theme->size() > 0) {
        bool bOk;
        QString colorName = QInputDialog::getText(this,
                        tr("New view color"),
                        tr("View name:"),
                        QLineEdit::Normal,
                        "new_view_name",
                        &bOk);
        if (bOk) {
            ThemeItem *themeItem = new ThemeItem();
            themeItem->name = colorName;
            themeItem->setRawColor(-1);
            theme->append(themeItem);
            model->refreshData();
            setThemeChangeStatus();
        }
    }
}

void MainWindow::deleteViewColor()
{
    if (mCurrentThemeItem != nullptr) {
        QMessageBox* messageBox =
                    new QMessageBox(QMessageBox::Question,
                        tr("Delete view color"),
                        QString(tr("Do you want to remove view color '%1'?")).arg(mCurrentThemeItem->name),
                        QMessageBox::Yes | QMessageBox::No,
                        this);
        if (messageBox->exec() == QMessageBox::Yes) {
            theme->removeOne(mCurrentThemeItem);
            model->refreshData();
            mCurrentThemeItem = nullptr;
            setThemeChangeStatus();
        }
        delete messageBox;
    }
}

void MainWindow::clicked1(const QModelIndex &index)
{
    mCurrentThemeItem = model->getByIndex(index);
}

void MainWindow::exitApp()
{
    if (wallpaperChanged || isAnyModified()) {
        QMessageBox* messageBox =
                    new QMessageBox(QMessageBox::Question,
                        tr("Exit"),
                        QString(tr("Theme has been changed. Do you want to save changes?")),
                        QMessageBox::Yes | QMessageBox::No,
                        this);
        if (messageBox->exec() == QMessageBox::Yes) {
            saveTheme(openedThemePath);
        }
        delete messageBox;
    }
    close();
}
