#include "STLLoaderThread.h"

STLLoaderThread::STLLoaderThread(QObject* parent)
    : QThread(parent) {}

void STLLoaderThread::setFilePath(const QString& path) {
    filePath = path;
}

void STLLoaderThread::run() {
    // 检查文件路径是否有效
    if (filePath.isEmpty()) {
        emit errorOccurred("File path is empty.");
        return;
    }

    // 使用 vtkSTLReader 加载 STL 文件
    vtkSmartPointer<vtkSTLReader> reader = vtkSmartPointer<vtkSTLReader>::New();
    reader->SetFileName(filePath.toUtf8().constData());
    reader->Update();

    // 检查文件是否成功加载
    if (reader->GetOutput() == nullptr || reader->GetOutput()->GetNumberOfPoints() == 0) {
        emit errorOccurred("Failed to load STL file or file is empty.");
        return;
    }

    // 创建 Mapper 和 Actor
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(reader->GetOutputPort());

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    // 发送结果到主线程
    emit loadingFinished(actor);
}