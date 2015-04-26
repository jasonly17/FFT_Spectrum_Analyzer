#ifndef SPECTRUMDISPLAY_H
#define SPECTRUMDISPLAY_H

#include <QObject>

class SpectrumDisplay : public QObject
{
	Q_OBJECT

public:
	explicit SpectrumDisplay(QObject *parent = 0);
	~SpectrumDisplay();

signals:

public slots:

private:
	QList<QQuickItem*> bands;
};

#endif // SPECTRUMDISPLAY_H
