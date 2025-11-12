#include "MainWindow.hpp"
#include <QDesktopServices>
#include <QVBoxLayout>
#include <QUrl>

QtSample::MainWindow::MainWindow(bool loggedIn, QWidget *parent)
    : QMainWindow(parent)
{
    m_Stack = new QStackedWidget(this);
    
    resize(800, 600);

    m_LoginWidget = new LoginWidget();
    m_Stack->addWidget(m_LoginWidget);

	m_HomeWidget = new HomeWidget();
	m_Stack->addWidget(m_HomeWidget);

    setCentralWidget(m_Stack);
    m_Stack->setCurrentIndex(loggedIn ? 1 : 0);
}

QtSample::MainWindow::~MainWindow()
{
    //delete m_UI;
    delete m_HomeWidget;
    delete m_LoginWidget;
	delete m_Stack;
}

bool QtSample::MainWindow::IsRunning()
{
    return m_IsRunning;
}

void QtSample::MainWindow::closeEvent(QCloseEvent* event)
{
    m_IsRunning = false;
    QWidget::closeEvent(event);
}

void QtSample::MainWindow::LoginAction()
{
    qDebug () << "LoginAction: " << true;
    /*if (loggedIn)
    {
        m_Stack->setCurrentIndex(1);
    }*/
}
