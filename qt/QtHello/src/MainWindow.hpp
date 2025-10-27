#pragma once

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool IsRunning();
protected:
    void closeEvent(QCloseEvent* event) override;

public slots:
    void buttonClickedEffect(bool checked = false);

private:
    Ui::MainWindow *ui;
    bool m_IsRunning = true;
};
