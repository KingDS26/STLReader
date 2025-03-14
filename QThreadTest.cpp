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

    //��ʼ��
    vtkRenderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    ui.openGLWidget->SetRenderWindow(vtkRenderWindow);

    //�����ť����ʾstl�ļ�
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
    // ʹ�� vtkSTLReader ��ȡ STL �ļ�
    vtkSmartPointer<vtkSTLReader> reader = vtkSmartPointer<vtkSTLReader>::New();
    reader->SetFileName(filePath.toUtf8().constData());
    reader->Update();

    // ����ļ��Ƿ�ɹ�����
    if (reader->GetOutput() == nullptr || reader->GetOutput()->GetNumberOfPoints() == 0) {
        qWarning() << "Failed to load STL file or file is empty.";
        return;
    }

    // ���� Mapper
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(reader->GetOutputPort());

    // ���� Actor
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

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