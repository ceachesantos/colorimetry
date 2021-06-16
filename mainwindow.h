#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    float frecuencia[2048];
    float valor[2048];
    float valornormalizado[2048];
    float SpecWave[2048];
    float SpecValue[2048];
    int numerolineas;
    int RValor;
    int GValor;
    int BValor;
    float luminancia;

    float frecuenciaXYZ[89];
    float X[89];
    float Y[89];
    float Z[89];


    ~MainWindow();

private slots:
   

    void UpdatePseudo();
    void UpdateReal();

    void on_B_On_clicked();   

    void on_S_R_valueChanged(int value);

    void on_S_G_valueChanged(int value);

    void on_S_B_valueChanged(int value);

    void on_S_Luminancia_valueChanged(int value);

private:
    QString Path;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
