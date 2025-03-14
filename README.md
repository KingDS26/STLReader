# STLReader
## 一个简易的.stl文件浏览器

### 第一版：

UI界面里只有两个控件：

1. **openGL Widget** -> 提升为**QVTKOpenGLNativeWidget**
2.  **QPushbutton**

在QThreadTest.h中声明

1. **QString filePath;**
2. **vtkSmartPointer<vtkGenericOpenGLRenderWindow> vtkRenderWindow;** 
3. **void loadAndDisplaySTL(const QString&);**

点击按钮，会弹出文件资源浏览器，选择合适的.stl文件，在openGLWidget控件中进行可视化。