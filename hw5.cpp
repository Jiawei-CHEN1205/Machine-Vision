/*****************************************************************************
* matching.cpp
*****************************************************************************
*
* Project:      HALCON/C++
* Description:  Example program for HALCON/C++
*
* (c) 2011-2019 by MVTec Software GmbH
*               www.mvtec.com
*****************************************************************************
*
* Locating a chip on a board and measuring the pins
*
****************************************************************************/

#ifndef __APPLE__
#  include "HalconCpp.h"
#  include "HDevThread.h"
#  if defined(__linux__)
#    include <X11/Xlib.h>
#  endif
#else
#  ifndef HC_LARGE_IMAGES
#    include <HALCONCpp/HalconCpp.h>
#    include <HALCONCpp/HDevThread.h>
#  else
#    include <HALCONCppxl/HalconCpp.h>
#    include <HALCONCppxl/HDevThread.h>
#  endif
#  include <stdio.h>
#  include <HALCON/HpThread.h>
#  include <CoreFoundation/CFRunLoop.h>
#endif


using namespace HalconCpp;
using namespace std;

void click(const HalconCpp::HWindow& w);
void run(void);


bool singleStep = false;


void click(const HalconCpp::HWindow& w)
{
    printf("Click with mouse button in window to continue ...\n");
    w.Click();

    Hlong mrow, mcolumn, mbutton;
    do
    {
        try
        {
            w.GetMposition(&mrow, &mcolumn, &mbutton);
        }
        catch (HException&)
        {
            // Mouse cursor outside window
            mbutton = 0;
        }
    } while (mbutton != 0);
}


void run(void)
{
    // Initialize acquisition

    HFramegrabber acq("File", 1, 1, 0, 0, 0, 0, "default", -1, "default", -1, "default",
        "board/board.seq", "default", -1, 1);

    HImage image = acq.GrabImage();
    Hlong  width, height;
    image.GetImageSize(&width, &height);

    // Initialize window

    HSystem::SetWindowAttr("border_width", HTuple(0));
    HWindow window(16, 16, width, height, 0, "visible", "");

    window.SetPart(0, 0, height - 1, width - 1);
    window.SetLineWidth(3);
    window.DispObj(image);

    // Init Model

    HRegion rectangle(188, 182, 298, 412);

    // Compute the coordinates of the measurement rectangles relative to the
    // center of the model. Note that area itself (i.e., the return value of
    // AreaCenter) is not used.
    double  center_row, center_column;
    rectangle.AreaCenter(&center_row, &center_column);

    double  rect1_row = center_row - 102;
    double  rect1_col = center_column + 5;
    double  rect2_row = center_row + 107;
    double  rect2_col = center_column + 5;
    double  rectPhi = 0;
    double  rect_length = 170.0;
    double  rect_width = 5.0;


    // Generate the two measurement rectangles for visualization purposes.
    HRegion rectangle1, rectangle2;
    rectangle1.GenRectangle2(rect1_row, rect1_col, rectPhi,
        rect_length, rect_width);
    rectangle2.GenRectangle2(rect2_row, rect2_col, rectPhi,
        rect_length, rect_width);

    // Create an iconic representation of the model.  This region will be
    // transformed by the measured position of the model for visualization
    // purposes later on.
    HImage  image_reduced = image.ReduceDomain(rectangle);
    HRegion shape_model_contours;
    HImage  shape_model_image = image_reduced.InspectShapeModel(&shape_model_contours, 1, 30);

    // Create the model.
    HShapeModel shape_model(image_reduced, 4, 0, 2 * PI, PI / 180,
        "none", "use_polarity", 30, 10);

    // Display the model and measurement rectangles.
    window.SetColor("green");
    window.DispObj(shape_model_contours);
    //window.SetColor("blue");
    //window.SetDraw("margin");
    //window.DispObj(rectangle1);
    //window.DispObj(rectangle2);
    //window.SetDraw("fill");

    click(window);

    Hlong mrow, mcolumn, mbutton;
    do
    {
        image = acq.GrabImage();

        double  dS1, dS2;
        double  rect1_row_check, rect1_col_check, rect2_row_check, rect2_col_check;
        HTuple  row_check, column_check, htAngle_check, score;
        HHomMat2D transformation;
        HTuple  row_edge_first1, column_edge_first1, amplitude_first1;
        HTuple  row_edge_second1, column_edge_second1, amplitude_second1;
        HTuple  intra_distance1, inter_distance1;
        HTuple  row_edge_first2, column_edge_first2, amplitude_first2;
        HTuple  row_edge_second2, column_edge_second2, amplitude_second2;
        HTuple  intra_distance2, inter_distance2;
        HRegion contour_trans;

        double  match_time, match_score, measure_time;

        // Note that flicker-free display is achieved by using
        // SetSystem("flush_graphic","false") here and
        // SetSystem("flush_graphic","true") at the end of this function.

        HSystem::SetSystem("flush_graphic", "false");
        window.DispObj(image);


        // Find the IC in the current image.
        dS1 = HSystem::CountSeconds();
        shape_model.FindShapeModel(image, 0, 2 * PI, 0.7, 1, 0.5, "least_squares",
            4, 0.7, &row_check, &column_check, &htAngle_check, &score);
        dS2 = HSystem::CountSeconds();

        match_time = (dS2 - dS1) * 1000;

        match_score = (double)score[0];

        if (score.Length() == 1)
        {
            double d_row_check = row_check.D();
            double d_column_check = column_check.D();
            double d_angle_check = htAngle_check.D();

            // Rotate the model for visualization purposes.
            transformation.VectorAngleToRigid(center_row, center_column, 0,
                d_row_check, d_column_check, d_angle_check);
            contour_trans = transformation.AffineTransRegion(shape_model_contours, "false");
            //window.SetColor("green");
            window.DispObj(contour_trans);

            // Compute the parameters of the measurement rectangles.
            transformation.AffineTransPixel(rect1_row, rect1_col,
                &rect1_row_check, &rect1_col_check);
            transformation.AffineTransPixel(rect2_row, rect2_col,
                &rect2_row_check, &rect2_col_check);

            // For visualization purposes, generate the two rectangles as regions
            // and display them.

            /*
            HRegion rectangle1, rectangle2;

            rectangle1.GenRectangle2(rect1_row_check, rect1_col_check,
                d_angle_check, rect_length, rect_width);
            rectangle2.GenRectangle2(rect2_row_check, rect2_col_check,
                d_angle_check, rect_length, rect_width);
                */
            //window.SetColor("blue");
            //window.SetDraw("margin");
            //window.DispObj(rectangle1);
            //window.DispObj(rectangle2);
            //window.SetDraw("fill");

            // Do the actual measurements.

            dS1 = HSystem::CountSeconds();

            Hlong num_leads;

            {
                HMeasure measure1(rect1_row_check, rect1_col_check, d_angle_check,
                    rect_length, rect_width, width,
                    height, "bilinear");

                HMeasure measure2(rect2_row_check, rect2_col_check, d_angle_check,
                    rect_length, rect_width, width,
                    height, "bilinear");

                measure1.MeasurePairs(image, 2, 90, "positive", "all",
                    &row_edge_first1, &column_edge_first1,
                    &amplitude_first1, &row_edge_second1,
                    &column_edge_second1, &amplitude_second1,
                    &intra_distance1, &inter_distance1);
                measure2.MeasurePairs(image, 2, 90, "positive", "all",
                    &row_edge_first2, &column_edge_first2,
                    &amplitude_first2, &row_edge_second2,
                    &column_edge_second2, &amplitude_second2,
                    &intra_distance2, &inter_distance2);

                // Measure objects will be destroyed when leaving scope
            }

            dS2 = HSystem::CountSeconds();

            // Display the measurement results.
            //window.SetColor("red");
            /*
            window.DispLine(
                row_edge_first1 - rect_width * htAngle_check.TupleCos(),
                column_edge_first1 - rect_width * htAngle_check.TupleSin(),
                row_edge_first1 + rect_width * htAngle_check.TupleCos(),
                column_edge_first1 + rect_width * htAngle_check.TupleSin());
            window.DispLine(
                row_edge_second1 - rect_width * htAngle_check.TupleCos(),
                column_edge_second1 - rect_width * htAngle_check.TupleSin(),
                row_edge_second1 + rect_width * htAngle_check.TupleCos(),
                column_edge_second1 + rect_width * htAngle_check.TupleSin());
            window.DispLine(
                row_edge_first2 - rect_width * htAngle_check.TupleCos(),
                column_edge_first2 - rect_width * htAngle_check.TupleSin(),
                row_edge_first2 + rect_width * htAngle_check.TupleCos(),
                column_edge_first2 + rect_width * htAngle_check.TupleSin());
            window.DispLine(
                row_edge_second2 - rect_width * htAngle_check.TupleCos(),
                column_edge_second2 - rect_width * htAngle_check.TupleSin(),
                row_edge_second2 + rect_width * htAngle_check.TupleCos(),
                column_edge_second2 + rect_width * htAngle_check.TupleSin());
            */
            //window.SetDraw("fill");

            measure_time = (dS2 - dS1) * 1000;

            // Update the lead number label with the measured number of leads.
            num_leads = intra_distance1.Length() + intra_distance2.Length();

            //printf("Match time %6.3f ms, Score %7.5f, Measure time %5.2f ms, Num Leads %2d\n",
               // match_time, match_score, measure_time, (int)num_leads);
            printf("Match time %6.3f ms, Score %7.5f\n",
                match_time, match_score);
        }

        HSystem::SetSystem("flush_graphic", "true");
        // Force the graphics window to be updated by displaying an empty circle.
        window.DispCircle(-1, -1, 1);

        try
        {
            window.GetMposition(&mrow, &mcolumn, &mbutton);
        }
        catch (HException&)
        {
            // Mouse cursor outside window
            mbutton = 0;
        }
    } while (mbutton == 0);
}


#ifdef __APPLE__
// On OS X systems, we must have a CFRunLoop running on the main thread in
// order for the HALCON graphics operators to work correctly, and run the
// main function in a separate thread. A CFRunLoopTimer is used to make sure
// the action function is not called before the CFRunLoop is running.
// Note that starting with macOS 10.12, the run loop may be stopped when a
// window is closed, so we need to put the call to CFRunLoopRun() into a loop
// of its own.
HTuple      gStartMutex;
H_pthread_t gActionThread;
HBOOL       gTerminate = FALSE;

static void timer_callback(CFRunLoopTimerRef timer, void* info)
{
    UnlockMutex(gStartMutex);
}

static Herror apple_run(void** parameters)
{
    // Wait until the timer has fired to start processing.
    LockMutex(gStartMutex);
    UnlockMutex(gStartMutex);

    run();

    // Tell the main thread to terminate itself.
    LockMutex(gStartMutex);
    gTerminate = TRUE;
    UnlockMutex(gStartMutex);
    CFRunLoopStop(CFRunLoopGetMain());
    return H_MSG_OK;
}

static void apple_main(void)
{
    Herror                error;
    CFRunLoopTimerRef     Timer;
    CFRunLoopTimerContext TimerContext = { 0, 0, 0, 0, 0 };

    CreateMutex("type", "sleep", &gStartMutex);
    LockMutex(gStartMutex);

    error = HpThreadHandleAlloc(&gActionThread);
    if (H_MSG_OK != error)
    {
        fprintf(stderr, "HpThreadHandleAlloc failed: %d\n", error);
        exit(1);
    }

    error = HpThreadCreate(gActionThread, 0, apple_run);
    if (H_MSG_OK != error)
    {
        fprintf(stderr, "HpThreadCreate failed: %d\n", error);
        exit(1);
    }

    Timer = CFRunLoopTimerCreate(kCFAllocatorDefault,
        CFAbsoluteTimeGetCurrent(), 0, 0, 0,
        timer_callback, &TimerContext);
    if (!Timer)
    {
        fprintf(stderr, "CFRunLoopTimerCreate failed\n");
        exit(1);
    }
    CFRunLoopAddTimer(CFRunLoopGetCurrent(), Timer, kCFRunLoopCommonModes);

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

    CFRunLoopRemoveTimer(CFRunLoopGetCurrent(), Timer, kCFRunLoopCommonModes);
    CFRelease(Timer);

    error = HpThreadHandleFree(gActionThread);
    if (H_MSG_OK != error)
    {
        fprintf(stderr, "HpThreadHandleFree failed: %d\n", error);
        exit(1);
    }

    ClearMutex(gStartMutex);
}
#endif


int main(void)
{
#if defined(_WIN32)
    SetSystem("use_window_thread", "true");
#elif defined(__linux__)
    // in case of visualization out of different threads initialize X11:
    // (this requires linking against libX11)
    XInitThreads();
#endif

#ifndef __APPLE__
    run();
#else
    apple_main();
#endif

    return 0;
}