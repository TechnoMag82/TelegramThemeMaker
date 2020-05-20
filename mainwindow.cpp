#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableViewColors->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->labelWP->setVisible(false);
    theme = new QList<ThemeItem*>();
    findedPositions = new QList<int>();
    initMainMenu();
    initTableViewColors();
}

MainWindow::~MainWindow()
{
    theme->clear();
    findedPositions->clear();
    delete findedPositions;
    delete theme;
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
}

void MainWindow::openTheme()
{
    QString themePath = QFileDialog::getOpenFileName(this, tr("Open Theme"), workingDirectory, "*.attheme");
    if (!themePath.isNull() && !themePath.isEmpty()) {
        ThemeLoader *loader = new ThemeLoader();
        loader->loadTheme(themePath, *theme);
        hasWallpaper = loader->isWallpaperExist();
        if (hasWallpaper) {
            wpPath = "wallpaper.jpg";
            QPixmap img("wallpaper.jpg");
            img.scaledToWidth(ui->labelWP->width());
            ui->labelWP->setPixmap(img);
            ui->labelWP->setVisible(true);
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
        setWindowTitle(QString("%1 - [%2]").arg(QCoreApplication::applicationName(), file.fileName()));
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

    connect(ui->lineEditSearch, SIGNAL(textEdited(QString)),
            this, SLOT(searchName(QString)));

    connect(ui->pushButtonPrev, SIGNAL(clicked()),
            this, SLOT(prevSearch()));

    connect(ui->pushButtonNext, SIGNAL(clicked()),
            this, SLOT(nextSearch()));
}

int MainWindow::qColorToRaw(QColor color)
{
    return ((color.alpha() & 0xff) << 24)
            + ((color.red() & 0xff) << 16)
            + ((color.green() & 0xff) << 8)
            + (color.blue() & 0xff);
}

void MainWindow::saveTheme(QString filePath)
{
    ThemeLoader *loader = new ThemeLoader();
    if (hasWallpaper) {
        loader->saveTheme(filePath, *theme, wpPath);
    } else {
        loader->saveTheme(filePath, *theme);
    }
    delete loader;
}

void MainWindow::gotoSearchPosition()
{
    QModelIndex index = ui->tableViewColors->model()->index(findedPositions->at(currentSearchPosition), 0);
    ui->tableViewColors->setCurrentIndex(index);
    ui->tableViewColors->scrollTo(index);
    ui->labelFinded->setText(QString("%1 of %2").arg(QString::number(currentSearchPosition + 1), QString::number(findedCount + 1)));
}

void MainWindow::saveTheme()
{
    if (theme != nullptr && theme->size() > 0) {
        saveTheme(openedThemePath);
    }
}

void MainWindow::doubleClicked1(const QModelIndex &index)
{
    if (index.column() == 2) {
        mCurrentThemeItem = model->getByIndex(index);
        QColor color = QColorDialog::getColor(mCurrentThemeItem->getColor(), this,
                                              tr("Color for ") + mCurrentThemeItem->name,
                                              QColorDialog::ShowAlphaChannel);
        if (color.isValid()) {
            mCurrentThemeItem->setRawColor(qColorToRaw(color));
            model->refreshData();
        }
    }
}

void MainWindow::searchName(const QString text)
{
    int themeSize = theme->size();
    if (theme != nullptr && themeSize > 0 && text.size() > 3) {
        findedPositions->clear();
        for (int i = 0; i < themeSize - 1; i++) {
            if (theme->at(i)->name.contains(text, Qt::CaseInsensitive)) {
                findedPositions->append(i);
            }
        }
        if (findedPositions->size() > 0) {
            currentSearchPosition = 0;
            findedCount = findedPositions->size() - 1;
            gotoSearchPosition();
        } else {
            findedCount = 0;
            ui->labelFinded->setText(QString("0 of 0"));
        }
    } else {
        findedCount = 0;
        ui->labelFinded->setText(QString("0 of 0"));
    }
}

void MainWindow::prevSearch()
{
    if (findedCount > 0 && currentSearchPosition > 0) {
        currentSearchPosition--;
        gotoSearchPosition();
    }
}

void MainWindow::nextSearch()
{
    if (findedCount > 0 && currentSearchPosition < findedCount) {
        currentSearchPosition++;
        gotoSearchPosition();
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
    ThemeLoader *loader = new ThemeLoader();
    loader->deleteWallpaper();
    delete loader;
    hasWallpaper = false;
    ui->labelWP->setText(tr("No Wallpaper"));
    ui->labelWP->setVisible(false);
}

void MainWindow::selectWallpaper()
{
    wpPath = QFileDialog::getOpenFileName(this, tr("Open Wallpaper"), workingDirectory, "*.jpg");
    if (!wpPath.isNull() && !wpPath.isEmpty()) {
        QPixmap img(wpPath);
        hasWallpaper = true;
        img.scaledToWidth(ui->labelWP->width());
        ui->labelWP->setPixmap(img);
        ui->labelWP->setVisible(true);
    }
}
