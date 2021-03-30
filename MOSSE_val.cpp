#include <windows.h>
#include <winbase.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/gapi/core.hpp> // GPU API library

#define DEBUG1

using namespace cv;
using namespace std;

#define SSTR( x ) (static_cast< std::ostringstream >(( std::ostringstream() << std::dec << x ) ).str()) // number to string / itoa()
#define Center( r ) (Point((r.x + r.width/2),(r.y + r.height/2))) // center point of the rect r
#define frame_ratio 15
int dist_rows, dist_cols;
int mode = 1; // player modes --> play - 1 : stop - 0

const char* winname = "Track";
const int win_size_h = 640, win_size_w = 640; // fixed win size

void foregroundHistProb(Mat in, Mat &hist, Mat& histProb, int val = 3); // foreground prob. function for object color prob. map 
Rect Rescale(Rect2d bbox, Point center, float scw, float sch); // box rescale function with respect to the center

HANDLE hPort;
bool writeByte(char *data);
BOOL WriteABuffer(char* lpBuf, DWORD dwToWrite);


int main(int argc, char** argv)
{
	CommandLineParser parser(argc, argv, "{ @image | <none> | path to image file }");
	string filename = parser.get<string>("@image");

	DCB dcb = { 0 };
	hPort=CreateFile(L"COM3",
		GENERIC_READ | GENERIC_WRITE,
		0,
		0,
		OPEN_EXISTING,
		FILE_FLAG_OVERLAPPED,
		0);
	if (hPort == INVALID_HANDLE_VALUE);
	::GetCommState(hPort, &dcb);
	dcb.BaudRate = 9600;
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;
	SetCommState(hPort, &dcb);



	COMMTIMEOUTS timeout = { 0 };
	timeout.ReadIntervalTimeout = 50;
	timeout.ReadTotalTimeoutConstant = 50;
	timeout.ReadTotalTimeoutMultiplier = 50;
	timeout.WriteTotalTimeoutConstant = 50;
	timeout.WriteTotalTimeoutMultiplier = 10;
	SetCommTimeouts(hPort, &timeout);

	Ptr<Tracker>tracker = TrackerMOSSE::create();//Tracker declaration

	VideoCapture video;
	if (!filename.empty())
	{
		video.open(filename);
		video.set(CAP_PROP_FRAME_WIDTH, win_size_w); // resize the screen
		video.set(CAP_PROP_FRAME_HEIGHT, win_size_h);
	}
	else
		video.open(0);
	// Exit if video is not opened
	if (!video.isOpened())
	{
		cout << "Could not read video file" << endl;
		waitKey(10);
		return 1;
	}
	cout << cv::getBuildInformation << endl; // get build inf - contrib is installed ?

	Mat frame, frame_gray, ROI_gray; 
	// Read first frame 	
	bool ok = video.read(frame);
	resize(frame, frame, Size(win_size_w, win_size_h), 0.0, 0.0, INTER_CUBIC); // resize frame

	cvtColor(frame, frame_gray, COLOR_BGR2GRAY); 
	imshow("gray_image", frame_gray); // show gray image 
	
	Rect2d bbox = selectROI(frame); // ROI select 
	ROI_gray = frame_gray(bbox);
	Rect2d bbox_exp=bbox; //expected box

	Mat probmap; // base probmap
	Mat back_hist_old = Mat(Size(1, 256), CV_32F, Scalar(0));
	dist_rows = ROI_gray.rows / frame_ratio; // çerçeve oranı
	dist_cols = ROI_gray.cols / frame_ratio;
	foregroundHistProb(ROI_gray, back_hist_old, probmap); // calc probs. just for the ROI area
	//if(probmap.empty())
		//cout<<"empty"<<endl;
	
	rectangle(ROI_gray, Rect(Point(5, 5), Point(ROI_gray.cols - 5, ROI_gray.rows - 5)),Scalar(255,0,0)); // background rect drawing
	imshow("foreprob", ROI_gray);
	
	//######### set initial frame scale with moment #################
	Moments mu = moments(probmap);
	float X = mu.m10 / mu.m00;
	float Y = mu.m01 / mu.m00;
	float a = (mu.m20 / mu.m00) - X * X;
	float b = 2 * ((mu.m11 / mu.m00) - X * Y);
	float c = (mu.m02 / mu.m00) - Y * Y;
	float baseWidth = sqrt(((a+c)-sqrt(b*b+(a-c)*(a-c)))/2);
	float baseHeight = sqrt(((a + c) + sqrt(b * b + (a - c) * (a - c))) / 2);
	cout << "base values(width-height) =  " << baseWidth << ":" << baseHeight << endl;
	//###################################################

	// Display bounding box.
	rectangle(frame, bbox, Scalar(255, 0, 0), 2, 1);
	imshow(winname, frame);

	waitKey(0);
	tracker->init(frame, bbox); // initialize tracker

	while (true)
	{
		if (mode)
		{
			if (video.read(frame));
			else
				break; // if frame error occurs
			// Start timer
			resize(frame, frame, Size(win_size_w, win_size_h), 0.0, 0.0, INTER_CUBIC);
			double timer = (double)getTickCount();
			cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
			ok = tracker->update(frame_gray, bbox); // Update the tracking result
			 //expected box update
			dist_rows = ROI_gray.rows / frame_ratio; // çerçeve oranı
			dist_cols = ROI_gray.cols / frame_ratio;
			bbox_exp = Rect(Center(bbox)-Point((bbox_exp.width+dist_cols)/2,(bbox_exp.height+dist_rows)/2),Center(bbox)+Point((bbox_exp.width+dist_cols)/2,(bbox_exp.height+dist_rows)/2)); // get new object square position to calc size
			ROI_gray = frame_gray(bbox_exp); // ROI with new position but old size 
			
			foregroundHistProb(ROI_gray,back_hist_old, probmap);
			//blur(probmap,probmap,Size(2,2),Point(-1,-1));
			mu = moments(probmap);
			X = mu.m10 / mu.m00;
			Y = mu.m01 / mu.m00;
			a = (mu.m20 / mu.m00) - X * X;
			b = 2 * ((mu.m11 / mu.m00) - X * Y);
			c = (mu.m02 / mu.m00) - Y * Y;
			float width_exp = sqrt(((a + c) - sqrt(b * b + (a - c) * (a - c))) / 2);
			float height_exp = sqrt(((a + c) + sqrt(b * b + (a - c) * (a - c))) / 2);
			bbox_exp = Rescale(bbox, Center(bbox), width_exp / baseWidth, height_exp / baseHeight);

			float fps = getTickFrequency() / ((double)getTickCount() - timer);
			cout << "values(width-height) =  " << baseWidth << ":" << baseHeight << " scale(w-h) = " << width_exp / baseWidth << "..." << height_exp / baseHeight << "..." << endl;
			//writeByte((char*)"test");
			if (WriteABuffer((char*)"test", NULL))
				cout << "ok" << endl;
			else
				cout << "error" << endl;
			if (ok)
			{
				rectangle(frame, bbox_exp, Scalar(255, 0, 0), 2, 1);
				drawMarker(frame, Center(bbox), Scalar(0, 255, 0)); //mark the center 
			}
			else
			{
				// Tracking failure detected.
				putText(frame, "Tracking failure detected", Point(100, 80), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 255), 2);
			}

			// Display FPS on frame
			putText(frame, "FPS : " + SSTR(int(fps)), Point(100, 50), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50, 170, 50), 2);
		}
		// Display frame.
		imshow(winname, frame);
		waitKey(0); // frame by frame

		int keyboard = waitKey(5);
		if (keyboard == 'q' || keyboard == 27) // quit
			break;
		else if (keyboard == 'p' || keyboard == 112) // pause
			mode = 0;
		else if (keyboard == 'r' || keyboard == 114) // return
			mode = 1;
		
	}
}
/*void momentVector(Mat probHist)
{
	vector<uchar> cord = {0,0,0,0};
	for (auto vec:cord)
		//for ()
		{

		}
}*/
Rect Rescale(Rect2d bbox, Point center, float scw, float sch)
{
	float w, h;
	w = MAX(bbox.width * scw, 3);
	h = MAX(bbox.height * sch, 3);
	return Rect(center.x - w / 2, center.y - h / 2, w, h);
}
void foregroundHistProb(Mat in, Mat &hist, Mat& probHist, int val) // gives most likelihood probs. being in foreground values ---- foreground but discarded background values  
{
	Mat mask = Mat(in.size(), CV_8U, Scalar(0)); // foreground mask
	mask(Rect(Point(dist_cols, dist_rows), Point(in.cols - dist_cols, in.rows - dist_rows))).setTo(Scalar::all(255)); // set all pixel in mask

	Mat back_hist, fore_hist;
	float range[] = { 0,255 }; // one channel gray image parameters
	const float* ranges[] = {range};
	int hist_size[] = { 256 };//bins = 256
	int channels[] = { 0 };
	
	calcHist(&in, 1, channels, 255-mask, back_hist, 1, hist_size, ranges, true, false); //background hist calc. with back mask or border mask
	calcHist(&in, 1, channels, mask, fore_hist, 1, hist_size, ranges, true, false);
	

	normalize(back_hist, back_hist, 0, 255, NORM_MINMAX, -1, Mat()); //normalize histograms
	normalize(fore_hist, fore_hist, 0, 255, NORM_MINMAX, -1, Mat());
	threshold(back_hist, back_hist, val, 255, THRESH_BINARY); // val == arkaplandan bir değerin alınma sınırı default 4 - 4 pixel veya az varsa alınabilir demek
	hist = back_hist;// | hist;
	fore_hist = fore_hist &(255 - hist); // XOR(or substract) foreground with background
	normalize(fore_hist, fore_hist, 0, 255, NORM_MINMAX, -1, Mat()); // normalize values in case of values drop quickly  
	
	#ifdef DEBUG1 // draw hist lines for visualization purposes 
	Mat histImage = Mat::zeros(600, 600, CV_8UC3);
	int hist_w = 512, hist_h = 400;
	int bin_w = cvRound((double)hist_w / 256);
	for (int i = 1; i < 256; i++)
	{
		line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(hist.at<float>(i - 1))),
			Point(bin_w * (i), hist_h - cvRound(hist.at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0);
		line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(fore_hist.at<float>(i - 1))),
			Point(bin_w * (i), hist_h - cvRound(fore_hist.at<float>(i))),
			Scalar(0, 255, 0), 2, 8, 0);
	}
	imshow("calcHist Demo", histImage); // histogram to look up table
	#endif

	Mat out;
	calcBackProject(&in, 1, channels, fore_hist, out, ranges); // backproject from histogram according to input image
	threshold(out, probHist, 1, 255, THRESH_BINARY); // BINARY quantization to probs so values takes only 1 or 0 - this case we want to take all pixel same weighted
	imshow("prob Demo", probHist);
}
BOOL WriteABuffer(char* lpBuf, DWORD dwToWrite)
{
	OVERLAPPED osWrite = { 0 };
	DWORD dwWritten;
	BOOL fRes;

	// Create this writes OVERLAPPED structure hEvent.
	osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (osWrite.hEvent == NULL)
		// Error creating overlapped event handle.
		return FALSE;

	// Issue write.
	if (!WriteFile(hPort, lpBuf, dwToWrite, &dwWritten, &osWrite)) {
		if (GetLastError() != ERROR_IO_PENDING) {
			// WriteFile failed, but it isn't delayed. Report error and abort.
			fRes = FALSE;
		}
		else {
			// Write is pending.
			if (!GetOverlappedResult(hPort, &osWrite, &dwWritten, TRUE))
				fRes = FALSE;
			else
				// Write operation completed successfully.
				fRes = TRUE;
		}
	}
	else
		// WriteFile completed immediately.
		fRes = TRUE;

	CloseHandle(osWrite.hEvent);
	return fRes;
}
bool writeByte(char *data)
{
	hPort=CreateFile(L"COM3", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	//cout << GetLastError() << endl;
	DCB dcb = { 0 };
	BYTE Byte;
	DWORD byteswritten, dwBytesTransferred;
	dcb.DCBlength = sizeof(DCB);

	::GetCommState(hPort, &dcb);
	dcb.BaudRate = 9600;
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;
	SetCommState(hPort, &dcb);


	
	COMMTIMEOUTS timeout = { 0 };
	timeout.ReadIntervalTimeout = 50;
	timeout.ReadTotalTimeoutConstant = 50;
	timeout.ReadTotalTimeoutMultiplier = 50;
	timeout.WriteTotalTimeoutConstant = 50;
	timeout.WriteTotalTimeoutMultiplier = 10;
	SetCommTimeouts(hPort, &timeout);
	
	/*uintptr_t u_adress;
	std::stringstream ss;
	ss << std::hex << data;
	ss >> u_adress;
	LPCVOID address = (LPCVOID)u_adress;*/
	//bool retVal = WriteFile(hPort, address, 1, &byteswritten, NULL);
	bool retVal = WriteABuffer(data,NULL);
	ReadFile(hPort, &Byte, 2, &dwBytesTransferred, 0);
	cout << "scanned data:"<<Byte<<"--"<<retVal<<endl;
	CloseHandle(hPort);
	return retVal;
}