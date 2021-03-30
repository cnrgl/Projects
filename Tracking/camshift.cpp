#include <iostream> // gerekli tüm kütüphaneler
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
using namespace cv;
using namespace std; // buna aslında gerek yok - std:: etiketi kullanıldı

////////////  global değişken listesi  \\\\\\\\\\\\\\\\\\/
Mat image;
// ekran tanımlamaları - win size const olursa hata oluşmaz hem depolama açısından daha uygun
const string winname = "test_window";
const string window_detection_name = "Detection";
const int win_size_h = 640, win_size_w = 640; 
bool mode = 1;

int loss = 3; // ROI penceresi için kaybolma şartı == en küçük win alanı
bool color_select = false; // renk seçimi bayrağı
bool SelectObject = false; // ROI seçim bayrağı
bool ROI_select = true; // ROI - renk seçimi devam ediyor mu ? etmiyorsa bayrak false olur
Point BasePoint; //imleç konumu için geçici alan - imleç registerı
Rect Selection; // imleç konumuna göre güncellenen ROI rect
Vec3b color; // color tutucu

const int max_value_H=180, max_value=255; // trackbar için constraint
int hue_l=30, hue_h=max_value_H, sat_l=60, sat_h=255, val_l=32, val_h=255; // filte - trackbar hsv değer tutucular

void meanDev_weighted(Mat image, Point center, double* mean, double* stddev, InputArray);
static void onWindow(int , int , int , int, void*); // fare tıklaması ile tetiklenen seçim fonksiyonu

// trackbar fonksiyon deklarasyonları - tanımlar aşağıda 
static void on_hue_l_thresh_trackbar(int, void*); 
static void on_hue_h_thresh_trackbar(int, void*);
static void on_sat_l_thresh_trackbar(int, void*);
static void on_sat_h_thresh_trackbar(int, void*);
static void on_val_l_thresh_trackbar(int, void*);
static void on_val_h_thresh_trackbar(int, void*);

int main(int argc, char** argv)
{
	char str[100]; // video'ya string yazmak için hafıza
	VideoCapture capture; // capture değişkeni - duruma bağlı giriş verilecek --> initialization aşağıda 
	Mat frame, roi, hsv_roi, mask; // Matris değişkenleri
	Mat roi_hist, hsv, dst;
	Point2f points[4]; // rotated rect noktaları

	CommandLineParser parser(argc, argv, "{ @image |<none>| path to image file }");
	string filename = parser.get<string>("@image"); // dosya isminin parse edilmesi - farklı yollar var örnektekini değiştirmedim 


	if (!filename.empty()) // kameradan almıyorsak dosya konumu verilmeli yoksa kameraya devam ...
	{
		capture.open(filename); // dosya konumundan video girişini al
		capture.set(CAP_PROP_FRAME_WIDTH, win_size_w); // ekran boyutunu ayarla ... 
		capture.set(CAP_PROP_FRAME_HEIGHT, win_size_h);
	}
	else
		capture.open(0,CAP_ANY); // kamera girişi

	if (!capture.isOpened()) { // giriş uygun şekilde açıldı mı ?
		cerr << "Unable to open file!" << endl;
		return 0;
	}
	
	// ekranın önceden açılması gerekli -- trackbar görülmeli
	namedWindow(winname); // ana ekran -- bu sonra da açılabilir 
	namedWindow(window_detection_name);

	// trackbar oluşturuluyor ...
	createTrackbar("Low H", window_detection_name, &hue_l, max_value_H, on_hue_l_thresh_trackbar);
	createTrackbar("High H", window_detection_name, &hue_h, max_value_H, on_hue_h_thresh_trackbar);
	createTrackbar("Low S", window_detection_name, &sat_l, max_value, on_sat_l_thresh_trackbar);
	createTrackbar("High S", window_detection_name, &sat_h, max_value, on_sat_h_thresh_trackbar);
	createTrackbar("Low V", window_detection_name, &val_l, max_value, on_val_l_thresh_trackbar);
	createTrackbar("High V", window_detection_name, &val_h, max_value, on_val_h_thresh_trackbar); 

	capture.read(frame); // capture.read() fonskiyonu giriş gelene kadar bekler ...  -- kamera girişi sorunlu olabiliyor

	if (frame.empty()) // Tekrar bir kontrolün zararı olmaz ... -- olmasa da olur silinebilir
		std::cerr << "frame is empty !!!";

	resize(frame, frame, Size(win_size_w, win_size_h), 0.0, 0.0, INTER_CUBIC); 	// ekran boyutlarını ayarla...
	frame.copyTo(image); // üstünde oynama yapılacak frame i image e kopyala -- frame yedeklendi ...

	putText(image, "Pencere Sec ve renk ayarini yap", Point(10, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(100, 255, 100), 1, LINE_AA); // ekrana TEXT yazmak için fonksiyon
	imshow(winname, image); // seçim başlangıcı için ekran gösteriliyor...

	waitKey(0); // geçici bekleme -- yazı gözükmeli

	setMouseCallback(winname, onWindow, 0); // mouse tetikleme fonksiyonu -- onWindow -> interrupt fonksiyonu

	do{ // ROI seçim döngüsü 
		frame.copyTo(image); // frame alındı -- GEÇİCİ ROI kareleri silinmiş oldu
		waitKey(5); // mouse feedback için bekleme süresi -- yoksa click alınmıyor
		rectangle(image, Selection, cv::Scalar(0, 0, 255)); // mouse R_button gelene kadar GEÇİCİ KAREYİ çiz 
		imshow(winname, image); // yeni image i bas
	} while (ROI_select); // ROI - color selection bitti mi ?

	drawMarker(image, BasePoint, Scalar(10, 255, 50), 3, 10, 1); // color selection yapılan konumu seçtikten sonra kullanıcıya göster ...
	imshow(winname, image); // son halini ekrana bas
	waitKey(0);
	roi = frame(Selection); // roi yi al

	int k = 3;
	Mat roiv,rois,element = getStructuringElement(MORPH_RECT,Size(2 * k + 1, 2 * k + 1),Point(k,k));//Mat(k, k, CV_8UC3, Scalar(1,1,1));
	//morphologyEx(roi, roi, MORPH_CLOSE,element);
	double std,std2,m,m2;
	extractChannel(roi, roiv, 2);
	extractChannel(roi, rois, 1);
	cout << "test" << endl;

	meanDev_weighted(rois, BasePoint, &m2, &std2, 0);
	meanDev_weighted(roiv, BasePoint, &m, &std, 0);
	cout << "mean color[0] = " << m << "---"<< std<<"--"<<std2;

	imshow("roi", roiv);
	cout << "mean1" << color[1] << endl;
	// BURASI ÖNEMLİ -- mask için değerlerin uygulanması \\/
	hue_l = MAX(0, color[0] - 5); // seçim renginden uygun min değer sınırlarını belirle
	hue_h = MIN(180, color[0] + 10);
	sat_l = MAX(0, m2 - std2);
	sat_h = MIN(255, m2 + std2);
	val_l = MAX(0, m - std);
	val_h = MIN(255, m + std);
	on_hue_l_thresh_trackbar(0, 0); // trackbar güncelleme
	on_hue_h_thresh_trackbar(0, 0);
	on_sat_l_thresh_trackbar(0, 0);
	on_sat_h_thresh_trackbar(0, 0);
	on_val_l_thresh_trackbar(0, 0);
	on_val_h_thresh_trackbar(0, 0);

	waitKey(0); // renk seçim işlemi bitene kadar bekle

	cvtColor(roi, hsv_roi, COLOR_BGR2HSV); // ROI'yi hsv formatına dönüştür
	inRange(hsv_roi, Scalar(hue_l,sat_l,val_l), Scalar(hue_h,sat_h,val_h), mask); // ROI için mask yap

	float h_range[] = { 0, 180 }; // hue hist range
	float s_range[] = { 0, 255 }; // sat hist range
	float v_range[] = { 0, 255 }; // val hist range
	const float* ranges[] = { h_range , s_range}; // ranges değişkeni
	int histSize[] = { 50 , 50 , 50 }; // hist sizes
	int channels[] = { 0, 1 }; // 0' dan 2'e olan channelları kabul et

	calcHist(&hsv_roi, 1, channels, mask, roi_hist, 2, histSize, ranges, true, false); // uygun mask ile histogram hesabı yapıldı
	normalize(roi_hist, roi_hist, 0, 255, NORM_MINMAX); // histogram backprojection için normalize edildi

	putText(image, "Pencere Secildi", Point(10,15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(100, 255, 100), 1, LINE_AA); // seçim bittiğine dair mesaj ekrana yazıldı ... 
	imshow(winname, image); // mesaj göster
	waitKey(0); // işlem başlasın mı ?

	while (true) {
		if (mode == 1)
		{
			capture >> frame; // frame al
			if (frame.empty()) // frame kontrol
				break;

			resize(frame, frame, Size(win_size_w, win_size_h), 0.0, 0.0, INTER_CUBIC); // ekran boyutuna uygun boyutlandırma yapıldı
			cvtColor(frame, hsv, COLOR_BGR2HSV); // frame hsv formatına dönüştürüldü
		}
		
		inRange(hsv, Scalar(hue_l, sat_l, val_l), Scalar(hue_h, sat_h, val_h), mask); // ekran maskesi için range
		Mat test = frame.clone();
		test.setTo(Scalar(0,0,0),(255-mask));
		
		imshow("mask applied image",test);

		calcBackProject(&hsv, 1, channels, roi_hist, dst, ranges); // backproject olasılıksal hesaplaması yapıldı
		dst &= mask; // ADAPTİF FİLTRELEME -- backproject olasılıklarını renk maskesiyle karşılaştır...
		imshow(window_detection_name, dst); // backproject çıktısı alındı
		
		if (mode == 1) {
			RotatedRect rot_rect = CamShift(dst, Selection, TermCriteria(TermCriteria::EPS | TermCriteria::COUNT, 10, 1)); // camshift ile window hareket işlemi yapıldı + camshift çıktısı alındı 
			if (!(rot_rect.size.width * rot_rect.size.height > loss)) // yeni window size kayıp kriterimize uygun mu ? -- rotated rect alanı kriter kabul edildi
			{
				putText(frame, "Object has disappeared !!!", Point(50, 50), FONT_HERSHEY_SIMPLEX, 0.60, Scalar(50, 50, 250), 1, LINE_AA); // obje için kayıp yazısı
				imshow(winname, frame);
				waitKey(0); // yazı için bekle
				break;
			}

		
			rot_rect.points(points);


			// ekranın sol üstünde H S V değerleri için text çıktısı -- development purpose
			sprintf_s(str, "hue : %d - %d", hue_l, hue_h);
			putText(frame, str, Point(10, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(100, 250, 100), 1, LINE_AA);
			sprintf_s(str, "sat : %d - %d", sat_l, sat_h);
			putText(frame, str, Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(100, 255, 100), 1, LINE_AA);
			sprintf_s(str, "val : %d - %d", val_l, val_h);
			putText(frame, str, Point(10, 45), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(100, 255, 100), 1, LINE_AA);

			for (int i = 0; i < 4; i++) // rect 4 kenar çizilmesi
				line(frame, points[i], points[(i + 1) % 4], 255, 2); // i ve i+1. köşe için line

			imshow(winname, frame); // ekran güncelleme
		}
		int keyboard = waitKey(20); // çıkış için bekleme ekran yavaşsa wait oranını azalt !!!
		if (keyboard == 'q' || keyboard == 27)
			break;
		else if (keyboard == 'p' || keyboard == 112)
			mode = 0;
		else if (keyboard == 'r' || keyboard == 114)
			mode = 1;
	}
}
static void onWindow(int event, int x, int y, int, void*) // mouse tetikleme fonksiyonu
{
	Mat hsv;
	if (SelectObject && !color_select) // obje seçimi açık ve renk seçimi açık değil ise -- ilk olarak ROI seçiliyor
	{
		Selection.x = MIN(x, BasePoint.x);
		Selection.y = MIN(y, BasePoint.y);
		Selection.height = std::abs(y - BasePoint.y);
		Selection.width = std::abs(x - BasePoint.x);
		Selection &= Rect(0, 0, image.cols, image.rows); // prevent boundary exceptions
	}
	else if (color_select) // renk seçimini açtıysak renk işlemi
	{
		BasePoint = Point(x,y);
		cvtColor(image, hsv, COLOR_BGR2HSV);
		color = hsv.at<Vec3b>(Point(x, y));
	}

	switch(event) // mouse eventleri
	{
		case EVENT_LBUTTONDOWN:
			SelectObject = true; // ROI seçimini aç
			BasePoint = Point(x,y);
			if(!color_select)  // renk seçimi açık değilse kareyi güncelle - değilse kare değişmemeli
				Selection = Rect(x,y,0,0); 
			break;
		case EVENT_LBUTTONUP:
			SelectObject = false; // ROI seçim bayrağı kapatıldı
			if (color_select) // color seçimi bittiyse işlem bitmiştir
				ROI_select = false;
			color_select = true; // ROI seçimi bittiyse color seçimine geçelim
			break;
	}
}
// trackbar tetikleme fonksiyonları
static void on_hue_l_thresh_trackbar(int, void*) 
{
	hue_l = min(hue_h - 1, hue_l);
	setTrackbarPos("Low H", window_detection_name, hue_l);
}
static void on_hue_h_thresh_trackbar(int, void*)
{
	hue_h = max(hue_h, hue_l + 1);
	setTrackbarPos("High H", window_detection_name, hue_h);
}
static void on_sat_l_thresh_trackbar(int, void*)
{
	sat_l = min(sat_h - 1, sat_l);
	setTrackbarPos("Low S", window_detection_name, sat_l);
}
static void on_sat_h_thresh_trackbar(int, void*)
{
	sat_h = max(sat_h, sat_l + 1);
	setTrackbarPos("High S", window_detection_name, sat_h);
}
static void on_val_l_thresh_trackbar(int, void*)
{
	val_l = min(val_h - 1, val_l);
	setTrackbarPos("Low V", window_detection_name, val_l);
}
static void on_val_h_thresh_trackbar(int, void*)
{
	val_h = max(val_h, val_l + 1);
	setTrackbarPos("High V", window_detection_name, val_h);
}
void meanDev_weighted(Mat image, Point center, double *mean, double *stddev, InputArray mask = noArray())
{
	Mat src, weights = Mat::ones(image.rows, image.cols, CV_32FC1);
	image.convertTo(src,CV_32FC1);
	for (int i = 0; i < image.rows; i++)
		for (int j = 0; j < image.cols; j++)
			weights.at<float>(i,j) /= cv::norm(Point(i, j) - center); // L2 norm
	float N = sum(weights)[0];
	*mean = (double) sum(weights.mul(src) / N )[0];
	*stddev = sqrt(sum((src - mean[0]).mul(src - mean[0]))[0] / (image.rows*image.cols));
}
