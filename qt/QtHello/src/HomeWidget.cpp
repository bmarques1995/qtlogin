#include "HomeWidget.hpp"
#include "ui_HomeWidget.hpp"
#include <QDesktopServices>
#include <QUrl>

QtSample::HomeWidget::HomeWidget(QWidget* parent) :
	QWidget(parent),
	m_UI(new Ui::HomeWidget)
{
	m_UI->setupUi(this);
	connect(m_UI->pushButton, &QPushButton::clicked,
		this, &HomeWidget::fetchPrivateData);
}

QtSample::HomeWidget::~HomeWidget()
{
	delete m_UI;
}

void QtSample::HomeWidget::fetchPrivateData(bool checked)
{
	qDebug() << "fetchPrivateData";
}
