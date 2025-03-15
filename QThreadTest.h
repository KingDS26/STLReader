#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QThreadTest.h"

#include <QString>
#include <string>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkSmartPointer.h>
#include "STLLoaderThread.h"  //子线程类


class QThreadTest : public QMainWindow
{
    Q_OBJECT

public:
    QThreadTest(QWidget *parent = nullptr);
    ~QThreadTest();

private:
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> vtkRenderWindow; // VTK 渲染窗口

    STLLoaderThread* loaderThread;  //子线程对象
 
private slots:
    void onLoadingFinished(vtkSmartPointer<vtkActor> actor);

    void onErrorOccurred(const QString& message);

private:
    Ui::QThreadTestClass ui;
};
