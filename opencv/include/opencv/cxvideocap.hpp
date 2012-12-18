/**
*** Copyright (C) 1985-2010 Intel Corporation.  All rights reserved.
***
*** The information and source code contained herein is the exclusive
*** property of Intel Corporation and may not be disclosed, examined
*** or reproduced in whole or in part without explicit written authorization
*** from the company.
***
*** Embedded Application Lab, Intel Labs China.
**/

#ifndef _CX_VIDEOCAP_HPP_
#define _CX_VIDEOCAP_HPP_

#include "highgui.h"

class CxVideoReader 
{
public:
    // constructor
	CxVideoReader( void ) 
	{
		capture = NULL;
	}

    CxVideoReader( const char *filename ) 
    {
        if( filename==NULL || filename[0]>='0' && filename[0]<='9' && filename[1]=='\0' )
            capture = cvCreateCameraCapture( 0 );
        else
            capture = cvCreateFileCapture( filename );
        if( ! capture )
        {
            //OPENCV_ERROR( CV_StsBadArg, 
            //             "CxVideoReader::CxVideoReader()", 
            //              "No video source." );
            fprintf(stderr, "! Error: no video source \n");
			exit(-1);
			return;
        }
    }

	bool load( const char *filename ) 
	{
		if( filename==NULL || filename[0]>='0' && filename[0]<='9' && filename[1]=='\0' )
			capture = cvCreateCameraCapture( 0 );
		else
			capture = cvCreateFileCapture( filename );
		if( !capture )
		{
			//OPENCV_ERROR( CV_StsBadArg, 
			//	"CxVideoReader::CxVideoReader()", 
			//	"No video source." );
			//fprintf(stderr, "! Error: no video source \n");
			//exit(-1);
			return false;
		}

		return true;
	}

	void release() { cvReleaseCapture( &capture ); }

    // destructor
    virtual ~CxVideoReader() { cvReleaseCapture( &capture ); }

	bool valid() { return (capture!=NULL); }

    // query
    IplImage* query() { return cvQueryFrame( capture ); }
    bool eof() { return (count() > 0) && (index() >= count()); }

    // properties
    int width() const
    { return (int)cvGetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH ); }
    
    int width( int value ) 
    { return cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH, value ); }

    int height() const
    { return (int)cvGetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT ); }
    
    int height( int value ) 
    { return cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT, value ); }

    int count() const
    { return (int)cvGetCaptureProperty( capture, CV_CAP_PROP_FRAME_COUNT ); }
    
    int index() const
    { return (int)cvGetCaptureProperty( capture, CV_CAP_PROP_POS_FRAMES ); }
    
    int index( int value )
    { return cvSetCaptureProperty( capture, CV_CAP_PROP_POS_FRAMES, value ); }

    double fps() const  
    { return cvGetCaptureProperty( capture, CV_CAP_PROP_FPS ); }

    double fps( double value ) 
    { return cvSetCaptureProperty( capture, CV_CAP_PROP_FPS, value ); }

    int msec() const 
    { return (int)cvGetCaptureProperty( capture, CV_CAP_PROP_POS_MSEC ); }

    int msec( int value )
    { return cvSetCaptureProperty( capture, CV_CAP_PROP_POS_MSEC, value ); }
 
    void config( int _width, int _height, double _fps )
    {
        width( _width );
        height( _height );
        fps( _fps );
    }
 
    // operator
    operator const CvCapture* () const { return capture; }

protected:
    CvCapture* capture;

};

class CxVideoWriter 
{
public:
    // constructor
    CxVideoWriter( const char* filename, double fps=30, int fourcc=-1 ) 
    {
        char* q = this->filename;
        while( (*q++ = *filename++) );

        this->fps = (fps<=0) ? 30 : fps;
        this->fourcc = fourcc;
        writer = NULL;
    }

    // destructor
    virtual ~CxVideoWriter() 
    {
        if( writer )
            cvReleaseVideoWriter( &writer );
    }

    // write frame
    void write( const IplImage* image ) 
    {
        OPENCV_ASSERT( image->width % 4 == 0, 
                       "CxVideoWriter::write()", 
                       "The width must be multiple of 4." );
        if( writer==NULL ) 
        {
            writer = cvCreateVideoWriter( filename, 
                                          fourcc, fps, 
                                          cvGetSize(image), 1 );
            OPENCV_ASSERT( writer, 
                    "CxVideoWriter::write)", 
                    "Failed to create video.");
        }
        cvWriteFrame( writer, image );
    }

    // operator
    operator const CvVideoWriter* () const { return writer; }
    operator CvVideoWriter* () { return writer; }

protected:
    char    filename[1024];
    double  fps;
    int     fourcc;
    CvVideoWriter* writer;

};

#endif // _CX_VIDEOCAP_HPP_
