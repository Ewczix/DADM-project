#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>  // Do otwierania okna wyboru pliku
#include <QFile>        // Do pracy z plikami
#include <QTextStream>  // Do czytania tekstu z pliku
#include <QMessageBox> // Do wyskakujących okien
#include <QInputDialog> //Do wprowadzania inputu użytkownika
#include <QPushButton>

#include <QPrinter> //generwoanie pdf
#include <QPainter> //umożliwi dodanie danych do pliku PDF

#include <QTabWidget> //taby dla okna analizy

#include <QIcon>


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

    this->setWindowIcon(QIcon(":/images/logo.png"));
    this->setWindowTitle("ECGuider");

    applyStyle();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::applyStyle() {
    QFile file("C:/Users/wikto/Desktop/GUIdadm/NOWE GUI/GUInew/style1.qss");
    if (!file.exists()) {
        QMessageBox::warning(this, "Błąd", "Plik .qss nie istnieje.");
    } else {
        if (file.open(QFile::ReadOnly)) {
            QString styleSheet = file.readAll();
            qApp->setStyleSheet(styleSheet);
        } else {
            QMessageBox::warning(this, "Błąd", "Nie udało się otworzyć pliku .qss.");
        }
    }
}
void MainWindow::on_actionWczytaj_plik_triggered()
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
    // Tworzymy nowy widget i układ
    QWidget *newWidget = new QWidget();
    QLabel *newLabel = new QLabel("New Widget for 2", newWidget);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(newLabel);
    newWidget->setLayout(layout);
    setCentralWidget(newWidget);
}

void MainWindow::on_actionFiltracja_triggered()
{
    QFont font; // umożliwia zmiany wyglądu fontów
    font.setPointSize(12);

    // Tworzymy nowy widget i siatkę układu
    QWidget *newWidget = new QWidget(this);
    newWidget->setContentsMargins(0, 0, 0, 0);

    QGridLayout *layout = new QGridLayout(newWidget);
    layout->setContentsMargins(20, 10, 20, 10);
    layout->setSpacing(10);

    // Tytuł strony
    QLabel *label = new QLabel("Wybierz metody filtracji:", newWidget);
    label->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    layout->addWidget(label, 0, 0, 1, 2);
    label->setFont(font);

    // Placeholder na wykres
    QLabel *placeholderLabel = new QLabel("Tutaj pojawi się wykres.", newWidget);
    placeholderLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(placeholderLabel, 1, 0, 1, 2);

    // Lista metod filtracji (rozwijana lista)
    QStringList items = {"Brak filtracji", "Średnia krocząca", "Filtr Butterwortha", "Filtr Savitzky-Golay", "Filtr LMS"};
    QComboBox *comboBox = new QComboBox(newWidget);
    comboBox->addItems(items);
    layout->addWidget(comboBox, 2, 0);

    // Przycisk "Zastosuj"
    QPushButton *applyButton = new QPushButton("Zastosuj", newWidget);
    applyButton->setFixedWidth(150);
    layout->addWidget(applyButton, 2, 1);



    // Obsługa wyboru w dropdownlist
    connect(comboBox, &QComboBox::currentTextChanged, [=](const QString &selectedItem) {

    });

    // Obsługa kliknięcia przycisku
    connect(applyButton, &QPushButton::clicked, [=]() {
        QString selectedItem = comboBox->currentText();

    });

    newWidget->setLayout(layout);
    setCentralWidget(newWidget);
}

void MainWindow::on_actionAnaliza_triggered()
{
    QFont font; // Umożliwi zmiany wyglądu fontów
    font.setPointSize(12);

    QTabWidget *tabWidget = new QTabWidget(this);

    // Tworzymy zawartość dla zakładki HRV1
    QWidget *tab1 = new QWidget();
    QVBoxLayout *layout1 = new QVBoxLayout(tab1);

    // Tworzymy nowy widget i siatkę układu
    QWidget *newWidget = new QWidget();
    QGridLayout *layout = new QGridLayout(newWidget);

    // Dodajemy placeholder na wykres (jako QLabel)
    QLabel *placeholderLabel = new QLabel("Tutaj pojawi się wykres.", newWidget);
    placeholderLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(placeholderLabel, 1, 0, 1, 2);
    layout->setSpacing(5); // Mniejsze odstępy między wierszami

    // Dodajemy przycisk "Zastosuj"
    QPushButton *applyButton = new QPushButton("Kliknij aby dokonać analizy HRV 1", newWidget);
    layout->addWidget(applyButton, 2, 0, 1, 2, Qt::AlignCenter);

    // Ustaw układ na newWidget
    newWidget->setLayout(layout);
    layout1->addWidget(newWidget);

    // Dodaj zakładkę do QTabWidget
    tabWidget->addTab(tab1, "HRV1");

    // Tworzymy zawartość dla zakładki HRV2
    QWidget *tab2 = new QWidget();
    QVBoxLayout *layout2 = new QVBoxLayout(tab2);

    // Tworzymy nowy widget i siatkę układu
    QWidget *newWidget2 = new QWidget();
    QGridLayout *layout2_2 = new QGridLayout(newWidget2);


    QLabel *placeholderLabel2 = new QLabel("Tutaj pojawi się wykres.", newWidget2);
    placeholderLabel2->setAlignment(Qt::AlignCenter);
    layout2_2->addWidget(placeholderLabel2, 1, 0, 1, 2);
    layout2_2->setSpacing(5); // Mniejsze odstępy między wierszami

    QPushButton *applyButton2 = new QPushButton("Kliknij aby dokonać analizy HRV 2", newWidget2);
    layout2_2->addWidget(applyButton2, 2, 0, 1, 2, Qt::AlignCenter);

    // Ustaw układ na newWidget2
    newWidget2->setLayout(layout2_2);
    layout2->addWidget(newWidget2);

    // Dodaj zakładkę do QTabWidget
    tabWidget->addTab(tab2, "HRV2");
    setCentralWidget(tabWidget);

    // Tworzymy zawartość dla zakładki HRV DFA
    QWidget *tab3 = new QWidget();
    QVBoxLayout *layout3 = new QVBoxLayout(tab3);

    // Tworzymy nowy widget i siatkę układu
    QWidget *newWidget3 = new QWidget();
    QGridLayout *layout3_3 = new QGridLayout(newWidget3);


    QLabel *placeholderLabel3 = new QLabel("Tutaj pojawi się wykres.", newWidget3);
    placeholderLabel3->setAlignment(Qt::AlignCenter);
    layout3_3->addWidget(placeholderLabel3, 1, 0, 1, 2);
    layout3_3->setSpacing(5); // Mniejsze odstępy między wierszami

    QPushButton *applyButton3 = new QPushButton("Kliknij aby dokonać analizy HRV DFA", newWidget3);
    layout3_3->addWidget(applyButton3, 2, 0, 1, 2, Qt::AlignCenter);


    newWidget3->setLayout(layout3_3);
    layout3->addWidget(newWidget3);

    // Dodaj zakładkę do QTabWidget
    tabWidget->addTab(tab3, "HRV DFA");
    setCentralWidget(tabWidget);

    // Tworzymy zawartość dla zakładki Heart Class
    QWidget *tab4 = new QWidget();
    QVBoxLayout *layout4 = new QVBoxLayout(tab4);

    // Tworzymy nowy widget i siatkę układu
    QWidget *newWidget4 = new QWidget();
    QGridLayout *layout4_4 = new QGridLayout(newWidget4);


    QLabel *placeholderLabel4 = new QLabel("Tutaj pojawi się wykres.", newWidget4);
    placeholderLabel4->setAlignment(Qt::AlignCenter);
    layout4_4->addWidget(placeholderLabel4, 1, 0, 1, 2);
    layout4_4->setSpacing(5); // Mniejsze odstępy między wierszami

    QPushButton *applyButton4 = new QPushButton("Kliknij aby dokonać analizy Heart Class", newWidget4);
    layout4_4->addWidget(applyButton4, 2, 0, 1, 2, Qt::AlignCenter);


    newWidget4->setLayout(layout4_4);
    layout4->addWidget(newWidget4);

    // Dodaj zakładkę do QTabWidget
    tabWidget->addTab(tab4, "Heart Class");
    setCentralWidget(tabWidget);

}

void MainWindow::on_actionRaport_triggered()
{
    // Wyświetlenie dialogu zapisu pliku
    QString fileName = QFileDialog::getSaveFileName(this, "Zapisz wygenerowany raport", "", "PDF (*.pdf)");

    // Sprawdzenie, czy użytkownik podał nazwę pliku
    if (fileName.isEmpty()) {
        QMessageBox::warning(this, "Brak nazwy pliku", "Nie podano nazwy pliku/wybrano anuluj.");
        return;
    }

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);

    // Otwarcie QPainter
    QPainter painter;
    if (!painter.begin(&printer)) {
        QMessageBox::critical(this, "Błąd", "Nie udało się zapisać pliku.");
        return;
    }

    // Generowanie zawartości PDF TO MOŻNA USUNĄĆ W TRAKCIE INTEGRACJI DODAŁAM TO JAKO TEST
    painter.setFont(QFont("Arial", 12));
    painter.drawText(100, 100, "To jest przykładowy raport");
    //DOTAD MOZNA USUNAC


    // Zakończenie rysowania
    painter.end();

    // Wyświetlenie informacji o sukcesie
    QMessageBox::information(this, "Sukces", "Twój raport został wygenerowany i zapisany jako plik PDF.");
}
void MainWindow::on_actionWyjd_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionJak_korzysta_z_aplikacji_triggered()
{
    QMessageBox::information(this, "Instrukcja", "Okno bedzie zawierac instrukcje dla uzytkonika jak działają posczególne elementy interefjsu\n1.AAAA\n2.BBBB");
}

