#pragma once

#include <QWidget>
#include "ui_QtWidgetsImageName.h"

class QtWidgetsImageName : public QWidget
{
	Q_OBJECT

	int id{};
public:
	QtWidgetsImageName(QWidget *parent = Q_NULLPTR);
	~QtWidgetsImageName();
	void setId(int const newId);
	void setImageName(QString const newName);
	void mousePressEvent(QMouseEvent* evnt);
	QString getImageName() const;
private:
	Ui::QtWidgetsImageName ui;
	
signals:
	void mousePres(int idWig);
};
