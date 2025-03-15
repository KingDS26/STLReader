#if _MSC_VER >= 1600 //VS2015>VS>VS2010, MSVC VER= 10.0 -14.0
#pragma execution_character_set("utf-8")
#endif

#include "QThreadTest.h"
#include <QtWidgets/QApplication>
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <QMetaType>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 注册 vtkSmartPointer<vtkActor> 类型
    qRegisterMetaType<vtkSmartPointer<vtkActor>>("vtkSmartPointer<vtkActor>");

    QThreadTest w;
    w.setWindowTitle("STL文件查看器");
    w.show();
    return a.exec();
}
