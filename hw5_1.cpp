///////////////////////////////////////////////////////////////////////////////
// File generated by HDevelop for HALCON/C++ Version 20.11.1.2
// Non-ASCII strings in this file are encoded in local-8-bit encoding (cp936).
// Ensure that the interface encoding is set to locale encoding by calling
// SetHcppInterfaceStringEncodingIsUtf8(false) at the beginning of the program.
// 
// Please note that non-ASCII characters in string constants are exported
// as octal codes in order to guarantee that the strings are correctly
// created on all systems, independent on any compiler settings.
// 
// Source files with different encoding should not be mixed in one project.
///////////////////////////////////////////////////////////////////////////////



#ifndef __APPLE__
#  include "HalconCpp.h"
#  include "HDevThread.h"
#else
#  ifndef HC_LARGE_IMAGES
#    include <HALCONCpp/HalconCpp.h>
#    include <HALCONCpp/HDevThread.h>
#    include <HALCON/HpThread.h>
#  else
#    include <HALCONCppxl/HalconCpp.h>
#    include <HALCONCppxl/HDevThread.h>
#    include <HALCONxl/HpThread.h>
#  endif
#  include <stdio.h>
#  include <CoreFoundation/CFRunLoop.h>
#endif



using namespace HalconCpp;


#ifndef NO_EXPORT_MAIN
// Main procedure 
void action()
{

  // Local iconic variables
  HObject  ho_Image, ho_ModelRegion, ho_TemplateImage;
  HObject  ho_ModelContours, ho_TransContours;

  // Local control variables
  HTuple  hv_ModelID, hv_ModelRegionArea, hv_RefRow;
  HTuple  hv_RefColumn, hv_HomMat2D, hv_TestImages, hv_T;
  HTuple  hv_Row, hv_Column, hv_Angle, hv_Score, hv_I;

  //
  //Matching 02: ************************************************
  //Matching 02: BEGIN of generated code for model initialization
  //Matching 02: ************************************************
  SetSystem("border_shape_models", "false");
  //
  //Matching 02: Obtain the model image
  ReadImage(&ho_Image, "D:/hw5_Imgdata/bd-01.png");
  //
  //Matching 02: Build the ROI from basic regions
  GenRectangle1(&ho_ModelRegion, 179.539, 145.096, 312.621, 457.417);
  //
  //Matching 02: Reduce the model template
  ReduceDomain(ho_Image, ho_ModelRegion, &ho_TemplateImage);
  //
  //Matching 02: Create the shape model
  CreateShapeModel(ho_TemplateImage, 3, HTuple(0).TupleRad(), HTuple(360).TupleRad(), 
      HTuple(0.7316).TupleRad(), (HTuple("point_reduction_medium").Append("no_pregeneration")), 
      "use_polarity", ((HTuple(25).Append(30)).Append(4)), 7, &hv_ModelID);
  //
  //Matching 02: Get the model contour for transforming it later into the image
  GetShapeModelContours(&ho_ModelContours, hv_ModelID, 1);
  //
  //Matching 02: Get the reference position
  AreaCenter(ho_ModelRegion, &hv_ModelRegionArea, &hv_RefRow, &hv_RefColumn);
  VectorAngleToRigid(0, 0, 0, hv_RefRow, hv_RefColumn, 0, &hv_HomMat2D);
  AffineTransContourXld(ho_ModelContours, &ho_TransContours, hv_HomMat2D);
  //
  //Matching 02: Display the model contours
  if (HDevWindowStack::IsOpen())
    DispObj(ho_Image, HDevWindowStack::GetActive());
  if (HDevWindowStack::IsOpen())
    SetColor(HDevWindowStack::GetActive(),"green");
  if (HDevWindowStack::IsOpen())
    SetDraw(HDevWindowStack::GetActive(),"margin");
  if (HDevWindowStack::IsOpen())
    DispObj(ho_ModelRegion, HDevWindowStack::GetActive());
  if (HDevWindowStack::IsOpen())
    DispObj(ho_TransContours, HDevWindowStack::GetActive());
  // stop(...); only in hdevelop
  //
  //Matching 02: END of generated code for model initialization
  //Matching 02:  * * * * * * * * * * * * * * * * * * * * * * *
  //Matching 02: BEGIN of generated code for model application
  //
  //Matching 02: Loop over all specified test images
  hv_TestImages.Clear();
  hv_TestImages[0] = "D:/hw5_Imgdata/bd-01.png";
  hv_TestImages[1] = "D:/hw5_Imgdata/bd-02.png";
  hv_TestImages[2] = "D:/hw5_Imgdata/bd-03.png";
  hv_TestImages[3] = "D:/hw5_Imgdata/bd-04.png";
  hv_TestImages[4] = "D:/hw5_Imgdata/bd-05.png";
  hv_TestImages[5] = "D:/hw5_Imgdata/bd-06.png";
  hv_TestImages[6] = "D:/hw5_Imgdata/bd-07.png";
  hv_TestImages[7] = "D:/hw5_Imgdata/bd-08.png";
  hv_TestImages[8] = "D:/hw5_Imgdata/bd-09.png";
  hv_TestImages[9] = "D:/hw5_Imgdata/bd-10.png";
  hv_TestImages[10] = "D:/hw5_Imgdata/bd-11.png";
  hv_TestImages[11] = "D:/hw5_Imgdata/bd-12.png";
  hv_TestImages[12] = "D:/hw5_Imgdata/bd-13.png";
  hv_TestImages[13] = "D:/hw5_Imgdata/bd-14.png";
  hv_TestImages[14] = "D:/hw5_Imgdata/bd-15.png";
  hv_TestImages[15] = "D:/hw5_Imgdata/bd-16.png";
  hv_TestImages[16] = "D:/hw5_Imgdata/bd-17.png";
  hv_TestImages[17] = "D:/hw5_Imgdata/bd-18.png";
  hv_TestImages[18] = "D:/hw5_Imgdata/bd-19.png";
  hv_TestImages[19] = "D:/hw5_Imgdata/bd-20.png";
  for (hv_T=0; hv_T<=19; hv_T+=1)
  {
    //
    //Matching 02: Obtain the test image
    ReadImage(&ho_Image, HTuple(hv_TestImages[hv_T]));
    //
    //Matching 02: Find the model
    FindShapeModel(ho_Image, hv_ModelID, HTuple(0).TupleRad(), HTuple(360).TupleRad(), 
        0.5, 1, 0.5, "least_squares", (HTuple(3).Append(1)), 0.75, &hv_Row, &hv_Column, 
        &hv_Angle, &hv_Score);
    //
    //Matching 02: Transform the model contours into the detected positions
    if (HDevWindowStack::IsOpen())
      DispObj(ho_Image, HDevWindowStack::GetActive());
    {
    HTuple end_val50 = (hv_Score.TupleLength())-1;
    HTuple step_val50 = 1;
    for (hv_I=0; hv_I.Continue(end_val50, step_val50); hv_I += step_val50)
    {
      HomMat2dIdentity(&hv_HomMat2D);
      HomMat2dRotate(hv_HomMat2D, HTuple(hv_Angle[hv_I]), 0, 0, &hv_HomMat2D);
      HomMat2dTranslate(hv_HomMat2D, HTuple(hv_Row[hv_I]), HTuple(hv_Column[hv_I]), 
          &hv_HomMat2D);
      AffineTransContourXld(ho_ModelContours, &ho_TransContours, hv_HomMat2D);
      if (HDevWindowStack::IsOpen())
        SetColor(HDevWindowStack::GetActive(),"green");
      if (HDevWindowStack::IsOpen())
        DispObj(ho_TransContours, HDevWindowStack::GetActive());
      // stop(...); only in hdevelop
    }
    }
  }
  //
  //Matching 02: *******************************************
  //Matching 02: END of generated code for model application
  //Matching 02: *******************************************
  //

}


#ifndef NO_EXPORT_APP_MAIN

#ifdef __APPLE__
// On OS X systems, we must have a CFRunLoop running on the main thread in
// order for the HALCON graphics operators to work correctly, and run the
// action function in a separate thread. A CFRunLoopTimer is used to make sure
// the action function is not called before the CFRunLoop is running.
// Note that starting with macOS 10.12, the run loop may be stopped when a
// window is closed, so we need to put the call to CFRunLoopRun() into a loop
// of its own.
HTuple      gStartMutex;
H_pthread_t gActionThread;
HBOOL       gTerminate = FALSE;

static void timer_callback(CFRunLoopTimerRef timer, void *info)
{
  UnlockMutex(gStartMutex);
}

static Herror apple_action(void **parameters)
{
  // Wait until the timer has fired to start processing.
  LockMutex(gStartMutex);
  UnlockMutex(gStartMutex);

  try
  {
    action();
  }
  catch (HException &exception)
  {
    fprintf(stderr,"  Error #%u in %s: %s\n", exception.ErrorCode(),
            exception.ProcName().TextA(),
            exception.ErrorMessage().TextA());
  }

  // Tell the main thread to terminate itself.
  LockMutex(gStartMutex);
  gTerminate = TRUE;
  UnlockMutex(gStartMutex);
  CFRunLoopStop(CFRunLoopGetMain());
  return H_MSG_OK;
}

static int apple_main(int argc, char *argv[])
{
  Herror                error;
  CFRunLoopTimerRef     Timer;
  CFRunLoopTimerContext TimerContext = { 0, 0, 0, 0, 0 };

  CreateMutex("type","sleep",&gStartMutex);
  LockMutex(gStartMutex);

  error = HpThreadHandleAlloc(&gActionThread);
  if (H_MSG_OK != error)
  {
    fprintf(stderr,"HpThreadHandleAlloc failed: %d\n", error);
    exit(1);
  }

  error = HpThreadCreate(gActionThread,0,apple_action);
  if (H_MSG_OK != error)
  {
    fprintf(stderr,"HpThreadCreate failed: %d\n", error);
    exit(1);
  }

  Timer = CFRunLoopTimerCreate(kCFAllocatorDefault,
                               CFAbsoluteTimeGetCurrent(),0,0,0,
                               timer_callback,&TimerContext);
  if (!Timer)
  {
    fprintf(stderr,"CFRunLoopTimerCreate failed\n");
    exit(1);
  }
  CFRunLoopAddTimer(CFRunLoopGetCurrent(),Timer,kCFRunLoopCommonModes);

  for (;;)
  {
    HBOOL terminate;

    CFRunLoopRun();

    LockMutex(gStartMutex);
    terminate = gTerminate;
    UnlockMutex(gStartMutex);

    if (terminate)
      break;
  }

  CFRunLoopRemoveTimer(CFRunLoopGetCurrent(),Timer,kCFRunLoopCommonModes);
  CFRelease(Timer);

  error = HpThreadHandleFree(gActionThread);
  if (H_MSG_OK != error)
  {
    fprintf(stderr,"HpThreadHandleFree failed: %d\n", error);
    exit(1);
  }

  ClearMutex(gStartMutex);
  return 0;
}
#endif

int main(int argc, char *argv[])
{
  int ret = 0;

  try
  {
#if defined(_WIN32)
    SetSystem("use_window_thread", "true");
#endif

    // file was stored with local-8-bit encoding
    //   -> set the interface encoding accordingly
    SetHcppInterfaceStringEncodingIsUtf8(false);

    // Default settings used in HDevelop (can be omitted)
    SetSystem("width", 512);
    SetSystem("height", 512);

#ifndef __APPLE__
    action();
#else
    ret = apple_main(argc,argv);
#endif
  }
  catch (HException &exception)
  {
    fprintf(stderr,"  Error #%u in %s: %s\n", exception.ErrorCode(),
            exception.ProcName().TextA(),
            exception.ErrorMessage().TextA());
    ret = 1;
  }
  return ret;
}

#endif


#endif


