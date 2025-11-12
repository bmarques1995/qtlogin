#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
    class LoginWidget;
}
QT_END_NAMESPACE

namespace QtSample
{
    class LoginWidget : public QWidget
    {
    public:
        LoginWidget(QWidget* parent = nullptr);
        ~LoginWidget();

    public slots:
        void googleButtonClickedEffect(bool checked = false);
        void githubButtonClickedEffect(bool checked = false);

    private:
        Ui::LoginWidget* m_UI;
    };
}