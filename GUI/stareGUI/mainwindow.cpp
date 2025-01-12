#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>  // Do otwierania okna wyboru pliku
#include <QFile>        // Do pracy z plikami
#include <QTextStream>  // Do czytania tekstu z pliku
#include <QMessageBox> // Do wyskakujących okien
#include <QInputDialog> //Do wprowadzania inputu użytkownika

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWidget *defaultWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(defaultWidget);

    QLabel *welcomeLabel = new QLabel("Witaj w aplikacji! Wgraj swoj plik EKG aby następnie dokonać jego analizy :).", defaultWidget);
    welcomeLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(welcomeLabel);

    defaultWidget->setLayout(layout);
    setCentralWidget(defaultWidget);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionWczytaj_plik_triggered()
{
    // Tworzymy przycisk, który pojawi się po kliknięciu "Wczytaj plik"
    QPushButton *button = new QPushButton("Wczytaj plik", this);

    // Łączymy kliknięcie przycisku z otwarciem okna dialogowego wyboru pliku
    connect(button, &QPushButton::clicked, this, &MainWindow::on_buttonWczytajPlik_clicked);

    // Tworzymy nowy widget (okno główne)
    QWidget *newWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(newWidget);

    // Dodajemy przycisk do layoutu
    layout->addWidget(button);
    newWidget->setLayout(layout);

    // Ustawiamy nowy widget jako centralny widget
    setCentralWidget(newWidget);
}
void MainWindow::on_buttonWczytajPlik_clicked()
{

    QString fileName = QFileDialog::getOpenFileName(this, "Wczytaj plik", "", "Text Files (*.txt);;All Files (*)");

    if (!fileName.isEmpty()) {
        // Jeśli użytkownik wybrał plik, otwórz go
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            file.close();
            QMessageBox::information(this, "Wczytano", "Plik poprawnie wczytano! :) Przejdz do kolejnych kroków");
        } else {
            QMessageBox::warning(this, "Błąd", "Nie udało się otworzyć pliku. Spróbuj ponownie.");
        }
    } else {
        QMessageBox::information(this, "Błąd", "Nie wybrano pliku. Spróbuj ponownie.");
    }
}
void MainWindow::on_actionWy_wietl_EKG_triggered()
{
    QWidget *newWidget = new QWidget();
    QLabel *newLabel = new QLabel("New Widget for 2", newWidget);
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(newLabel);
    newWidget->setLayout(layout);
    setCentralWidget(newWidget);
}

void MainWindow::on_actionFiltracja_triggered()
{
    {
        bool ok;
        QStringList items = {"Brak filtracji","Średnia krocząca", "Filtr Butterwortha", "Filtr Savitzky-Golay", "Filtr LMS"};
        QString selectedItem = QInputDialog::getItem(this, tr("Wybierz filtracje"),
                                                     tr("Dostępne metody filtacji sygnału:"),
                                                     items, 0, false, &ok);

        if (ok && !selectedItem.isEmpty()) {
            QMessageBox::information(this, tr("Wybrano opcję"), tr("Wybrałeś: %1").arg(selectedItem));
        } else {
            QMessageBox::information(this, tr("Informacja"), tr("Nie wybrano opcji."));
        }
    }
}

void MainWindow::on_actionAnaliza_triggered()
{
    QWidget *newWidget = new QWidget();
    QLabel *newLabel = new QLabel("New Widget for 4", newWidget);
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(newLabel);
    newWidget->setLayout(layout);
    setCentralWidget(newWidget);
}

void MainWindow::on_actionRaport_triggered()
{
    QWidget *newWidget = new QWidget();
    QLabel *newLabel = new QLabel("New Widget for 5", newWidget);
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(newLabel);
    newWidget->setLayout(layout);
    setCentralWidget(newWidget);
}
void MainWindow::on_actionWyjd_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionJak_korzysta_z_aplikacji_triggered()
{
    QMessageBox::information(this, "Instrukcja", "Okno bedzie zawierac instrukcje dla uzytkonika jak działają posczególne elementy interefjsu\n1.AAAA\n2.BBBB");
}

