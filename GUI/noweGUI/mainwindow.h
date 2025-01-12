#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
     void on_buttonWczytajPlik_clicked();

private slots:
    void on_actionWczytaj_plik_triggered();
    void on_actionWy_wietl_EKG_triggered();
    void on_actionFiltracja_triggered();
    void on_actionAnaliza_triggered();
    void on_actionRaport_triggered();
    void on_actionWyjd_triggered();




    void on_actionJak_korzysta_z_aplikacji_triggered();

private:
    Ui::MainWindow *ui;
    void applyStyle();
};

#endif // MAINWINDOW_H
