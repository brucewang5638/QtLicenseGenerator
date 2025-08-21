#include "mainwindow.h"
#include <QApplication>

/**
 * @brief C++ 程序的入口点。
 *
 * 这是所有 Qt GUI 应用程序的标准入口函数。
 * @param argc 命令行参数的数量。
 * @param argv 命令行参数的数组。
 * @return 返回应用程序的退出码。
 */
int main(int argc, char *argv[])
{
    // 1. 创建 QApplication 对象。
    //    每个 Qt GUI 程序都必须有且只有一个 QApplication 对象。
    //    它负责管理应用程序范围的资源、事件循环等。
    QApplication a(argc, argv);

    // 2. 创建主窗口对象。
    //    MainWindow 是我们自己定义的、继承自 QMainWindow 的类。
    MainWindow w;

    // 3. 显示主窗口。
    //    窗口在创建后默认是不可见的，必须调用 show() 或类似函数才能让它显示出来。
    w.show();

    // 4. 进入并执行应用程序的事件循环。
    //    a.exec() 会启动 Qt 的事件处理机制。程序会在这里一直等待，
    //    直到用户关闭主窗口或调用 exit()，然后返回一个退出码。
    //    所有用户的交互（如点击按钮、输入文本）都会在这个循环中被处理。
    return a.exec();
}
