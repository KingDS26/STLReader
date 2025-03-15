#include "QThreadTest.h"
#include <QFileDialog>
#include <QDebug>

#include <QVTKOpenGLNativeWidget.h>
#include <vtkRenderer.h>
#include <vtkCamera.h>
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);

//���캯����ʼ��
QThreadTest::QThreadTest(QWidget *parent)
    : QMainWindow(parent), loaderThread(new STLLoaderThread(this))
{
    ui.setupUi(this);

    //��ʼ�� QVTKOpenGLNativeWidget ������ vtkGenericOpenGLRenderWindow
    vtkRenderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    ui.openGLWidget->SetRenderWindow(vtkRenderWindow);

    //�������߳��źŵ��ۺ���
    connect(loaderThread, &STLLoaderThread::loadingFinished, this, &QThreadTest::onLoadingFinished);
    connect(loaderThread, &STLLoaderThread::errorOccurred, this, &QThreadTest::onErrorOccurred);

    // �����ť��ѡ�� STL �ļ����������߳�
    connect(ui.pushButton, &QPushButton::clicked, this, [this]() {
        QString filePath = QFileDialog::getOpenFileName(this, "Select STL File", "", "STL Files (*.stl)");

        if (filePath.isEmpty()) {
            qWarning() << "No file path selected.";
            return;
        }

        // �������̵߳��ļ�·��������
        loaderThread->setFilePath(filePath);
        loaderThread->start();
        });
    
}

QThreadTest::~QThreadTest() {
    if (loaderThread && loaderThread->isRunning()) {
        loaderThread->quit(); // �����˳��߳�
        loaderThread->wait(); // �ȴ��߳��˳�
    }
}

void QThreadTest::onLoadingFinished(vtkSmartPointer<vtkActor> actor) {
    // ���� Renderer
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(actor);
    renderer->SetBackground(0.1, 0.2, 0.3); // ���ñ�����ɫ

    // �� Renderer ��ӵ� RenderWindow
    vtkRenderWindow->AddRenderer(renderer);

    // ��������ӽ�
    renderer->ResetCamera();
    renderer->GetActiveCamera()->Azimuth(30);  // �����ӽǽǶ�
    renderer->GetActiveCamera()->Elevation(30);
    renderer->ResetCameraClippingRange();

    // ˢ����ʾ
    vtkRenderWindow->Render();
}

void QThreadTest::onErrorOccurred(const QString& message) {
    qWarning() << "Error:" << message;
}