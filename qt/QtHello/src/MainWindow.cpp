#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include <QDesktopServices>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->googleButton, &QPushButton::clicked,
            this, &MainWindow::googleButtonClickedEffect);
    connect(ui->githubButton, &QPushButton::clicked,
        this, &MainWindow::githubButtonClickedEffect);
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

void MainWindow::googleButtonClickedEffect(bool checked)
{
    QDesktopServices::openUrl(QUrl("http://localhost:4000/login/google/qtapp"));
}

void MainWindow::githubButtonClickedEffect(bool checked)
{
    QDesktopServices::openUrl(QUrl("http://localhost:4000/login/github/qtapp"));
}
