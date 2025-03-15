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

//构造函数初始化
QThreadTest::QThreadTest(QWidget *parent)
    : QMainWindow(parent), loaderThread(new STLLoaderThread(this))
{
    ui.setupUi(this);

    //初始化 QVTKOpenGLNativeWidget 并设置 vtkGenericOpenGLRenderWindow
    vtkRenderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    ui.openGLWidget->SetRenderWindow(vtkRenderWindow);

    //连接子线程信号到槽函数
    connect(loaderThread, &STLLoaderThread::loadingFinished, this, &QThreadTest::onLoadingFinished);
    connect(loaderThread, &STLLoaderThread::errorOccurred, this, &QThreadTest::onErrorOccurred);

    // 点击按钮，选择 STL 文件并启动子线程
    connect(ui.pushButton, &QPushButton::clicked, this, [this]() {
        QString filePath = QFileDialog::getOpenFileName(this, "Select STL File", "", "STL Files (*.stl)");

        if (filePath.isEmpty()) {
            qWarning() << "No file path selected.";
            return;
        }

        // 设置子线程的文件路径并启动
        loaderThread->setFilePath(filePath);
        loaderThread->start();
        });
    
}

QThreadTest::~QThreadTest() {
    if (loaderThread && loaderThread->isRunning()) {
        loaderThread->quit(); // 请求退出线程
        loaderThread->wait(); // 等待线程退出
    }
}

void QThreadTest::onLoadingFinished(vtkSmartPointer<vtkActor> actor) {
    // 创建 Renderer
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(actor);
    renderer->SetBackground(0.1, 0.2, 0.3); // 设置背景颜色

    // 将 Renderer 添加到 RenderWindow
    vtkRenderWindow->AddRenderer(renderer);

    // 调整相机视角
    renderer->ResetCamera();
    renderer->GetActiveCamera()->Azimuth(30);  // 调整视角角度
    renderer->GetActiveCamera()->Elevation(30);
    renderer->ResetCameraClippingRange();

    // 刷新显示
    vtkRenderWindow->Render();
}

void QThreadTest::onErrorOccurred(const QString& message) {
    qWarning() << "Error:" << message;
}