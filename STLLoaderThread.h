#pragma once
#include <QThread>
#include <QString>
#include <vtkSmartPointer.h>
#include <vtkSTLReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>

class STLLoaderThread : public QThread {
	Q_OBJECT
private:
	QString filePath;

public:
	explicit STLLoaderThread(QObject* parent = nullptr);

	void setFilePath(const QString& path);

protected:
	void run() override;

signals:
	void loadingFinished(vtkSmartPointer<vtkActor> actor); // ��������ź�

	void errorOccurred(const QString& message);            // �����ź�
};

