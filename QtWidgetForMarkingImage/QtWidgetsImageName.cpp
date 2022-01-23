#include "QtWidgetsImageName.h"

QtWidgetsImageName::QtWidgetsImageName(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

QtWidgetsImageName::~QtWidgetsImageName()
{
}

void QtWidgetsImageName::setId(int const newId)
{
	id = newId;
}

void QtWidgetsImageName::setImageName(QString const newName)
{
	ui.label_nameImage->setText(newName);
}

void QtWidgetsImageName::mousePressEvent(QMouseEvent* evnt)
{
	emit mousePres(id);
}

QString QtWidgetsImageName::getImageName() const
{
	return ui.label_nameImage->text();
}
