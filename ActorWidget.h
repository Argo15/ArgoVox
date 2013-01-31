#ifndef ACTORWIDGET_H
#define ACTORWIDGET_H

#include <QtGui>

class ActorWidget : public QWidget
{
	Q_OBJECT

private:
	ActorWidget();
	static ActorWidget* m_pInstance;

	QTableWidget *tableWidget;
	QTableWidgetItem *modelHdr, *materialHdr, *transHdr, *rotHdr, *scaleHdr;

	QComboBox *matCmb,*modelCmb;
	QLineEdit *posXEdt, *posYEdt, *posZEdt,
				*rotXEdt, *rotYEdt, *rotZEdt,
				*scaleXEdt, *scaleYEdt, *scaleZEdt;

public:
    static ActorWidget *getInstance();

public slots:
	void refresh();

	void currentMatChanged(QString material);
	void currentModelChanged(QString model);
	void changePosition();
	void changeScale();
};

#endif