#include "STLLoaderThread.h"

STLLoaderThread::STLLoaderThread(QObject* parent)
    : QThread(parent) {}

void STLLoaderThread::setFilePath(const QString& path) {
    filePath = path;
}

void STLLoaderThread::run() {
    // ����ļ�·���Ƿ���Ч
    if (filePath.isEmpty()) {
        emit errorOccurred("File path is empty.");
        return;
    }

    // ʹ�� vtkSTLReader ���� STL �ļ�
    vtkSmartPointer<vtkSTLReader> reader = vtkSmartPointer<vtkSTLReader>::New();
    reader->SetFileName(filePath.toUtf8().constData());
    reader->Update();

    // ����ļ��Ƿ�ɹ�����
    if (reader->GetOutput() == nullptr || reader->GetOutput()->GetNumberOfPoints() == 0) {
        emit errorOccurred("Failed to load STL file or file is empty.");
        return;
    }

    // ���� Mapper �� Actor
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(reader->GetOutputPort());

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    // ���ͽ�������߳�
    emit loadingFinished(actor);
}