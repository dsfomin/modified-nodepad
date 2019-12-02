#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QFileSystemModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionRedo_triggered();

    void on_actionSave_As_triggered();

    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionCopy_triggered();

    void on_actionCut_triggered();

    void on_actionPaste_triggered();

    void on_actionUndo_triggered();

    void on_treeView_clicked(const QModelIndex &index);

    void on_pushButton_add_file_clicked();

    void on_pushButton_add_folder_clicked();

    void on_actionExit_triggered();

    void on_pushButton_delete_clicked();
private:
    Ui::MainWindow *ui;
    QString mFilename;
    QFileSystemModel *dirmodel;
};
#endif // MAINWINDOW_H
