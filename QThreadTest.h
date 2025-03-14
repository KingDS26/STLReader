#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QThreadTest.h"

#include <QString>
#include <string>
#include <vtkGenericOpenGLRenderWindow.h> // Ìæ´ú vtkRenderWindow

class QThreadTest : public QMainWindow
{
    Q_OBJECT

public:
    QThreadTest(QWidget *parent = nullptr);
    ~QThreadTest();

private:
    QString filePath;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> vtkRenderWindow; // VTK äÖÈ¾´°¿Ú
    void loadAndDisplaySTL(const QString&);
    

private:
    Ui::QThreadTestClass ui;
};
