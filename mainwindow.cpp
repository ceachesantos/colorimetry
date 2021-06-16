#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QPainter>
#include <string>
#include <QTextStream>
#include <cmath>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    BValor=450;
    GValor=550;
    RValor=650;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_B_On_clicked()
{
    ui->fallo->setPlainText("hola");
    //Path="..";

    QString filename=QFileDialog::getOpenFileName(this,tr("Abrir"),Path,tr(""));

  //  ui->fallo->setPlainText("leer 1");
    if(filename.isEmpty())return;
  //  ui->fallo->setPlainText("leer 2");
    QFile in(filename);
  //  ui->fallo->setPlainText("leer 3");
    QFileInfo finfo(filename);
  //  ui->fallo->setPlainText("leer 4");
    Path=finfo.path();

   // ui->fallo->setPlainText("fallo 0");


    in.open(QIODevice::ReadOnly|QIODevice::Text);
    QTextStream sin(&in);

    numerolineas=0;
    /*while(!in.atEnd()){
        numerolineas++;
        // QString line=in.readLine();
        // QStringList lista=line.split(" ");

    } //Final while*/

    numerolineas=2048;

  //  ui->fallo->setPlainText("fallo 1");

    for (int i=0; i<numerolineas; i++){
        sin>>SpecWave[i];
        sin>>SpecValue[i];        
    }

  //  ui->fallo->setPlainText("fallo 2");

  /*  valornormalizado=SpecValue;
    frecuencia=SpecWave;
    valor=SpecValue; */

    float valorMax= -1;

    for(int i=0; i<numerolineas;i++){

        if(valorMax<SpecValue[i]){

            valorMax=SpecValue[i];
        }

    }

   // ui->fallo->setPlainText("fallo 3");

    for(int i=0; i<numerolineas;i++){

       // if(valorMax<SpecValue[i]){

            valornormalizado[i]=SpecValue[i]/valorMax;
       // }

    }
   // ui->fallo->setPlainText("fallo 4");


    // Leer conversor XYZ
    QString filenameXYZ("C:\\Users\\alumno\\Documents\\SI_Clase1\\Texto\\ciexyzjv.txt");
    if(filenameXYZ.isEmpty())return;
  //  ui->fallo->setPlainText("leer 2");
    QFile inXYZ(filenameXYZ);
  //  ui->fallo->setPlainText("leer 3");
    QFileInfo finfoXYZ(filenameXYZ);
  //  ui->fallo->setPlainText("leer 4");
    Path=finfoXYZ.path();

   // ui->fallo->setPlainText("fallo 0");


    inXYZ.open(QIODevice::ReadOnly|QIODevice::Text);
    QTextStream sinXYZ(&inXYZ);


  //  ui->fallo->setPlainText("fallo 1");

    for (int i=0; i<89; i++){
        sinXYZ>>frecuenciaXYZ[i];
        sinXYZ>>X[i];
        sinXYZ>>Y[i];
        sinXYZ>>Z[i];
    }
    ui->fallo->setPlainText(QString::number(X[0]));



    UpdateReal();


    //

}

void MainWindow::UpdateReal(){


    float XYZ2RGB[9]={3.240479, -1.537150, -0.498535, -0.969256, 1.875992, 0.041556, 0.055648, -0.204043, 1.057311};
    float RGB2XYZ[9]={0.4149, 0.3724, 0.2987, 0.2139, 0.7228, 0.1331, 0.0194, 0.1199, 0.9558};

    // Color real:
    int j = 0;
    float accumX = 0;
    float accumY = 0;
    float accumZ = 0;
    for(int i=0; i<89;i++){
        while(SpecWave[j++]<frecuenciaXYZ[i]){
            if (SpecWave[j]>380 && SpecWave[j]<825){
                accumX = accumX+X[i]*SpecValue[j];
                accumY = accumY+Y[i]*SpecValue[j];
                accumZ = accumZ+Z[i]*SpecValue[j];
            }
        }

    }

    // NORMALIZAR XYZ
    float Xnormalizado = accumX*luminancia/accumY;
    float Ynormalizado = accumY*luminancia/accumY;
    float Znormalizado = accumZ*luminancia/accumY;

    // Invertir todo
    float RValorNormalizado = XYZ2RGB[0]*Xnormalizado+XYZ2RGB[3]*Xnormalizado+XYZ2RGB[6]*Xnormalizado;
    float GValorNormalizado = XYZ2RGB[1]*Ynormalizado+XYZ2RGB[4]*Ynormalizado+XYZ2RGB[7]*Ynormalizado;
    float BValorNormalizado = XYZ2RGB[2]*Znormalizado+XYZ2RGB[5]*Znormalizado+XYZ2RGB[8]*Znormalizado;

    // Controlar clipping
    int Rfinal = RValorNormalizado*255>255?255:RValorNormalizado*255<0?0:(int)(RValorNormalizado*255);
    int Gfinal = GValorNormalizado*255>255?255:GValorNormalizado*255<0?0:(int)(GValorNormalizado*255);
    int Bfinal = BValorNormalizado*255>255?255:BValorNormalizado*255<0?0:(int)(BValorNormalizado*255);

    ui->L_Color_real->setStyleSheet("background-color:rgb(" + QString::number(Rfinal) + ","
                                     + QString::number(Gfinal) + ","
                                     + QString::number(Bfinal) + ")");
}


void MainWindow::UpdatePseudo(){


    /*float RGB2XYZ[9] = {3.240479, −1.537150, −0.498535,
    −0.969256, 1.875992, 0.041556,
    0.055648, −0.204043, 1.057311};*/
    float XYZ2RGB[9]={3.240479, -1.537150, -0.498535, -0.969256, 1.875992, 0.041556, 0.055648, -0.204043, 1.057311};
    float RGB2XYZ[9]={0.4149, 0.3724, 0.2987, 0.2139, 0.7228, 0.1331, 0.0194, 0.1199, 0.9558};


    float intensidadR=0,intensidadG=0,intensidadB=0;
    int i=0;
    int j=0;

    while(SpecWave[i++]<BValor);
    intensidadB=SpecValue[i];
    while(SpecWave[i++]<GValor);
    intensidadG=SpecValue[i];
    while(SpecWave[i++]<RValor);
    intensidadR=SpecValue[i];

    float XValor =  RGB2XYZ[0]*intensidadR+RGB2XYZ[3]*intensidadR+RGB2XYZ[6]*intensidadR;
    float YValor =  RGB2XYZ[1]*intensidadG+RGB2XYZ[4]*intensidadG+RGB2XYZ[7]*intensidadG;
    float ZValor =  RGB2XYZ[2]*intensidadB+RGB2XYZ[5]*intensidadB+RGB2XYZ[8]*intensidadB;

    // NORMALIZAR XYZ
    float Xnormalizado = XValor*luminancia/YValor;
    float Ynormalizado = YValor*luminancia/YValor;
    float Znormalizado = ZValor*luminancia/YValor;

    // Invertir todo
    float RValorNormalizado = XYZ2RGB[0]*Xnormalizado+XYZ2RGB[3]*Xnormalizado+XYZ2RGB[6]*Xnormalizado;
    float GValorNormalizado = XYZ2RGB[1]*Ynormalizado+XYZ2RGB[4]*Ynormalizado+XYZ2RGB[7]*Ynormalizado;
    float BValorNormalizado = XYZ2RGB[2]*Znormalizado+XYZ2RGB[5]*Znormalizado+XYZ2RGB[8]*Znormalizado;
/*
    float freq=0;
    float minR=10000,minG=10000,minB=10000;
    float diferenciaG=0,diferenciaR=0,diferenciaB=0;


    for(int i=0; i<numerolineas; i++){

       // freq=frecuencia[i];
        freq=SpecWave[i];

        if(freq>400 && freq<500){

            diferenciaR = abs(SpecWave[i]-RValor);
          //  ui->fallo->setPlainText(QString::number(diferenciaR));

            if(diferenciaR<=minR){
                minR = diferenciaR;
                intensidadR=valornormalizado[i];
              //  ui->fallo->setPlainText(QString::number(intensidadR)+"Mic mic"+QString::number(valornormalizado[i]));


            }
        }


        if(freq>=500 && freq<600){

            diferenciaG=abs(SpecWave[i]-GValor);

            if(diferenciaG<=minG){
                minG = diferenciaG;
                intensidadG=valornormalizado[i];
            }
        }


        if(freq>=600 && freq<750){

            diferenciaB=abs(SpecWave[i]-BValor);

            if(diferenciaB<=minB){
                minB = diferenciaB;
                intensidadB=valornormalizado[i];
            }

        }



    } */

    // Controlar clipping
    int Rfinal = RValorNormalizado*255>255?255:RValorNormalizado*255<0?0:(int)(RValorNormalizado*255);
    int Gfinal = GValorNormalizado*255>255?255:GValorNormalizado*255<0?0:(int)(GValorNormalizado*255);
    int Bfinal = BValorNormalizado*255>255?255:BValorNormalizado*255<0?0:(int)(BValorNormalizado*255);

    ui->fallo->setPlainText("background-color:rgb(" + QString::number(Rfinal) + ","
                                     + QString::number(Gfinal) + ","
                                     + QString::number(Bfinal) + ")");

    ui->L_Color_falso->setStyleSheet("background-color:rgb(" + QString::number(Rfinal) + ","
                                     + QString::number(Gfinal) + ","
                                     + QString::number(Bfinal) + ")");

}



void MainWindow::on_S_R_valueChanged(int value)
{
    RValor=value;
    UpdatePseudo();
    ui->L_R->setText(QString::number(RValor));
}

void MainWindow::on_S_G_valueChanged(int value)
{
    GValor=value;
    UpdatePseudo();
     ui->L_G->setText(QString::number(GValor));
}

void MainWindow::on_S_B_valueChanged(int value)
{
    BValor=value;
    UpdatePseudo();
     ui->L_B->setText(QString::number(BValor));
}

//valornormalizado una variable global, un array de floats




void MainWindow::on_S_Luminancia_valueChanged(int value)
{
     luminancia=value/100.00;
     UpdatePseudo();
     UpdateReal();
     //Leer matriz de transformacion para pasar las moviditas

}
