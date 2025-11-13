#pragma once

#include <QWidget>
#include "Fetcher.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
    class HomeWidget;
}
QT_END_NAMESPACE

namespace QtSample
{
    class HomeWidget : public QWidget
    {
    public:
        HomeWidget(QWidget* parent = nullptr);
        ~HomeWidget();

    public slots:
        void fetchPrivateData(bool checked = false);

    private:
        Ui::HomeWidget* m_UI;
        Fetcher* m_Fetcher;
    };
}