#pragma once

#include <QMainWindow>
#include <QStackedWidget>
#include "LoginWidget.hpp"
#include "HomeWidget.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace QtSample
{
    class MainWindow : public QMainWindow
    {
    public:
        MainWindow(QWidget* parent = nullptr);
        ~MainWindow();

        bool IsRunning();

        void LoginAction();
        void LogoutAction();
    protected:
        void closeEvent(QCloseEvent* event) override;

    private:
        bool m_IsRunning = true;
        QStackedWidget* m_Stack;
        LoginWidget* m_LoginWidget;
        HomeWidget* m_HomeWidget;
    };
}
