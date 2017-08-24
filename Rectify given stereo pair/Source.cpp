#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(){
	cout << "progress..."<<endl;
	Mat imgl = imread("l.JPG");
	Mat imgr = imread("r.JPG");
	int rows = imgl.rows;
	int cols = imgl.cols;
	vector<Point3f> pl,pr;
	//左圖點位置
	pl.push_back(Point3f(35, 249, 1));
	pl.push_back(Point3f(479, 362, 1));
	pl.push_back(Point3f(593, 697, 1));
	pl.push_back(Point3f(118, 1933, 1));
	pl.push_back(Point3f(687, 858, 1));
	pl.push_back(Point3f(2106, 577, 1));
	pl.push_back(Point3f(1529, 523, 1));
	pl.push_back(Point3f(1976, 1256, 1));
	pl.push_back(Point3f(2377, 1037, 1));
	pl.push_back(Point3f(982, 1706, 1));
	//右圖點位置
	pr.push_back(Point3f(772, 435, 1));
	pr.push_back(Point3f(1158, 475,1));
	pr.push_back(Point3f(871, 793, 1));
	pr.push_back(Point3f(622, 1864, 1));
	pr.push_back(Point3f(1349, 910, 1));
	pr.push_back(Point3f(2887, 457, 1));
	pr.push_back(Point3f(2179, 488, 1));
	pr.push_back(Point3f(2734, 1224, 1));
	pr.push_back(Point3f(1845, 988, 1));
	pr.push_back(Point3f(1430, 1694, 1));

//----------計算T----------------
	float plx_avg = (pl[0].x + pl[1].x + pl[2].x + pl[3].x + pl[4].x + pl[5].x + pl[6].x + pl[7].x + pl[8].x + pl[9].x ) / 10;
	float ply_avg = (pl[0].y + pl[1].y + pl[2].y + pl[3].y + pl[4].y + pl[5].y + pl[6].y + pl[7].y + pl[8].y + pl[9].y ) / 10;
	float prx_avg = (pr[0].x + pr[1].x + pr[2].x + pr[3].x + pr[4].x + pr[5].x + pr[6].x + pr[7].x + pr[8].x + pr[9].x ) / 10;
	float pry_avg = (pr[0].y + pr[1].y + pr[2].y + pr[3].y + pr[4].y + pr[5].y + pr[6].y + pr[7].y + pr[8].y + pr[9].y ) / 10;
	float pl_length[10];
	float pr_length[10];
	float length_avgl = 0;
	float length_avgr = 0;
	
	Point2f templ,tempr;
	for (int i = 0; i < 10; i++){
		templ.x = pl[i].x - plx_avg;
		templ.y = pl[i].y - ply_avg;
		tempr.x = pr[i].x - prx_avg;
		tempr.y = pr[i].y - pry_avg;
		pl_length[i] = norm(templ);
		pr_length[i] = norm(tempr);
		length_avgl += pl_length[i];
		length_avgr += pr_length[i];		
	}
	length_avgl /= 10;
	length_avgr /= 10;
	Mat Tl_l = (Mat_<float>(3, 3) <<
		sqrt(2) / length_avgl, 0, 0,
		0, sqrt(2) / length_avgl, 0,
		0, 0, 1
		);
	Mat Tl_r = (Mat_<float>(3, 3) <<
		1, 0, -1 * plx_avg,
		0, 1, -1*ply_avg,
		0, 0, 1
		);
	Mat Tl = Tl_l*Tl_r;

	Mat Tr_l = (Mat_<float>(3, 3) <<
		sqrt(2) / length_avgr, 0, 0,
		0, sqrt(2) / length_avgr, 0,
		0, 0, 1
		);
	Mat Tr_r = (Mat_<float>(3, 3) <<
		1, 0, -1 * prx_avg,
		0, 1, -1 * pry_avg,
		0, 0, 1
		);
	Mat Tr = Tr_l*Tr_r;

//----------將點normailzed---------------
	vector<Point3f> pl_nor, pr_nor;
	Mat normailzed;
	for (int i = 0; i < 10; i++){
		normailzed = (Mat_<float>(3, 1) << pl[i].x, pl[i].y, pl[i].z);
		normailzed = Tl*normailzed;
		pl_nor.push_back(Point3f(normailzed.at<float>(0, 0), normailzed.at<float>(1, 0), normailzed.at<float>(2, 0)));		
		normailzed = (Mat_<float>(3, 1) << pr[i].x, pr[i].y, pr[i].z);
		normailzed = Tr*normailzed;
		pr_nor.push_back(Point3f(normailzed.at<float>(0, 0), normailzed.at<float>(1, 0), normailzed.at<float>(2, 0)));		
		
	}

//----------算出F矩陣--------------
	Mat A = (Mat_<float>(10, 9) <<
		pr_nor[0].x * pl_nor[0].x, pr_nor[0].x*pl_nor[0].y, pr_nor[0].x, pr_nor[0].y*pl_nor[0].x, pr_nor[0].y*pl_nor[0].y, pr_nor[0].y, pl_nor[0].x, pl_nor[0].y, 1,
		pr_nor[1].x * pl_nor[1].x, pr_nor[1].x*pl_nor[1].y, pr_nor[1].x, pr_nor[1].y*pl_nor[1].x, pr_nor[1].y*pl_nor[1].y, pr_nor[1].y, pl_nor[1].x, pl_nor[1].y, 1,
		pr_nor[2].x * pl_nor[2].x, pr_nor[2].x*pl_nor[2].y, pr_nor[2].x, pr_nor[2].y*pl_nor[2].x, pr_nor[2].y*pl_nor[2].y, pr_nor[2].y, pl_nor[2].x, pl_nor[2].y, 1,
		pr_nor[3].x * pl_nor[3].x, pr_nor[3].x*pl_nor[3].y, pr_nor[3].x, pr_nor[3].y*pl_nor[3].x, pr_nor[3].y*pl_nor[3].y, pr_nor[3].y, pl_nor[3].x, pl_nor[3].y, 1,
		pr_nor[4].x * pl_nor[4].x, pr_nor[4].x*pl_nor[4].y, pr_nor[4].x, pr_nor[4].y*pl_nor[4].x, pr_nor[4].y*pl_nor[4].y, pr_nor[4].y, pl_nor[4].x, pl_nor[4].y, 1,
		pr_nor[5].x * pl_nor[5].x, pr_nor[5].x*pl_nor[5].y, pr_nor[5].x, pr_nor[5].y*pl_nor[5].x, pr_nor[5].y*pl_nor[5].y, pr_nor[5].y, pl_nor[5].x, pl_nor[5].y, 1,
		pr_nor[6].x * pl_nor[6].x, pr_nor[6].x*pl_nor[6].y, pr_nor[6].x, pr_nor[6].y*pl_nor[6].x, pr_nor[6].y*pl_nor[6].y, pr_nor[6].y, pl_nor[6].x, pl_nor[6].y, 1,
		pr_nor[7].x * pl_nor[7].x, pr_nor[7].x*pl_nor[7].y, pr_nor[7].x, pr_nor[7].y*pl_nor[7].x, pr_nor[7].y*pl_nor[7].y, pr_nor[7].y, pl_nor[7].x, pl_nor[7].y, 1,
		pr_nor[8].x * pl_nor[8].x, pr_nor[8].x*pl_nor[8].y, pr_nor[8].x, pr_nor[8].y*pl_nor[8].x, pr_nor[8].y*pl_nor[8].y, pr_nor[8].y, pl_nor[8].x, pl_nor[8].y, 1,
		pr_nor[9].x * pl_nor[9].x, pr_nor[9].x*pl_nor[9].y, pr_nor[9].x, pr_nor[9].y*pl_nor[9].x, pr_nor[9].y*pl_nor[9].y, pr_nor[9].y, pl_nor[9].x, pl_nor[9].y, 1		
		);
	Mat w, u, vt;
	SVD::compute(A, w, u, vt, SVD::FULL_UV);	//SVD分解A
	Mat FH = (Mat_<float>(3, 3) << 
		vt.at<float>(8, 0), vt.at<float>(8, 1), vt.at<float>(8, 2),
		vt.at<float>(8, 3), vt.at<float>(8, 4), vt.at<float>(8, 5),
		vt.at<float>(8, 6), vt.at<float>(8, 7), vt.at<float>(8, 8)
		);
	FH = FH*-1;	

	Mat F = (Tr.t())*(FH*Tl);	//算出F
//-----------------把最大的點刪掉------------------
	Mat x,xp;
	float max = 0,local;
	for (int i = 0; i < 10; i++){
		x = (Mat_<float>(3, 1) << pl[i].x, pl[i].y, pl[i].z);
		xp = (Mat_<float>(3, 1) << pr[i].x, pr[i].y, pr[i].z);
		x = xp.t()*(F* x);
		if (abs(x.at<float>(0, 0))>max){
			max = abs(x.at<float>(0, 0));
			local = i;
		}
	}
	
	pl.erase(pl.begin() + local);
	pr.erase(pr.begin() + local);
	pl_nor.erase(pl_nor.begin() + local);
	pr_nor.erase(pr_nor.begin() + local);
	
//-----------------計算新的F------------------
	Mat A_new=(Mat_<float>(9, 9) <<
		pr_nor[0].x * pl_nor[0].x, pr_nor[0].x*pl_nor[0].y, pr_nor[0].x, pr_nor[0].y*pl_nor[0].x, pr_nor[0].y*pl_nor[0].y, pr_nor[0].y, pl_nor[0].x, pl_nor[0].y, 1,
		pr_nor[1].x * pl_nor[1].x, pr_nor[1].x*pl_nor[1].y, pr_nor[1].x, pr_nor[1].y*pl_nor[1].x, pr_nor[1].y*pl_nor[1].y, pr_nor[1].y, pl_nor[1].x, pl_nor[1].y, 1,
		pr_nor[2].x * pl_nor[2].x, pr_nor[2].x*pl_nor[2].y, pr_nor[2].x, pr_nor[2].y*pl_nor[2].x, pr_nor[2].y*pl_nor[2].y, pr_nor[2].y, pl_nor[2].x, pl_nor[2].y, 1,
		pr_nor[3].x * pl_nor[3].x, pr_nor[3].x*pl_nor[3].y, pr_nor[3].x, pr_nor[3].y*pl_nor[3].x, pr_nor[3].y*pl_nor[3].y, pr_nor[3].y, pl_nor[3].x, pl_nor[3].y, 1,
		pr_nor[4].x * pl_nor[4].x, pr_nor[4].x*pl_nor[4].y, pr_nor[4].x, pr_nor[4].y*pl_nor[4].x, pr_nor[4].y*pl_nor[4].y, pr_nor[4].y, pl_nor[4].x, pl_nor[4].y, 1,
		pr_nor[5].x * pl_nor[5].x, pr_nor[5].x*pl_nor[5].y, pr_nor[5].x, pr_nor[5].y*pl_nor[5].x, pr_nor[5].y*pl_nor[5].y, pr_nor[5].y, pl_nor[5].x, pl_nor[5].y, 1,
		pr_nor[6].x * pl_nor[6].x, pr_nor[6].x*pl_nor[6].y, pr_nor[6].x, pr_nor[6].y*pl_nor[6].x, pr_nor[6].y*pl_nor[6].y, pr_nor[6].y, pl_nor[6].x, pl_nor[6].y, 1,
		pr_nor[7].x * pl_nor[7].x, pr_nor[7].x*pl_nor[7].y, pr_nor[7].x, pr_nor[7].y*pl_nor[7].x, pr_nor[7].y*pl_nor[7].y, pr_nor[7].y, pl_nor[7].x, pl_nor[7].y, 1,
		pr_nor[8].x * pl_nor[8].x, pr_nor[8].x*pl_nor[8].y, pr_nor[8].x, pr_nor[8].y*pl_nor[8].x, pr_nor[8].y*pl_nor[8].y, pr_nor[8].y, pl_nor[8].x, pl_nor[8].y, 1
		
		);
	
	SVD::compute(A_new, w, u, vt, SVD::FULL_UV);
	FH = (Mat_<float>(3, 3) <<
		vt.at<float>(8, 0), vt.at<float>(8, 1), vt.at<float>(8, 2),
		vt.at<float>(8, 3), vt.at<float>(8, 4), vt.at<float>(8, 5),
		vt.at<float>(8, 6), vt.at<float>(8, 7), vt.at<float>(8, 8)
		);
	FH = FH*-1;
	F = (Tr.t())*(FH*Tl);
	
	SVD::compute(F, w, u, vt, SVD::FULL_UV);	
	Mat s = (Mat_<float>(3, 3) <<
		w.at<float>(0, 0), 0, 0,
		0, w.at<float>(1, 0), 0,
		0, 0, 0
		);
	F = u*s*vt;	//新F

//----------------epipole-------------------
	Mat pl1 = (Mat_<float>(3, 1) <<
		pl[0].x, pl[0].y, pl[0].z
		);
	Mat pl2 = (Mat_<float>(3, 1) <<
		pl[1].x, pl[1].y, pl[1].z
		);
	Mat pr1 = (Mat_<float>(3, 1) <<
		pr[0].x, pr[0].y, pr[0].z
		);
	Mat pr2 = (Mat_<float>(3, 1) <<
		pr[1].x, pr[1].y, pr[1].z
		);

	Mat e_l = (F.t()*pr1).cross(F.t()*pr2);	//左邊epipole
	Mat e_r = (F*pl1).cross(F *pl2);	//右邊epipole

	e_l.at<float>(0, 0) /= e_l.at<float>(2, 0);
	e_l.at<float>(1, 0) /= e_l.at<float>(2, 0);
	e_l.at<float>(2, 0) = 1;
	e_r.at<float>(0, 0) /= e_r.at<float>(2, 0);
	e_r.at<float>(1, 0) /= e_r.at<float>(2, 0);
	e_r.at<float>(2, 0) = 1;
//----------------T------------------
	Mat T = (Mat_<float>(3, 3) <<
		1, 0, -1496,
		0, 1, -1000,
		0, 0, 1
		);
	Mat Te_l = T*e_l;
	Mat Te_r = T*e_r;	

//-------------------R-------------------
	float angle_l = -1 * atan(Te_l.at<float>(1, 0) / Te_l.at<float>(0, 0));
	float angle_r = -1 * atan(Te_r.at<float>(1, 0) / Te_r.at<float>(0, 0));
	
	Mat R_l = (Mat_<float>(3, 3) <<
		cos(angle_l), -sin(angle_l), 0,
		sin(angle_l), cos(angle_l), 0,
		0, 0, 1
		);
	Mat R_r = (Mat_<float>(3, 3) <<
		cos(angle_r), -sin(angle_r), 0,
		sin(angle_r), cos(angle_r), 0,
		0, 0, 1
		);
	
	Mat RTe_l = R_l*Te_l;
	Mat RTe_r = R_r*Te_r;	
//----------------------G--------------------
	Mat G_l = (Mat_<float>(3, 3) <<
		1, 0, 0,
		0, 1, 0,
		-1 / RTe_l.at<float>(0, 0), 0, 1
		);
	
	Mat G_r = (Mat_<float>(3, 3) <<
		1, 0, 0,
		0, 1, 0,
		-1 / RTe_r.at<float>(0, 0), 0, 1
		);
//--------------------得到H-------------------
	Mat H_l = G_l*R_l*T;
	Mat H_r = G_r*R_r*T;	
	
//--------------------計算Ha----------------------
	Mat Ha_l;
	Mat Ha_r = (Mat_<float>(5, 1));
	float dis00 = 0, dis01 = 0, dis02 = 0, dis11 = 0, dis12 = 0, disr00 = 0, disr10 = 0, disr20 = 0, disr30 = 0, disr40=0;

	for (int i = 0; i < 9; i++){
		x = (Mat_<float>(3, 1) << pl[i].x, pl[i].y, pl[i].z);
		xp = (Mat_<float>(3, 1) << pr[i].x, pr[i].y, pr[i].z);
		x = H_l*x;		
		x.at<float>(0, 0) /= x.at<float>(2,0);
		x.at<float>(1, 0) /= x.at<float>(2,0);
		xp = H_r*xp;
		xp.at<float>(0, 0) /= xp.at<float>(2, 0);
		xp.at<float>(1, 0) /= xp.at<float>(2, 0);
		dis00 += x.at<float>(0, 0)*x.at<float>(0, 0);
		dis01 += x.at<float>(0, 0)*x.at<float>(1, 0);
		dis02 += x.at<float>(0, 0);
		dis11 += x.at<float>(1, 0)*x.at<float>(1, 0);
		dis12 += x.at<float>(1,0);
		disr00 += x.at<float>(0, 0)*xp.at<float>(0, 0);
		disr10 += x.at<float>(1, 0)*xp.at<float>(0, 0);
		disr20 += xp.at<float>(0, 0);
		disr30 += x.at<float>(1, 0)*xp.at<float>(1, 0);
		disr40 += xp.at<float>(1, 0);
	}	
	
	Ha_l = (Mat_<float>(5, 5) <<
		dis00, dis01, dis02, 0, 0,
		dis01, dis11, dis12, 0, 0,
		dis02, dis12, pl_nor.size(), 0, 0,
		0, 0, 0, dis11, dis12,
		0, 0, 0, dis12, pl_nor.size()
		);
	
	Ha_r = (Mat_<float>(5, 1) << disr00, disr10, disr20, disr30, disr40);	
	Mat abcde = Ha_l.inv()*Ha_r;
	//Ha矩陣
	Mat Ha = (Mat_<float>(3, 3) <<
		abcde.at<float>(0, 0), abcde.at<float>(1, 0), abcde.at<float>(2, 0),
		0, abcde.at<float>(3, 0), abcde.at<float>(4, 0),
		0, 0, 1
		);
	
//------------------Ht---------------------
	Mat Ht = (Mat_<float>(3, 3) <<
		1, 0, 1496,
		0, 1, 1000,
		0, 0, 1
		);
	
//------------------------------------Homograhy-----------------------------------------	

	Mat Hl = Ht*Ha*H_l;
	Mat Hr = Ht*H_r;
	Mat img(rows,cols,CV_8UC3);
	Mat imgg(rows, cols, CV_8UC3);

	warpPerspective(imgl, img, Hl, img.size(), cv::INTER_LINEAR);
	warpPerspective(imgr, imgg, Hr, img.size(), cv::INTER_LINEAR);

	imwrite("l_new.jpg", img);
	imwrite("r_new.jpg", imgg);
	cout << "complete."<<endl;
	system("pause");
}

