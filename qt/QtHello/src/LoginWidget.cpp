#include "LoginWidget.hpp"
#include "ui_LoginWidget.hpp"
#include <QDesktopServices>
#include <QUrl>

QtSample::LoginWidget::LoginWidget(QWidget* parent)
    : QWidget(parent)
    , m_UI(new Ui::LoginWidget)
{
    m_UI->setupUi(this);
    connect(m_UI->googleButton, &QPushButton::clicked,
            this, &LoginWidget::googleButtonClickedEffect);
    connect(m_UI->githubButton, &QPushButton::clicked,
        this, &LoginWidget::githubButtonClickedEffect);
}

QtSample::LoginWidget::~LoginWidget()
{
    delete m_UI;
}

void QtSample::LoginWidget::googleButtonClickedEffect(bool checked)
{
    QDesktopServices::openUrl(QUrl("http://localhost:4000/login/google/qtapp"));
}

void QtSample::LoginWidget::githubButtonClickedEffect(bool checked)
{
    QDesktopServices::openUrl(QUrl("http://localhost:4000/login/github/qtapp"));
}
