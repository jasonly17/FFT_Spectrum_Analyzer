#include "circularbuffer.h"

#include <QDebug>

/******************************************************************************
 * Constructor/Deconstructor
******************************************************************************/
CircularBuffer::CircularBuffer(int size, QObject *parent)
	: QObject(parent)
{
	cBuffer.buffer = new char[size * 2];
	memset(cBuffer.buffer, 0, size * 2);
	cBuffer.count = 0;
	cBuffer.length = size;
	cBuffer.head = cBuffer.buffer;
	cBuffer.tail = cBuffer.buffer;
	cBuffer.end = cBuffer.buffer + size * 2 - 1;
	cState = State::Idle;
	interval = 8;
}

CircularBuffer::~CircularBuffer()
{
	if (cBuffer.buffer != NULL) {
		delete cBuffer.buffer;
	}
}

/******************************************************************************
 * Public Functions
******************************************************************************/
int CircularBuffer::count() const
{
	return cBuffer.count;
}

qint64 CircularBuffer::size() const {
	return (qint64) cBuffer.length * 2;
}

int CircularBuffer::length() const {
	return cBuffer.length;
}

int CircularBuffer::notifyInterval() const {
	return interval;
}

bool CircularBuffer::isEmpty() const {
	return cBuffer.count == 0;
}

void CircularBuffer::fill(char ch) {
	memset(cBuffer.buffer, ch, cBuffer.length * 2);
}

void CircularBuffer::clear() {
	fill(0);
	cBuffer.count = 0;
	cBuffer.head = cBuffer.buffer;
	cBuffer.tail = cBuffer.buffer;
}

bool CircularBuffer::writePair(const char lch, const char rch)
{
	if (cBuffer.count == cBuffer.length) {
		qDebug() << QString("The buffer is full! Attempting to write %1 and %2.")
					.arg(lch).arg(rch);
		return false;
	}

	if (cState != State::Idle) {
		qDebug() << QString("The buffer is currently being used!");
		return false;
	}
	cState = State::Writing;
	emit stateChanged(cState);

	*cBuffer.head = lch;
	*(cBuffer.head + 1) = rch;
	if (cBuffer.head + 1 >= cBuffer.end) {
		cBuffer.head = cBuffer.buffer;
	} else {
		cBuffer.head += 2;
	}
	cBuffer.count++;
	if (cBuffer.count == interval) {
		emit notify();
	}
	cState = State::Idle;
	emit stateChanged(cState);
	return true;
}

bool CircularBuffer::readPair(char &lch, char &rch)
{
	if (cBuffer.count == 0) {
		qDebug() << QString("The buffer is empty!");
		return false;
	}

	if (cState != State::Idle) {
		qDebug() << QString("The buffer is currently being used!");
		return false;
	}
	cState = State::Reading;
	emit stateChanged(cState);

	lch = *cBuffer.tail;
	rch = *(cBuffer.tail + 1);
	if (cBuffer.tail + 2 > cBuffer.end) {
		cBuffer.tail = cBuffer.buffer;
	} else {
		cBuffer.tail += 2;
	}
	cBuffer.count--;
	cState = State::Idle;
	emit stateChanged(cState);
	return true;
}

bool CircularBuffer::readBlock(char *data)
{
	if (cBuffer.count == 0) {
		qDebug() << QString("The buffer is empty!");
		return false;
	}

	if (cState != State::Idle) {
		qDebug() << QString("The buffer is currently being used!");
		return false;
	}

	if (cBuffer.count < interval) {
		qDebug() << QString("The block is not ready to be read!");
		return false;
	}
	cState = State::Reading;
	emit stateChanged(cState);

	int copySize = 0;
	if (cBuffer.tail + interval * 2 > cBuffer.end) {
		copySize = cBuffer.end - cBuffer.tail;
		memcpy(data, cBuffer.tail, copySize);
		memcpy(data + copySize, cBuffer.buffer, interval * 2 - copySize);
	} else {
		memcpy(data, cBuffer.tail, interval * 2);
	}

	if (cBuffer.tail + interval * 2 > cBuffer.end) {
		cBuffer.tail = cBuffer.buffer + copySize;
	} else {
		cBuffer.tail += interval * 2;
	}
	cBuffer.count -= interval;
	cState = State::Idle;
	emit stateChanged(cState);
	return true;
}

void CircularBuffer::setNotifyInterval(int size)
{
	interval = size;
}

bool CircularBuffer::setSize(int size)
{
	if (cState == State::Idle) {
		delete cBuffer.buffer;
		cBuffer.buffer = new char[size * 2];
		cBuffer.count = 0;
		cBuffer.length = size;
		cBuffer.head = cBuffer.buffer;
		cBuffer.tail = cBuffer.buffer;
		cBuffer.end = cBuffer.buffer + size * 2 - 1;
		return true;
	}
	return false;
}
