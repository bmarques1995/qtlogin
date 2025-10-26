#include "MainWindow.hpp"
#include "MainWindow.qtui.h"
#include <QDesktopServices>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->googleButton, &QPushButton::clicked,
            this, &MainWindow::buttonClickedEffect);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::IsRunning()
{
    return m_IsRunning;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    m_IsRunning = false;
    QWidget::closeEvent(event); // Call base implementation
}

void MainWindow::buttonClickedEffect(bool checked)
{
    qDebug() << "Close button clicked";
    QDesktopServices::openUrl(QUrl("https://brunosmarques.com.br/posts/building-qt-from-scratch"));
}

