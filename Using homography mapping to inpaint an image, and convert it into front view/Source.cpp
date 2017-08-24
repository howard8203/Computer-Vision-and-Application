#include <stdio.h>
#include <math.h>
#include <sstream>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <stdlib.h>

using namespace std;
using namespace cv;


int main(){
	Mat image1 = imread("1.JPG");
	Mat image2 = imread("2.JPG");
	Mat hw;
	Mat point;
	int Case ;
	cout << "�п�J�����檺�B�J" << endl << "1.�N��1�������2����"  << endl << "2.��ɾB���ϰ�(�����Ѥ�ʴyø��ê����:Homo_1Cut.PNG)" << endl << "3.�N�Ϥ��ॿ" << endl;
	cin >> Case ;   //��J����B�J

	
	//----------------------------------------------------------------------------------------------------------------------
	if (Case == 1){            //1.�N��1�������2����
		cout << "please wait...";
		Mat Homo_1(image1.rows, image1.cols, CV_8UC3);

		Point po1(410, 202);    //�Ϥ@����I
		Point po2(434, 814);
		Point po3(1482, 1008);
		Point po4(2027, 352);
		Point pt1(480, 310);   //�ϤG����I
		Point pt2(490, 945);
		Point pt3(1462, 1198);
		Point pt4(1884, 608);

		Mat one_point = (Mat_<float>(8, 8) <<    
			po1.x, po1.y, 1, 0, 0, 0, -pt1.x*po1.x, -pt1.x*po1.y,
			0, 0, 0, po1.x, po1.y, 1, -pt1.y*po1.x, -pt1.y*po1.y,
			po2.x, po2.y, 1, 0, 0, 0, -pt2.x*po2.x, -pt2.x*po2.y,
			0, 0, 0, po2.x, po2.y, 1, -pt2.y*po2.x, -pt2.y*po2.y,
			po3.x, po3.y, 1, 0, 0, 0, -pt3.x*po3.x, -pt3.x*po3.y,
			0, 0, 0, po3.x, po3.y, 1, -pt3.y*po3.x, -pt3.y*po3.y,
			po4.x, po4.y, 1, 0, 0, 0, -pt4.x*po4.x, -pt4.x*po4.y,
			0, 0, 0, po4.x, po4.y, 1, -pt4.y*po4.x, -pt4.y*po4.y
			);

		Mat two_point = (Mat_<float>(8, 1) <<
			pt1.x, pt1.y, pt2.x, pt2.y, pt3.x, pt3.y, pt4.x, pt4.y
			);

		Mat H = one_point.inv()*two_point;   //�ۭ���XH

		Mat Homegraphy_matrix = (Mat_<float>(3, 3) <<   //�NH�g��3*3�x�} �̫�@����1
			H.at<float>(0, 0), H.at<float>(1, 0), H.at<float>(2, 0),
			H.at<float>(3, 0), H.at<float>(4, 0), H.at<float>(5, 0),
			H.at<float>(6, 0), H.at<float>(7, 0), 1
			);

		Mat Hi = Homegraphy_matrix.inv();  //�f�x�}

		for (int h = 0; h < image2.rows; h++)
		for (int w = 0; w < image2.cols; w++){
			hw = (Mat_<float>(3, 1) << w, h, 1);  //�N�I�g���x�}��1
			point = Hi*hw;  //�ۭ���X�������I
			//�ˬd���L�W�X���
			if (int(point.at<float>(1, 0) / point.at<float>(2, 0))<image2.rows && int(point.at<float>(1, 0) / point.at<float>(2, 0)) > 0 && int(point.at<float>(0, 0) / point.at<float>(2, 0))< image2.cols && int(point.at<float>(0, 0) / point.at<float>(2, 0)) > 0){				
				Homo_1.at<Vec3b>(h, w)[0] = image1.at<Vec3b>(int(point.at<float>(1, 0) / point.at<float>(2, 0)), int(point.at<float>(0, 0) / point.at<float>(2, 0)))[0];
				Homo_1.at<Vec3b>(h, w)[1] = image1.at<Vec3b>(int(point.at<float>(1, 0) / point.at<float>(2, 0)), int(point.at<float>(0, 0) / point.at<float>(2, 0)))[1];
				Homo_1.at<Vec3b>(h, w)[2] = image1.at<Vec3b>(int(point.at<float>(1, 0) / point.at<float>(2, 0)), int(point.at<float>(0, 0) / point.at<float>(2, 0)))[2];
			}
		}
		imwrite("Homo_1.jpg", Homo_1);
	}
	//-------------------------------------------------------------------------------------------------------------------------
	if (Case == 2){  //2.��ɾB���ϰ�
		Mat Homo_1Cut = imread("Homo_1Cut.PNG");  //Ū�J�w�ХܾB�������Ϥ�
		if (!Homo_1Cut.data){
			cout << "image not found"<<endl;
			system("pause");
			return 0;
		}
		
		Mat Homo_1md = Homo_1Cut.clone();

		for (int h = 0; h < Homo_1Cut.rows; h++)
		for (int w = 0; w < Homo_1Cut.cols; w++){
			if (Homo_1Cut.at<Vec3b>(h, w)[0] == 255 && Homo_1Cut.at<Vec3b>(h, w)[1] == 255 && Homo_1Cut.at<Vec3b>(h, w)[2] == 255){ //�O�զ⪺pixel�h����
				Homo_1md.at<Vec3b>(h, w)[0] = image2.at<Vec3b>(h, w)[0];
				Homo_1md.at<Vec3b>(h, w)[1] = image2.at<Vec3b>(h, w)[1];
				Homo_1md.at<Vec3b>(h, w)[2] = image2.at<Vec3b>(h, w)[2];
			}
		}
		imwrite("Homo_1md.jpg", Homo_1md);
	}
	//--------------------------------------------------------------------------------------------------------------------------
	if (Case == 3){  //3.�N�Ϥ��ॿ
		cout << "please wait...";
		Mat img_final(600, 800, CV_8UC3);
		Mat Homo_1md = imread("Homo_1md.jpg");

		Point po11(503, 317);
		Point po22(476, 1754);
		Point po33(1888, 1672);
		Point po44(1845, 600);
		Point pt11(0, 0);
		Point pt22(0, 600);
		Point pt33(800, 600);
		Point pt44(800, 0);

		Mat one_pointt = (Mat_<float>(8, 8) <<
			po11.x, po11.y, 1, 0, 0, 0, -pt11.x*po11.x, -pt11.x*po11.y,
			0, 0, 0, po11.x, po11.y, 1, -pt11.y*po11.x, -pt11.y*po11.y,
			po22.x, po22.y, 1, 0, 0, 0, -pt22.x*po22.x, -pt22.x*po22.y,
			0, 0, 0, po22.x, po22.y, 1, -pt22.y*po22.x, -pt22.y*po22.y,
			po33.x, po33.y, 1, 0, 0, 0, -pt33.x*po33.x, -pt33.x*po33.y,
			0, 0, 0, po33.x, po33.y, 1, -pt33.y*po33.x, -pt33.y*po33.y,
			po44.x, po44.y, 1, 0, 0, 0, -pt44.x*po44.x, -pt44.x*po44.y,
			0, 0, 0, po44.x, po44.y, 1, -pt44.y*po44.x, -pt44.y*po44.y
			);

		Mat two_pointt = (Mat_<float>(8, 1) <<
			pt11.x, pt11.y, pt22.x, pt22.y, pt33.x, pt33.y, pt44.x, pt44.y
			);

		Mat HH = one_pointt.inv()*two_pointt;

		Mat Homegraphy_matrixx = (Mat_<float>(3, 3) <<
			HH.at<float>(0, 0), HH.at<float>(1, 0), HH.at<float>(2, 0),
			HH.at<float>(3, 0), HH.at<float>(4, 0), HH.at<float>(5, 0),
			HH.at<float>(6, 0), HH.at<float>(7, 0), 1
			);


		Mat Hi = Homegraphy_matrixx.inv();

		for (int h = 0; h < img_final.rows; h++)
		for (int w = 0; w < img_final.cols; w++){

			hw = (Mat_<float>(3, 1) << w, h, 1);
			point = Hi*hw;
			if (int(point.at<float>(1, 0) / point.at<float>(2, 0))<Homo_1md.rows && int(point.at<float>(1, 0) / point.at<float>(2, 0)) > 0 && int(point.at<float>(0, 0) / point.at<float>(2, 0))< Homo_1md.cols && int(point.at<float>(0, 0) / point.at<float>(2, 0)) > 0){
				img_final.at<Vec3b>(h, w)[0] = Homo_1md.at<Vec3b>(int(point.at<float>(1, 0) / point.at<float>(2, 0)), int(point.at<float>(0, 0) / point.at<float>(2, 0)))[0];
				img_final.at<Vec3b>(h, w)[1] = Homo_1md.at<Vec3b>(int(point.at<float>(1, 0) / point.at<float>(2, 0)), int(point.at<float>(0, 0) / point.at<float>(2, 0)))[1];
				img_final.at<Vec3b>(h, w)[2] = Homo_1md.at<Vec3b>(int(point.at<float>(1, 0) / point.at<float>(2, 0)), int(point.at<float>(0, 0) / point.at<float>(2, 0)))[2];
			}
		}

		imwrite("M10515067.jpg", img_final);
	}
}
