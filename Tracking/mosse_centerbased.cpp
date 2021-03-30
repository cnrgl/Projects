#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/gapi/core.hpp>

using namespace cv;
using namespace std;

#define SSTR( x ) (static_cast< std::ostringstream >(( std::ostringstream() << std::dec << x ) ).str())
#define Center( r ) (Point((r.x + r.width/2),(r.y + r.height/2)))
int mode = 1;
Point BasePoint;
bool clc = false;

const char* winname = "Track";
const int win_size_h = 640, win_size_w = 640;

void WeightedMeanDev(Mat image, Point center, double* mean, double* stddev, InputArray mask = noArray());
void HistFilterBinary(Mat in, Mat out, double mean, double stddev);
void op(Mat in, Mat out, int opt_type);
Rect Rescale(Rect2d bbox, Point center, float sc);
static void onSelect(int, int, int, int, void*);

int main(int argc, char** argv)
{
	CommandLineParser parser(argc, argv, "{ @image | <none> | path to image file }");
	string filename = parser.get<string>("@image");
	string trackerType = "MOSSE";

	Ptr<Tracker>tracker = TrackerMOSSE::create();

	VideoCapture video;
	if (!filename.empty())
	{
		video.open(filename);
		video.set(CAP_PROP_FRAME_WIDTH, win_size_w); // ekran boyutunu ayarla ... 
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
	cout << cv::getBuildInformation << endl;

	// Read first frame 
	Mat frame, frame_gray, ROI_gray;
	bool ok = video.read(frame);
	resize(frame, frame, Size(win_size_w, win_size_h), 0.0, 0.0, INTER_CUBIC);
	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
	imshow("gray_image", frame_gray);
	Rect2d bbox = selectROI(frame);
	//######### set initial frame scale #################
	double m, std;
	ROI_gray = frame_gray(bbox);
	WeightedMeanDev(ROI_gray, Center(bbox), &m, &std);
	m = frame_gray.at<uchar>(Center(bbox)); 
	HistFilterBinary(frame_gray, frame_gray, m, 10);
	imshow("test", ROI_gray); // ROI_frame
	Scalar baseSum = sum(ROI_gray)/(bbox.height*bbox.width);
	cout << "base sum = " << baseSum[0] << "---" << m << "..." << std << endl;
	Rect2d bbox_esp = bbox;
	//##################################################

	// Display bounding box.
	rectangle(frame, bbox, Scalar(255, 0, 0), 2, 1);

	imshow(winname, frame);
	waitKey(0);
	tracker->init(frame, bbox);

	while (true)
	{
		if (mode)
		{
			if (video.read(frame));
			else
				break;
			// Start timer
			resize(frame, frame, Size(win_size_w, win_size_h), 0.0, 0.0, INTER_CUBIC);
			double timer = (double)getTickCount();
		
			cvtColor(frame,frame_gray,COLOR_BGR2GRAY);
			
			// topografik mean calc...
			ROI_gray = frame_gray(bbox_esp);
			// Mat markers = watershed(frame, ); // or opening to remove new

			WeightedMeanDev(ROI_gray,Center(bbox),&m,&std);
			m = frame_gray.at<uchar>(Center(bbox));

			// scale factor calculation ... 
			HistFilterBinary(frame_gray,frame_gray,m,7);

			Scalar sums = sum(ROI_gray)/(bbox_esp.height*bbox_esp.width);
			float sc = sqrt(sums[0]/ baseSum[0]); //scale factor
			cout << "mean = " << m << "-std =" << std << "-sums = " << sums << "--" << sc << "..." << endl;
			
			bbox_esp = Rescale(bbox_esp,Center(bbox),sc);
			
			// Update the tracking result
			ok = tracker->update(frame, bbox);	
			float fps = getTickFrequency() / ((double)getTickCount() - timer);
			imshow("test", ROI_gray); // ROI_frame
			if (ok)
			{
				rectangle(frame, bbox_esp, Scalar(255, 0, 0), 2, 1);
				drawMarker(frame, Center(bbox), Scalar(0, 255, 0));
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
		if (keyboard == 'q' || keyboard == 27)
			break;
		else if (keyboard == 'p' || keyboard == 112)
			mode = 0;
		else if (keyboard == 'r' || keyboard == 114)
			mode = 1;
		else if (keyboard == 'c' || keyboard == 99) // pixel color finder
		{
			setMouseCallback(winname, onSelect, 0);
			cout << "pick color" << endl;
			while (mode)
			{
				waitKey(5);
				int color = (int)frame_gray.at<uchar>(BasePoint);
				if (clc)
				{
					cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
					putText(frame_gray, "value = " + SSTR(color), Point(100, 100), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50, 170, 50), 2);
					imshow(winname, frame_gray);
					clc = false;
				}
			}
			mode = 0;
		}
	}
}
void WeightedMeanDev(Mat image, Point center, double* mean, double* stddev, InputArray mask)
{
	Mat src, weights = Mat::zeros(image.rows, image.cols, CV_32FC1);
	image.convertTo(src, CV_32FC1);
	int space = image.rows * image.cols;
	for (int i = 0; i < image.rows; i++)
		for (int j = 0; j < image.cols; j++)
			weights.at<float>(i, j) += exp(MIN(image.rows, image.cols) / cv::norm(Point(i, j) - center)); // L2 norm
	float N = sum(weights)[0];
	*mean = (double)sum(weights.mul(src) / N)[0];
	*stddev = 0;//sqrt(sum((src - mean[0]).mul(src - mean[0]))[0] / space);
}
void HistFilterBinary(Mat in, Mat out, double mean, double stddev)
{
	Mat high_pass, low_pass;
	threshold(in,high_pass, MIN(255,mean - stddev), 255, THRESH_BINARY);
	threshold(in,low_pass,MAX(0,mean + stddev), 255, THRESH_BINARY_INV);
	out = high_pass & low_pass;
}
Rect Rescale(Rect2d bbox, Point center, float sc)
{
	float w, h;
	w = MAX(bbox.width * sc,3);
	h = MAX(bbox.height * sc,3);

	return Rect(center.x - w / 2, center.y - h / 2, w, h);
}
static void onSelect(int event, int x, int y, int, void*)
{
	if (event == EVENT_LBUTTONDBLCLK)
	{
		BasePoint = Point(x, y);
		clc = true;
		cout << "left click" << endl;
	}
	else if (event == EVENT_RBUTTONDBLCLK)
	{
		mode = 0;
		cout << "right click" << endl;
	}
}
