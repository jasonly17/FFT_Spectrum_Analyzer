#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include <QObject>

/*
 * An implementation of a circular buffer for audio data that stores samples in
 * pairs of 2 bytes, the first byte for the left channel and the other byte for
 * the right channel.
*/
class CircularBuffer : public QObject
{
	Q_OBJECT

public:
	enum State { Idle, Reading, Writing };

	// Note: size is the number of samples, not bytes
	explicit CircularBuffer(int size, QObject *parent = 0);
	~CircularBuffer();

	// Returns the amount of data (samples) currently in the buffer
	int count() const;

	// Returns the size of the buffer
	// return: bytes
	qint64 size() const;

	// Returns the maximum number of pairs the buffer can contain simultaneously
	int length() const;

	// Returns the notify interval when a certain amount of data has been written
	// to the buffer
	// return: the number of samples that need to be written before a notification
	//         is emitted
	int notifyInterval() const;

	// Returns whether their is any data in the buffer
	bool isEmpty() const;

	// Clears the buffer and resets buffer to ground state
	void clear();

	// Fills the buffer with the provided ch and then returns the filled buffer
	void fill(char ch);

	// Writes the data pair to the buffer and shifts the head pointer accordingly
	// param lch: data for the left channel
	// param rch: data for the right channel
	bool writePair(const char lch, const char rch);

	// Reads the data pair from the buffer and shifts the tail pointer accordingly
	// param lch: where to store the retrieved left channel data
	// param rch: where to store the retrieved right channel data
	bool readPair(char &lch, char &rch);

	// Reads a block of samples equivalent to the size indicated by interval and then
	// returns it, or returns null if there are not enough samples yet
	// param data: must be equal or greater in size than interval
	// return: the array returned has a length of interval * 2; client must delete
	bool readBlock(char *data);

	// Sets the notify interval
	// param size: the number of samples before a notification is emitted
	void setNotifyInterval(int size);

	// Sets the maximum number of samples the internal buffer can hold simultaneously
	bool setSize(int size);

signals:
	// The buffer state has changed
	void stateChanged(CircularBuffer::State state);

	// A certain amount of samples has been written to the buffer
	void notify();

private:
	struct {
		char *buffer;
		int count;
		int length;
		char *head;
		char *tail;
		char *end;
	} cBuffer;
	CircularBuffer::State cState;
	int interval;
};

#endif // CIRCULARBUFFER_H
