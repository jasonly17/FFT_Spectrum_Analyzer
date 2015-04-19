#include <QApplication>
#include <QQmlApplicationEngine>
#include <QAudioFormat>
#include <QAudioInput>
#include <QFile>
#include <QDebug>

#include "circularbuffer.h"

void printDebugInfo(CircularBuffer &buffer)
{
	qDebug() << QString("Count: %1, Size: %2, Length %3, Notify Interval: %4, Empty: %5")
				.arg(buffer.count()).arg(buffer.size()).arg(buffer.length())
				.arg(buffer.notifyInterval()).arg(buffer.isEmpty());
}

void testCircularBuffer()
{
	CircularBuffer buffer(2);
	printDebugInfo(buffer);

	buffer.writePair('a', 'b');
	printDebugInfo(buffer);
	char lch, rch;
	buffer.readPair(lch, rch);
	qDebug() << lch << rch;
	printDebugInfo(buffer);

	buffer.writePair('a', 'b');
	printDebugInfo(buffer);
	buffer.writePair('c', 'd');
	printDebugInfo(buffer);
	buffer.writePair('e', 'f');
	printDebugInfo(buffer);
	buffer.readPair(lch, rch);
	qDebug() << lch << rch;
	printDebugInfo(buffer);
	char data[1024];
	buffer.readBlock(data);
	buffer.readPair(lch, rch);
	qDebug() << lch << rch;
	printDebugInfo(buffer);
	buffer.readPair(lch, rch);
	qDebug() << lch << rch;
	printDebugInfo(buffer);
}

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	testCircularBuffer();

	return 0;
}
