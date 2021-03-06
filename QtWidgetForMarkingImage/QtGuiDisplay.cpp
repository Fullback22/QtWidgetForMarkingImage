#include "QtGuiDisplay.h"

void QtGuiDisplay::setCursor_āorChangesTopBotton(double rotAngel)
{
	if ((rotAngel >= 337.5 && rotAngel <= 360) || (rotAngel >= 0 && rotAngel < 22.5) || (rotAngel >= 157.5 && rotAngel < 202.5))
		myCursor.setShape(Qt::SizeVerCursor);
	else if ((rotAngel >= 67.5 && rotAngel < 112.5) || (rotAngel >= 247.5 && rotAngel < 292.5))
		myCursor.setShape(Qt::SizeHorCursor);
	else if ((rotAngel >= 22.5 && rotAngel < 67.5) || (rotAngel >= 202.5 && rotAngel < 247.5))
		myCursor.setShape(Qt::SizeBDiagCursor);
	else if ((rotAngel >= 112.5 && rotAngel < 157.5) || (rotAngel >= 292.5 && rotAngel < 337.5))
		myCursor.setShape(Qt::SizeFDiagCursor);
}

void QtGuiDisplay::setCursor_āorChangesRightLeft(double rotAngel)
{
	if ((rotAngel >= 337.5 && rotAngel <= 360) || (rotAngel >= 0 && rotAngel < 22.5) || (rotAngel >= 157.5 && rotAngel < 202.5))
		myCursor.setShape(Qt::SizeHorCursor);
	else if ((rotAngel >= 67.5 && rotAngel < 112.5) || (rotAngel >= 247.5 && rotAngel < 292.5))
		myCursor.setShape(Qt::SizeVerCursor);
	else if ((rotAngel >= 22.5 && rotAngel < 67.5) || (rotAngel >= 202.5 && rotAngel < 247.5))
		myCursor.setShape(Qt::SizeFDiagCursor);
	else if ((rotAngel >= 112.5 && rotAngel < 157.5) || (rotAngel >= 292.5 && rotAngel < 337.5))
		myCursor.setShape(Qt::SizeBDiagCursor);
}

void QtGuiDisplay::setCursor_āorChangesTopRight_BottonLeft(double rotAngel)
{
	if ((rotAngel >= 337.5 && rotAngel <= 360) || (rotAngel >= 0 && rotAngel < 22.5) || (rotAngel >= 157.5 && rotAngel < 202.5))
		myCursor.setShape(Qt::SizeFDiagCursor);
	else if ((rotAngel >= 67.5 && rotAngel < 112.5) || (rotAngel >= 247.5 && rotAngel < 292.5))
		myCursor.setShape(Qt::SizeBDiagCursor);
	else if ((rotAngel >= 22.5 && rotAngel < 67.5) || (rotAngel >= 202.5 && rotAngel < 247.5))
		myCursor.setShape(Qt::SizeVerCursor);
	else if ((rotAngel >= 112.5 && rotAngel < 157.5) || (rotAngel >= 292.5 && rotAngel < 337.5))
		myCursor.setShape(Qt::SizeHorCursor);
}

void QtGuiDisplay::setCursor_āorChangesTopLeft_BottonRight(double rotAngel)
{
	if ((rotAngel >= 337.5 && rotAngel <= 360) || (rotAngel >= 0 && rotAngel < 22.5) || (rotAngel >= 157.5 && rotAngel < 202.5))
		myCursor.setShape(Qt::SizeBDiagCursor);
	else if ((rotAngel >= 67.5 && rotAngel < 112.5) || (rotAngel >= 247.5 && rotAngel < 292.5))
		myCursor.setShape(Qt::SizeFDiagCursor);
	else if ((rotAngel >= 22.5 && rotAngel < 67.5) || (rotAngel >= 202.5 && rotAngel < 247.5))
		myCursor.setShape(Qt::SizeHorCursor);
	else if ((rotAngel >= 112.5 && rotAngel < 157.5) || (rotAngel >= 292.5 && rotAngel < 337.5))
		myCursor.setShape(Qt::SizeVerCursor);
}

QtGuiDisplay::QtGuiDisplay(QWidget *parent)
	: QWidget(parent),
	isZoomNow(false),
	activProcesArea(0)
{
	ui.setupUi(this);
	activProcessedObj = new ProcessedObject();;
	event_img = false;
	change_roi = false;
	activatedRoi = true;
	changeActivArea = false;
	activ_roi = -2;
	isProcessingActiv = false;

	ui.horSB_forTempImg->hide();
	ui.verSB_forTempImg->hide();
	this->updateGeometry();
	connect(ui.horSB_forTempImg, SIGNAL(valueChanged(int)), this, SLOT(slot_mouvePixmap()));
	connect(ui.verSB_forTempImg, SIGNAL(valueChanged(int)), this, SLOT(slot_mouvePixmap()));
	connect(ui.pushButt_ZoomIncress, SIGNAL(clicked()), this, SLOT(slot_ZoomImg_In()));
	connect(ui.pushButt_ZoomDeduce, SIGNAL(clicked()), this, SLOT(slot_ZoomImg_Out()));
	connect(ui.pushButt_AllLabl, SIGNAL(clicked()), this, SLOT(slot_ZoomImg_AllLabl()));
	

	connect(ui.label_for_TempImg, SIGNAL(mousePos()), this, SLOT(slot_mouseCurrentPos()));
	connect(ui.label_for_TempImg, SIGNAL(mousePressed()), this, SLOT(slot_mousePressed()));
	connect(ui.label_for_TempImg, SIGNAL(mouseRelease()), this, SLOT(slot_mouseRelease()));
	connect(ui.label_for_TempImg, SIGNAL(mouseLeft()), this, SLOT(slot_mouseLeft()));
	connect(ui.label_for_TempImg, SIGNAL(mouseLeftMouveRoi(int)), this, SLOT(slot_mouseLeft(int)));
}

QtGuiDisplay::~QtGuiDisplay()
{
	//delete activProcessedObj;
}

void QtGuiDisplay::setSizeScrollBar()
{
	if (ui.label_for_TempImg->getScaledImgSize()->width() > ui.label_for_TempImg->width())
	{
		ui.horSB_forTempImg->show();
		ui.horSB_forTempImg->setRange(0, std::abs(ui.label_for_TempImg->getScaledImgSize()->width() - ui.label_for_TempImg->width()));
	}
	else
	{
		ui.horSB_forTempImg->hide();
	}
	if (ui.label_for_TempImg->getScaledImgSize()->height() > ui.label_for_TempImg->height())
	{
		ui.verSB_forTempImg->show();
		ui.verSB_forTempImg->setRange(0, std::abs(ui.label_for_TempImg->getScaledImgSize()->height() - ui.label_for_TempImg->height()));
	}
	else
	{
		ui.verSB_forTempImg->hide();
	}
}

void QtGuiDisplay::slot_mouvePixmap()
{
	if (activ && ui.pushButt_AllLabl->isEnabled())
		ui.label_for_TempImg->show_partImg(ui.horSB_forTempImg->value(), ui.verSB_forTempImg->value(), ui.label_for_TempImg->width(), ui.label_for_TempImg->height());
}

void QtGuiDisplay::slot_mouseCurrentPos()
{
	if (activProcessedObj != nullptr)
	{
		if (activProcessedObj->getProcesArears()->size() > 0)
		{
			if (!change_roi)
			{
				activ_roi = -2;
				int min_roi{ -1 };
				for (int i{ 0 }; i < activProcessedObj->getProcesArears()->size(); i++)
				{
					if (activProcessedObj->getProcesArears()[0][i].isActiv() && activProcessedObj->getProcesArears()[0][i].isDraw())
					{
						int x, y;
						QPoint buferPoint(ui.label_for_TempImg->getImageCoordinate());
						x = buferPoint.x();
						y = buferPoint.y();
						if (activProcessedObj->getProcesArears()[0][i].getRect()->contains(x, y) )  //check whether the pointer islocated in activ ROI
						{
							min_roi = i;
							activ_roi = min_roi;
							myCursor.setShape(Qt::SizeAllCursor);
							activProcessedObj->getProcesArears()[0][i].getRect()->setResizeType(NoResize);
							this->setCursor(myCursor);
						}
						else if (!activProcessedObj->getProcesArears()[0][i].getRect()->contains(x, y))
						{
							int point_X{ 0 };
							int point_Y{ 0 };
							{
								point_X = abs((activProcessedObj->getProcesArears()[0][i].getRect()->getUpRigth_X() - activProcessedObj->getProcesArears()[0][i].getRect()->getUpLeft_X()) / 2);
								point_Y = abs((activProcessedObj->getProcesArears()[0][i].getRect()->getUpRigth_Y() - activProcessedObj->getProcesArears()[0][i].getRect()->getUpLeft_Y()) / 2);
								if (activProcessedObj->getProcesArears()[0][i].getRect()->getRotateAngel() >= 0 && activProcessedObj->getProcesArears()[0][i].getRect()->getRotateAngel() < 90)
								{
									point_X += activProcessedObj->getProcesArears()[0][i].getRect()->getUpLeft_X();
									point_Y += activProcessedObj->getProcesArears()[0][i].getRect()->getUpLeft_Y();
									point_X += sin(activProcessedObj->getProcesArears()[0][i].getRect()->getRotateAngel(true)) * 17;
									point_Y -= cos(activProcessedObj->getProcesArears()[0][i].getRect()->getRotateAngel(true)) * 17;
								}
								else
								if (activProcessedObj->getProcesArears()[0][i].getRect()->getRotateAngel() >= 90 && activProcessedObj->getProcesArears()[0][i].getRect()->getRotateAngel() < 180)
								{
									point_X = activProcessedObj->getProcesArears()[0][i].getRect()->getUpLeft_X() - point_X;
									point_Y += activProcessedObj->getProcesArears()[0][i].getRect()->getUpLeft_Y();
									point_X += sin(activProcessedObj->getProcesArears()[0][i].getRect()->getRotateAngel(true)) * 17;
									point_Y -= cos(activProcessedObj->getProcesArears()[0][i].getRect()->getRotateAngel(true)) * 17;
								}
								else
								if (activProcessedObj->getProcesArears()[0][i].getRect()->getRotateAngel() >= 180 && activProcessedObj->getProcesArears()[0][i].getRect()->getRotateAngel() < 270)
								{
									point_X = activProcessedObj->getProcesArears()[0][i].getRect()->getUpLeft_X() - point_X;
									point_Y = activProcessedObj->getProcesArears()[0][i].getRect()->getUpLeft_Y() - point_Y;
									point_X += sin(activProcessedObj->getProcesArears()[0][i].getRect()->getRotateAngel(true)) * 17;
									point_Y -= cos(activProcessedObj->getProcesArears()[0][i].getRect()->getRotateAngel(true)) * 17;
								}
								else
								if (activProcessedObj->getProcesArears()[0][i].getRect()->getRotateAngel() >= 270 && activProcessedObj->getProcesArears()[0][i].getRect()->getRotateAngel() < 360)
								{
									point_X += activProcessedObj->getProcesArears()[0][i].getRect()->getUpLeft_X();
									point_Y = activProcessedObj->getProcesArears()[0][i].getRect()->getUpLeft_Y() - point_Y;
									point_X += sin(activProcessedObj->getProcesArears()[0][i].getRect()->getRotateAngel(true)) * 17;
									point_Y -= cos(activProcessedObj->getProcesArears()[0][i].getRect()->getRotateAngel(true)) * 17;
								}
							}
						}
						
						{
							QtRotateRect verifiableRect{ *activProcessedObj->getProcesArears()[0][i].getRect() };
							double diag{ sqrt(2 * 7 * 7) };
							double rotAngel{ activProcessedObj->getProcesArears()[0][i].getRect()->getRotateAngel() };
							if (QtRotateRect(verifiableRect.getUpLeft_X() + 7 * sin(verifiableRect.getRotateAngel(true)), verifiableRect.getUpLeft_Y() - 7 * cos(verifiableRect.getRotateAngel(true)), verifiableRect.width(), 10, verifiableRect.getRotateAngel()).contains(x, y))
							{
								min_roi = i;
								activ_roi = min_roi;
								activProcessedObj->getProcesArears()[0][i].getRect()->setResizeType(Resize_Y);
								setCursor_āorChangesTopBotton(rotAngel);
							}
							else if (QtRotateRect(verifiableRect.getDownLeft_X(), verifiableRect.getDownLeft_Y()-3, verifiableRect.width(), 10, verifiableRect.getRotateAngel()).contains(x, y))
							{
								min_roi = i;
								activ_roi = min_roi;
								activProcessedObj->getProcesArears()[0][i].getRect()->setResizeType(Resize_Height);
								setCursor_āorChangesTopBotton(rotAngel);
							}
							else if (QtRotateRect(verifiableRect.getUpLeft_X() - 7 * sin((90 - verifiableRect.getRotateAngel()) * 3.14 / 180), verifiableRect.getUpLeft_Y() - 7 * cos((90 - verifiableRect.getRotateAngel()) * 3.14 / 180), 10, verifiableRect.height() - 1, verifiableRect.getRotateAngel()).contains(x, y))
							{
								min_roi = i;
								activ_roi = min_roi;
								activProcessedObj->getProcesArears()[0][i].getRect()->setResizeType(Resize_X);
								setCursor_āorChangesRightLeft(rotAngel);
							}
							else if (QtRotateRect(verifiableRect.getUpRigth_X()-3, verifiableRect.getUpRigth_Y(), 13, verifiableRect.height(), verifiableRect.getRotateAngel()).contains(x, y))
							{
								min_roi = i;
								activ_roi = min_roi;
								activProcessedObj->getProcesArears()[0][i].getRect()->setResizeType(Resize_Width);
								setCursor_āorChangesRightLeft(rotAngel);
							}
							else if (QtRotateRect(verifiableRect.getUpLeft_X() - diag * cos((45 + verifiableRect.getRotateAngel()) * 3.14 / 180), verifiableRect.getUpLeft_Y() - diag * sin((45 + verifiableRect.getRotateAngel()) * 3.14 / 180), 8, 8, verifiableRect.getRotateAngel()).contains(x, y))
							{
								min_roi = i;
								activ_roi = min_roi;
								activProcessedObj->getProcesArears()[0][i].getRect()->setResizeType(Resize_XAndY);
								setCursor_āorChangesTopRight_BottonLeft(rotAngel);
							}
							else if(QtRotateRect(verifiableRect.getDownRigth_X() - 1, verifiableRect.getDownRigth_Y() - 1, 8, 8, verifiableRect.getRotateAngel()).contains(x, y))
							{
								min_roi = i;
								activ_roi = min_roi;
								activProcessedObj->getProcesArears()[0][i].getRect()->setResizeType(Resize_HeigthAndWidth);
								setCursor_āorChangesTopRight_BottonLeft(rotAngel);
							}
							else if (QtRotateRect(verifiableRect.getUpRigth_X() + 6 * sin(verifiableRect.getRotateAngel(true)), verifiableRect.getUpRigth_Y() - 8 * cos(verifiableRect.getRotateAngel(true)), 8, 8, verifiableRect.getRotateAngel()).contains(x, y))
							{
								min_roi = i;
								activ_roi = min_roi;
								activProcessedObj->getProcesArears()[0][i].getRect()->setResizeType(Resize_YAndWidth);
								setCursor_āorChangesTopLeft_BottonRight(rotAngel);
							}
							else if (QtRotateRect(verifiableRect.getDownLeft_X() - 7 * cos(verifiableRect.getRotateAngel(true)), verifiableRect.getDownLeft_Y() - 7 * sin(verifiableRect.getRotateAngel(true)), 8, 8, verifiableRect.getRotateAngel()).contains(x, y))
							{
								min_roi = i;
								activ_roi = min_roi;
								activProcessedObj->getProcesArears()[0][i].getRect()->setResizeType(Resize_XAndHeight);
								setCursor_āorChangesTopLeft_BottonRight(rotAngel);
							}
							this->setCursor(myCursor);
						}
					}
				}
				if (activ_roi == -2)
				{
					myCursor.setShape(Qt::ArrowCursor);
					this->setCursor(myCursor);
				}
			}
			else
			{
				if (this->myCursor.shape() == Qt::SizeAllCursor)
				{
					
					activProcessedObj->getProcesArears()[0][activ_roi].getRect()->changePosition(ui.label_for_TempImg->getImageCoordinate(),
																									ui.label_for_TempImg->getFirstImagePoint(),
																								*ui.label_for_TempImg->getOriginalImgSize());
					ui.label_for_TempImg->setFirstPixmapPoint(ui.label_for_TempImg->getImageCoordinate());
					
				}
				else if (this->myCursor.shape() == Qt::SizeHorCursor || this->myCursor.shape() == Qt::SizeVerCursor ||
						 this->myCursor.shape() == Qt::SizeBDiagCursor || this->myCursor.shape() == Qt::SizeFDiagCursor)
				{
					
					activProcessedObj->getProcesArears()[0][activ_roi].getRect()->resizeRect(ui.label_for_TempImg->getImageCoordinate(),
																								ui.label_for_TempImg->getImageCoordinate(false),
																								ui.label_for_TempImg->getFirstImagePoint(),
																								ui.label_for_TempImg->getOriginalImgSize());
				}
				else if (this->myCursor.shape() == Qt::CrossCursor)
				{
					
						activProcessedObj->getProcesArears()[0][activ_roi].getRect()->changeAngel(ui.label_for_TempImg->getImageCoordinate(),
																								 *ui.label_for_TempImg->getOriginalImgSize());
				}
				
				draw_proceseArears();
				int x, y;
				ui.label_for_TempImg->getDrPoint(x, y);
				ui.label_for_TempImg->show_partImg(x, y, ui.label_for_TempImg->width(), ui.label_for_TempImg->height());
			}
		}
		
	}

	if (event_img)
	{
		int dr_x, dr_y;
		ui.label_for_TempImg->moveImg(dr_x, dr_y);
		ui.horSB_forTempImg->setSliderPosition(dr_x);
		ui.verSB_forTempImg->setSliderPosition(dr_y);
	}
}

void QtGuiDisplay::slot_mousePressed()
{
	if (activ_roi != -2)
	{
		event_img = false;
		if (this->myCursor.shape() == Qt::SizeAllCursor|| this->myCursor.shape() == Qt::SizeHorCursor || this->myCursor.shape() == Qt::SizeVerCursor ||
			this->myCursor.shape() == Qt::SizeBDiagCursor || this->myCursor.shape() == Qt::SizeFDiagCursor || this->myCursor.shape() == Qt::CrossCursor)
		{
			change_roi = true;
		}
	}
	
	
	event_img = true;
	int min_roi{ -1 };
	for (int i{ 0 }; i < activProcessedObj->getProcesArears()->size(); i++)
	{
		if (activProcessedObj->getProcesArears()[0][i].isDraw())
		{
			/*int x, y;
			ui.label_for_TempImg->getPointInImg(x, y);*/

			int x, y;
			QPoint buferPoint(ui.label_for_TempImg->getImageCoordinate());
			x = buferPoint.x();
			y = buferPoint.y();

			if (activProcessedObj->getProcesArears()[0][i].getRect()->contains(x, y))   //check whether the pointer islocated in activ ROI
			{//selecting a activ roi
				if (min_roi == -1)
				{
					min_roi = i;
				}
				else if (activProcessedObj->getProcesArears()[0][i].getArea(true) < (activProcessedObj->getProcesArears()[0][min_roi].getArea(true)))
				{
					min_roi = i;
				}
			}
		}
	}
	for (int i{ 0 }; i < activProcessedObj->getProcesArears()->size(); i++)
	{
		if (min_roi != -1)
		{
			if (i == min_roi)
			{
				emit changeActivProcesArea(i);
				activProcessedObj->getProcesArears()[0][i].setActiv(true);
					
				myCursor.setShape(Qt::SizeAllCursor);
				this->setCursor(myCursor);
				change_roi = true;
					
				activ_roi = i;
				activProcesArea = i;
				event_img = false;
				updateImg();
			}
			else
			{
				activProcessedObj->getProcesArears()[0][i].setActiv(false);
			}
		}
	}
	
}

void QtGuiDisplay::slot_mouseLeft()
{
	if ( change_roi)
	{
		int dr_x, dr_y;
		ui.label_for_TempImg->getDrPoint(dr_x, dr_y);
		QPoint imgPoint(*(ui.label_for_TempImg->getImgPoint()));

		if (imgPoint.x() >= ui.label_for_TempImg->width() - 1)
		{
			ui.horSB_forTempImg->setSliderPosition(dr_x + 1);
		}
		else if (imgPoint.x() <= 0)
		{
			ui.horSB_forTempImg->setSliderPosition(dr_x - 1);
		}

		if (imgPoint.y() >= ui.label_for_TempImg->height() - 1)
		{
			ui.verSB_forTempImg->setSliderPosition(dr_y + 1);
		}
		else if (imgPoint.y() <= 0)
		{
			ui.verSB_forTempImg->setSliderPosition(dr_y - 1);
		}
	}
	else
	{
		myCursor.setShape(Qt::ArrowCursor);
		this->setCursor(myCursor);
	}
}

void QtGuiDisplay::slot_mouseLeft(int direct)
{
	if (change_roi)
	{
		int dr_x, dr_y;
		ui.label_for_TempImg->getDrPoint(dr_x, dr_y);
		QPoint imgPoint(*(ui.label_for_TempImg->getImgPoint()));

		if (direct == 1)
		{
			ui.horSB_forTempImg->setSliderPosition(dr_x + 3);
		}
		else if (direct == 4)
		{
			ui.horSB_forTempImg->setSliderPosition(dr_x - 3);
		}

		if (direct == 3)
		{
			ui.verSB_forTempImg->setSliderPosition(dr_y + 3);
		}
		else if (direct == 2)
		{
			ui.verSB_forTempImg->setSliderPosition(dr_y - 3);
		}
	}
}

void QtGuiDisplay::slot_mouseRelease()
{
	event_img = false;
	change_roi = false;
}

void QtGuiDisplay::slot_ZoomImg_In()
{
	if (!isZoomNow)
	{
		isZoomNow = true;
		int dr_x, dr_y;
		activ_scaled = ui.label_for_TempImg->scaledPixmap(1, dr_x, dr_y);
		if (activ_scaled == 500)
		{
			ui.pushButt_ZoomIncress->setEnabled(false);
		}
		else
		{
			ui.pushButt_ZoomIncress->setEnabled(true);
		}
		ui.label_Scale->setText(QString::number(activ_scaled) + "%");
		ui.pushButt_ZoomDeduce->setEnabled(true);
		this->setSizeScrollBar();
		ui.label_for_TempImg->scaledPixmap();
		this->setSizeScrollBar();
		ui.horSB_forTempImg->setValue(dr_x);
		ui.verSB_forTempImg->setValue(dr_y);
		if (!ui.pushButt_AllLabl->isEnabled())
			ui.pushButt_AllLabl->setEnabled(true);

		draw_proceseArears();
		ui.label_for_TempImg->show_partImg(dr_x, dr_y, ui.label_for_TempImg->width(), ui.label_for_TempImg->height());
		isZoomNow = false;
	}
}

void QtGuiDisplay::slot_ZoomImg_Out()
{
	if (!isZoomNow)
	{
		isZoomNow = true;
		ui.horSB_forTempImg->hide();
		ui.verSB_forTempImg->hide();
		int dr_x, dr_y;
		activ_scaled = ui.label_for_TempImg->scaledPixmap(-1, dr_x, dr_y);
		if (activ_scaled == 25)
		{
			ui.pushButt_ZoomDeduce->setEnabled(false);
		}
		else
		{
			ui.pushButt_ZoomDeduce->setEnabled(true);
		}

		ui.label_Scale->setText(QString::number(activ_scaled, 'f', 1) + "%");
		ui.pushButt_ZoomIncress->setEnabled(true);
		this->setSizeScrollBar();
		ui.label_for_TempImg->scaledPixmap();
		this->setSizeScrollBar();
		ui.horSB_forTempImg->setValue(dr_x);
		ui.verSB_forTempImg->setValue(dr_y);
		if (!ui.pushButt_AllLabl->isEnabled())
			ui.pushButt_AllLabl->setEnabled(true);

		draw_proceseArears();
		ui.label_for_TempImg->show_partImg(dr_x, dr_y, ui.label_for_TempImg->width(), ui.label_for_TempImg->height());
		isZoomNow = false;
	}
}

void QtGuiDisplay::slot_ZoomImg_AllLabl()
{	
	if (!isZoomNow)
	{
		isZoomNow = true;
		ui.horSB_forTempImg->hide();
		ui.verSB_forTempImg->hide();
		this->updateGeometry();
		int max, min;
		activ_scaled = ui.label_for_TempImg->scaledPixmap(0, max, min);
		ui.label_Scale->setText(QString::number(activ_scaled, 'f', 1) + "%");
		ui.label_for_TempImg->getMaxMinScal(max, min);
		if (max < 10)
			ui.pushButt_ZoomIncress->setEnabled(true);
		else
			ui.pushButt_ZoomIncress->setEnabled(false);
		if (min > 0)
			ui.pushButt_ZoomDeduce->setEnabled(true);
		else
			ui.pushButt_ZoomDeduce->setEnabled(false);
		ui.pushButt_AllLabl->setEnabled(false);
	
		ui.label_for_TempImg->show_partImg(0, 0, ui.label_for_TempImg->width(), ui.label_for_TempImg->height());
		draw_proceseArears();
		ui.label_for_TempImg->show_partImg(0, 0, ui.label_for_TempImg->width(), ui.label_for_TempImg->height());
		isZoomNow = false;
	}
}

void QtGuiDisplay::setEanbleActivededRoi(bool activ)
{
	if (activ)
	{
		activatedRoi = true;
	}
	else
	{
		activatedRoi = false;
	}
}

void QtGuiDisplay::setActiv(bool activ)
{
	this->activ = activ;
	if (activ)
	{
		ui.label_for_TempImg->setStyleSheet("");
		//this->setActivProcessObj(*activProcessedObj);      
	}
	else
	{
		//setEnableWidtsGrouBox(false);
		ui.label_for_TempImg->setText(" ");
		ui.label_for_TempImg->setStyleSheet("background-color: black");
		ui.horSB_forTempImg->hide();
		ui.verSB_forTempImg->hide();
		ui.label_for_TempImg->setPixmap(QPixmap());
	}
}

void QtGuiDisplay::setChangeActivArea(bool isChange)
{
	changeActivArea = isChange;
}

void QtGuiDisplay::setChangesProcessedArears(bool isChang)
{
	//changesProcesedArearsGeometry = isChang;
}

void QtGuiDisplay::draw_proceseArears()
{
	if (activProcessedObj != nullptr)
	{
		bool noDraw{ true };
		double penWeigth{ 1.0 };
		if (activ_scaled < 100)
			penWeigth *= 100 / activ_scaled;
		double penSize{ 1 * penWeigth };
		if (penSize < 1)
			penSize = 1;
		emit getActivProcessArea();
		
		for (int i{ 0 }; i < activProcessedObj->getProcesArears()->size(); ++i)
		{
			penSize = 1;
			if (activProcessedObj->getProcesArears()[0][i].isDraw())
			{
				QPen penBufer(QBrush(activProcessedObj->getProcesArears()[0][i].getColor()), ceil(penSize * penWeigth), Qt::DashLine);
				ui.label_for_TempImg->draw_rect(activProcessedObj->getProcesArears()[0][i].getRect(), penBufer);
				noDraw = false;
			}
		}
		if (activProcessedObj->getProcesArears()->size() > 0)
		{
			if (activProcessedObj->getProcesArears()[0][activProcesArea].isDraw())
			{
				QPen penBufer(QBrush(activProcessedObj->getProcesArears()[0][activProcesArea].getColor()), ceil(2 * penWeigth), Qt::DashLine);
				ui.label_for_TempImg->draw_rect(activProcessedObj->getProcesArears()[0][activProcesArea].getRect(), penBufer);
			}
		}
		
		if (noDraw)
		{
			QRect QR(0, 0, 0, 0);
			ui.label_for_TempImg->draw_rect(&QtRotateRect(QR));
		}
	}
}

void QtGuiDisplay::add_rect(int classLabel, QColor const *color)
{
	int w{ 0 };
	int h{ 0 };
	if (ui.label_for_TempImg->getScaledImgSize()->width() < ui.label_for_TempImg->size().width())
	{
		w = ui.label_for_TempImg->getScaledImgSize()->width() / 4;
	}
	else
	{
		w = ui.label_for_TempImg->size().width() / 4;
	}
	if (ui.label_for_TempImg->getScaledImgSize()->height() < ui.label_for_TempImg->size().height())
	{
		h = ui.label_for_TempImg->getScaledImgSize()->height() / 4;
	}
	else
	{
		h = ui.label_for_TempImg->size().height() / 4;
	}
	int x{ ui.label_for_TempImg->size().width() / 2 };
	int y{ ui.label_for_TempImg->size().height() / 2 };
	ui.label_for_TempImg->toImgCoordinate_(x, y);
	activProcessedObj->getProcesArears()[0].push_back(QtProcessedArea(QtRotateRect(QRect(x - w / 2, y - h / 2, w, h)), classLabel, color));
	
	activProcessedObj->getProcesArears()[0][activProcesArea].setActiv(false);
	activProcesArea = activProcessedObj->getProcesArears()[0].size() - 1;
	activProcessedObj->getProcesArears()[0][activProcesArea].setActiv(true);
	updateImg();
}

void QtGuiDisplay::deletActivRectangel()
{
	if (activProcessedObj->getProcesArears()[0].size() > 0)
	{
		activProcessedObj->getProcesArears()[0].erase(activProcessedObj->getProcesArears()[0].begin() + activProcesArea);
		if (activProcesArea == activProcessedObj->getProcesArears()[0].size() && activProcesArea > 0)
			--activProcesArea;
		updateImg();
	}
}

QRect QtGuiDisplay::getLabelRect()
{
	QRect bufer;
	bufer.setX(this->x() + ui.label_for_TempImg->x());
	bufer.setY(this->y() + ui.label_for_TempImg->y());
	bufer.setWidth(ui.label_for_TempImg->rect().width());
	bufer.setHeight(ui.label_for_TempImg->rect().height());

	return bufer;
}

void QtGuiDisplay::updateImg()
{
	if (activ)
	{
		draw_proceseArears();
		ui.label_for_TempImg->show_partImg();
	}
}

bool QtGuiDisplay::isActiv()
{
	return activ;
}

bool QtGuiDisplay::getChageActivArea()
{
	return changeActivArea;
}

bool QtGuiDisplay::ProcessedIsActiv()
{
	return isProcessingActiv;
}

void QtGuiDisplay::updateProcessObj(ProcessedObject& activObj)
{
	activProcessedObj = &activObj; 
	draw_proceseArears();
	this->setSizeScrollBar();
	ui.label_for_TempImg->show_partImg();
	activ_scaled = ui.label_for_TempImg->get_activScaled();
	ui.label_Scale->setText(QString::number(activ_scaled, 'f', 1) + "%");
}

void QtGuiDisplay::setActivProcesArea(int activArea)
{
	activProcesArea = activArea;
}

void QtGuiDisplay::setNewImage(cv::Mat const inputImg)
{
	QImage drPic(inputImg.data, inputImg.cols, inputImg.rows, inputImg.step, QImage::Format::Format_BGR888);
	ui.label_for_TempImg->update_myPixmap(QPixmap::fromImage(drPic));
	slot_ZoomImg_AllLabl();
	this->setSizeScrollBar();
}

void QtGuiDisplay::setNewClassLabel(int const classLabel, QColor const *color)
{
	activProcessedObj->getProcesArears()[0][activProcesArea].setClassLabel(classLabel);
	activProcessedObj->getProcesArears()[0][activProcesArea].setColor(color);
	updateImg();
}

void QtGuiDisplay::getClsaaRectangelAndLabel(std::vector<QRect>& rectangel, std::vector<int>& label)
{
	for (auto& element : activProcessedObj->getProcesArears()[0])
	{
		rectangel.push_back(element.getOriginalLimitRect());
		label.push_back(element.getClassLabel());
	}
}



void QtGuiDisplay::resizeEvent(QResizeEvent* event)
{
	ui.horSB_forTempImg->hide();
	ui.verSB_forTempImg->hide();
	int dr_x{ui.label_for_TempImg->getOldSize().width() - ui.label_for_TempImg->size().width() };
	int dr_y{ ui.label_for_TempImg->getOldSize().height() - ui.label_for_TempImg->size().height() };
	ui.label_for_TempImg->show_partImg(dr_x, dr_y, ui.label_for_TempImg->size().width() - 1, ui.label_for_TempImg->size().height() - 1);
	ui.horSB_forTempImg->setSliderPosition(ui.horSB_forTempImg->value() + dr_x);
	ui.verSB_forTempImg->setSliderPosition(ui.verSB_forTempImg->value() + dr_y);
	ui.label_for_TempImg->getDrPoint(dr_x, dr_y);
	ui.label_for_TempImg->setAllImgScaled();
	if (!ui.pushButt_AllLabl->isEnabled())
		slot_ZoomImg_AllLabl();
	else
		this->setSizeScrollBar();
}