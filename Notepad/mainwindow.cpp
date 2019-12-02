#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDir>
#include <QtCore>
#include <QTreeView>
#include <sstream>
#include <QMessageBox>
#include <QTreeWidgetItem>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString sPath = "C:/UNI/Programming/Labs/C++/Qt/Notepad/RootPath";
    dirmodel = new QFileSystemModel(this);
    dirmodel->setRootPath(sPath);
    dirmodel->sort(3);
//    dirmodel->setFilter(QDir::Dirs);
    ui->treeView->setModel(dirmodel);
    ui->treeView->setRootIndex(dirmodel->setRootPath(sPath));
    ui->treeView->hideColumn(1);
    ui->treeView->hideColumn(2);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void clearDir( const QString path )
{
    QDir dir( path );

    dir.setFilter( QDir::NoDotAndDotDot | QDir::Files );
    foreach( QString dirItem, dir.entryList() )
        dir.remove( dirItem );

    dir.setFilter( QDir::NoDotAndDotDot | QDir::Dirs );
    foreach( QString dirItem, dir.entryList() )
    {
        QDir subDir( dir.absoluteFilePath( dirItem ) );
        subDir.removeRecursively();
    }
}

void MainWindow::on_actionNew_triggered()
{
    mFilename.clear();
    ui->textEdit->setText("");
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
    QFile file(fileName);
    mFilename = fileName;
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
        return;
    }
    setWindowTitle("Notepad - " + fileName);
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setText(text);
    file.close();
}

void MainWindow::on_actionSave_triggered() {
       QString fileName;
       if (mFilename.isEmpty()) {
           fileName = QFileDialog::getSaveFileName(this, "Save");
           mFilename = fileName;
       } else {
           fileName = mFilename;
       }
       QFile file(fileName);
       if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
           QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
           return;
       }
       setWindowTitle("Notepad - " + fileName);
       QTextStream out(&file);
       QString text = ui->textEdit->toPlainText();
       out << text;
       file.flush();
       file.close();
}

void MainWindow::on_actionSave_As_triggered()
{
    QString file = QFileDialog::getSaveFileName(this, "Save as");

    if(!file.isEmpty()) {
        mFilename = file;
        on_actionSave_triggered();
    }
}

void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}

void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
}

void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}

void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}

void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}

void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
    QString sPath = dirmodel->fileInfo(index).filePath();
    if(!sPath.isEmpty()) {
        setWindowTitle("Notepad - " + sPath);
        QFile sFile(sPath);
        if(sFile.open(QFile::ReadOnly | QFile::Text)){
            mFilename = sPath;
            QTextStream in(&sFile);
            QString text = in.readAll();
            sFile.close();
            ui->textEdit->setPlainText(text);
        }
    }
}

void MainWindow::on_pushButton_add_file_clicked()
{
    QModelIndex index = ui->treeView->currentIndex();
    QString Path = dirmodel->rootPath();

    if(dirmodel->isDir(index) && index.isValid()) {
        Path = dirmodel->filePath(index);
    }

    QString name  = QInputDialog::getText(this, "Name", "Enter a name");

    if(name.isEmpty()) return;

    QFile file(Path + "/" + name);
    file.open(QIODevice::ReadWrite);
    ui->treeView->update();
}

void MainWindow::on_pushButton_add_folder_clicked()
{
   // Make directory
    QModelIndex index = ui->treeView->currentIndex();

    if((dirmodel->isDir(index) && !(index.isValid())) || (!dirmodel->isDir(index) && index.isValid())) {
        index = dirmodel->index(dirmodel->rootPath());
    }

    QString name  = QInputDialog::getText(this, "Name", "Enter a name");

    if(name.isEmpty()) return;

    dirmodel->mkdir(index, name);
    ui->treeView->update();
}

void MainWindow::on_actionExit_triggered()
{
    QCoreApplication::quit();
}

void MainWindow::on_pushButton_delete_clicked()
{
    QModelIndex index = ui->treeView->currentIndex();
    QString Path = dirmodel->filePath(index);

    if(dirmodel->isDir(index) && index.isValid()) {
        QDir dir(Path);
        dir.removeRecursively();
    }
    else if (!dirmodel->isDir(index) && index.isValid()) {
        QFile file(Path);
        file.remove();
    }
}
