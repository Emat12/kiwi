#ifndef VIDEOSTREAMSOURCE_H
#define VIDEOSTREAMSOURCE_H
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <QTimer>
#include <QThread>
#include <QString>
#include <QByteArray>
#include <QDir>
#include <QFile>
extern "C" {
#include <ccn/ccn.h>
#include <ccn/keystore.h>
#include <ccn/uri.h>
}

#include "VideoStreamEncoder.h"
#include "NdnHandler.h"

class SourceAnnouncer;

class CameraVideoInput{
public:
	CameraVideoInput();
	virtual ~CameraVideoInput();
	IplImage *getNextFrame();
	bool initCamera();
	void releaseCamera();
private:
	CvCapture *cap;
	bool initialized;
};

class VideoStreamSource: public QThread{
	Q_OBJECT
public:
	void run();	
	VideoStreamSource();
	~VideoStreamSource();
	void setNamePrefix(QString prefix) {namePrefix = prefix;}
	void toggleState();

signals:
	//void imageCaptured(QString, const unsigned char *, int);
	void imageCaptured(QString, IplImage *);


private slots:
	void processFrame();

private:
	void generateNdnContent(const unsigned char *buffer, int len);
	void readNdnParams();


private:
	CameraVideoInput *cam;
	VideoStreamEncoder *encoder;
	NdnHandler *nh;
	SourceAnnouncer *sa;
	QTimer *captureTimer;
	long seq;
	long frameNum;
	bool initialized;
	QString namePrefix;
	QString confName;
	QString username;
	bool bRunning;
	bool enabled;
};


class SourceAnnouncer: public QThread {
	Q_OBJECT
public:
	SourceAnnouncer(QString confName, QString prefix);
	~SourceAnnouncer();
	void run();
	void toggleLeaving();

public slots:
	void generateSourceInfo();
private:
	void registerInterest();
private:
	QString confName;
	QString username;
	QString prefix;
	struct ccn_closure *publishInfo;	
	NdnHandler *nh;
	QTimer *announceTimer;
	bool leaving;
	bool bRunning;
	bool enabled;
};


#endif
