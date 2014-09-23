#include <stdio.h>
#define MAX_DEVICES 10

#ifdef _WIN32
#include <videoinput.h>

typedef struct {
  unsigned char dev;
  videoInput VI;
  unsigned char *buffer;
} capture_t;


static int width;;
static int height;
static capture_t *devs[ MAX_DEVICES ];
static int devs_count = 0;

extern "C" int capture_init( char *device, int fps, int *w, int *h ) {
  int size, dev;
  printf( "RoboCortex [info]: WinCapInit\n" );

  capture_t *p_dev;
  printf( "RoboCortex [info]: 1\n");
  p_dev = new capture_t();
  dev = -1;

  printf( "RoboCortex [info]: 1\n");

  if( device ) {
  	if( strlen( device ) ) {
  		dev = atoi( device );
  	}
  }


  //Prints out a list of available devices and returns num of devices found
  if( dev >= 0 ) {
    if( !p_dev->VI.setupDevice( dev, *w, *h ) ) return( -1 );
  } else {
    int numDevices = p_dev->VI.listDevices();
    return( -1 );
  }

  p_dev->VI.setIdealFramerate( dev, fps );

  // Automatically reconnect on freeze, may fix bugs with some devices/drivers
  p_dev->VI.setAutoReconnectOnFreeze( dev, true, 25 );

  printf( "RoboCortex [info]: 2\n");

  width   = p_dev->VI.getWidth ( dev );
  height  = p_dev->VI.getHeight( dev );
  size    = p_dev->VI.getSize  ( dev );
  *w = width;
  *h = height;

  p_dev->buffer = new unsigned char[ size ];

  p_dev->dev = dev;
  devs[ devs_count ] = p_dev;

  return( devs_count++ );
}

extern "C" unsigned char * capture_fetch( int dev ) {
  if( devs[ dev ]->VI.isFrameNew( dev ) ) {
    devs[ dev ]->VI.getPixels( devs[ dev ]->dev, devs[ dev ]->buffer, false, true ); // BGR, flipped
  }
  return( devs[ dev ]->buffer );
}

extern "C" void capture_free() {
	unsigned int dev;
	for( dev = 0; dev < devs_count; dev++ ) {
  	devs[ dev ]->VI.stopDevice( devs[ dev ]->dev );
  	devs[ dev ] = NULL;
  }
  devs_count = 0;
}

#else
#include "opencv/cv.h"
#include "opencv/highgui.h"

static int width;;
static int height;
typedef struct {
  unsigned char *buffer;
  struct CvCapture *capture;
} capture_t;

static capture_t devs[ MAX_DEVICES ];
static int devs_count = 0;

extern "C" int capture_init( char *device, int fps, int *w, int *h ) {
    int size, dev;
    dev = atoi( device );
    // Initialize camera
    devs[ devs_count ].capture = cvCaptureFromCAM( dev );
    if( !devs[devs_count].capture ) return( -1 );

    cvSetCaptureProperty(devs[ devs_count ].capture, CV_CAP_PROP_FRAME_WIDTH, (double)*w);
    cvSetCaptureProperty(devs[ devs_count ].capture, CV_CAP_PROP_FRAME_HEIGHT, (double)*h);

    IplImage* img = 0;
    img = cvQueryFrame( devs[ devs_count ].capture );
    width = ( int )cvGetCaptureProperty( devs[ devs_count ].capture, CV_CAP_PROP_FRAME_WIDTH );
    height = ( int )cvGetCaptureProperty( devs[ devs_count ].capture, CV_CAP_PROP_FRAME_HEIGHT );
    size = img->imageSize;

    devs[ devs_count ].buffer = new unsigned char[ size ];

    *w = width;
    *h = height;
    return( devs_count++ );
}

extern "C" unsigned char * capture_fetch( int dev ) {
    IplImage* img = 0;
    if( !cvGrabFrame( devs[ dev ].capture ) ) {              // capture a frame
        printf( "Could not grab a frame\n" );
        return( NULL );
    }
    img = cvRetrieveFrame( devs[ dev ].capture );           // retrieve the captured frame
    memcpy( devs[ dev ].buffer, img->imageData, img->imageSize );
    return( devs[ dev ].buffer );
}

extern "C" void capture_free() {
	// TODO: ?
	devs_count = 0;
}

#endif
