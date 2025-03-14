#include "QThreadTest.h"
#include <QFileDialog>
#include <QDir>
#include <QDebug>

#include <QVTKOpenGLNativeWidget.h>
#include <vtkSmartPointer.h>
#include <vtkSTLReader.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);

QThreadTest::QThreadTest(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    //初始化
    vtkRenderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    ui.openGLWidget->SetRenderWindow(vtkRenderWindow);

    //点击按钮，显示stl文件
    connect(ui.pushButton, &QPushButton::clicked, this, [this]() {
        filePath = QFileDialog::getOpenFileName(this, "slect STL file", "", "STL files(*.stl)");

        if (filePath.isEmpty()) {
            qWarning() << "No file path selected. Please select a file first.";
            return;
        }
        loadAndDisplaySTL(filePath);
        });
}

QThreadTest::~QThreadTest()
{}

void QThreadTest::loadAndDisplaySTL(const QString& filePath) {
    // 使用 vtkSTLReader 读取 STL 文件
    vtkSmartPointer<vtkSTLReader> reader = vtkSmartPointer<vtkSTLReader>::New();
    reader->SetFileName(filePath.toUtf8().constData());
    reader->Update();

    // 检查文件是否成功加载
    if (reader->GetOutput() == nullptr || reader->GetOutput()->GetNumberOfPoints() == 0) {
        qWarning() << "Failed to load STL file or file is empty.";
        return;
    }

    // 创建 Mapper
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(reader->GetOutputPort());

    // 创建 Actor
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

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