#include <opencv2/opencv.hpp>
#include <fstream>

using namespace cv;
using namespace std;

int pnum = 364;
Mat *PicL = new Mat[pnum];
Mat *PicR = new Mat[pnum];

void readimg(){	
	stringstream Picnamel, Picnamer;

	for (int i = 0; i < pnum; i++) {
		Picnamel.str("");
		Picnamel.clear();
		Picnamer.str("");
		Picnamer.clear();
		if (i < 10){
			Picnamel << "L\\" << "000" << i << ".jpg";
			Picnamer << "R\\" << "000" << i << ".jpg";
		}
		if (i >= 10 && i < 100){
			Picnamel << "L\\" << "00" << i << ".jpg";
			Picnamer << "R\\" << "00" << i << ".jpg";
		}
		if (i >= 100){
			Picnamel << "L\\" << "0" << i << ".jpg";
			Picnamer << "R\\" << "0" << i << ".jpg";
		}
		PicL[i] = imread(Picnamel.str(), 0);
		PicR[i] = imread(Picnamer.str(), 0);
	}	
}

int main(){		
	fstream outf;
	outf.open("output.xyz", ios::out);
	
	readimg();//讀圖

	int rows = PicL[0].rows, cols = PicL[0].cols;

	Mat L_Camera_K = (Mat_<double>(3, 3) <<
		1031.107181256652, 0.000000000000, 300.315754576448,
		0.000000000000, 1031.922726531495, 596.128631557107,
		0.000000000000, 0.000000000000, 1.000000000000
		);
	
	Mat L_Camera_RT = (Mat_<double>(3, 4) <<
		1.000000000000, 0.000000000000, 0.000000000000, 0.000000000000,
		0.000000000000, 1.000000000000, 0.000000000000, 0.000000000000,
		0.000000000000, 0.000000000000, 1.000000000000, 0.000000000000
		);
	
	Mat R_Camera_K = (Mat_<double>(3, 3) <<
		1034.184348727144, 0.000000000000, 417.455360200033,
		0.000000000000, 1034.889311554467, 615.052068517334,
		0.000000000000, 0.000000000000, 1.000000000000
		);

	Mat R_Camera_RT = (Mat_<double>(3, 4) <<
		0.960420625577, 0.010140226529, 0.278369175326, - 70.168656978049,
		- 0.009291786823, 0.999947294142, - 0.004367108410, - 0.002608446641,
		- 0.278398787108, 0.001607713956, 0.960464226607, 13.498989529018
		);
	Mat F = (Mat_<double>(3, 3) <<
		-0.000000050362, 0.000002369273, - 0.001301863689,
		0.000000811157, - 0.000000004467, - 0.011984436200,
		- 0.000689813339, 0.010646819054, 1.000000000000
		);

	Mat PL = L_Camera_K*L_Camera_RT;	//左圖P
	Mat PR = R_Camera_K*R_Camera_RT;	//右圖P
	
	Mat p1, p2, p3, pp1, pp2, pp3;
	p1 = (Mat_<double>(4, 1) <<
		PL.at<double>(0, 0),PL.at<double>(0, 1),PL.at<double>(0, 2),PL.at<double>(0, 3) );
	p2 = (Mat_<double>(4, 1) <<
		PL.at<double>(1, 0),PL.at<double>(1, 1),PL.at<double>(1, 2),PL.at<double>(1, 3) );
	p3 = (Mat_<double>(4, 1) <<
		PL.at<double>(2, 0),PL.at<double>(2, 1),PL.at<double>(2, 2),PL.at<double>(2, 3)	);

	pp1 = (Mat_<double>(4, 1) <<
		PR.at<double>(0, 0),PR.at<double>(0, 1),PR.at<double>(0, 2),PR.at<double>(0, 3)	);
	pp2 = (Mat_<double>(4, 1) <<
		PR.at<double>(1, 0),PR.at<double>(1, 1),PR.at<double>(1, 2),PR.at<double>(1, 3)	);
	pp3 = (Mat_<double>(4, 1) <<
		PR.at<double>(2, 0),PR.at<double>(2, 1),PR.at<double>(2, 2),PR.at<double>(2, 3)	);
	
	int temp,temp1,maxw=-1,ymax=0,ymin=0;
	double temp2;
	Mat lpoint,rpointc,t,rpoint; 
	Mat a, b, c, d, A, U, S, vt, V, X;	

	for (int i = 0; i < pnum; i++){                             //第i張圖
		cout << "picture" << i<<endl;
		for (int h = 0; h < rows; h++){							//第h列
			temp = -1, maxw = -1;
			for (int w = 0; w < cols; w++){						//第w行
				if (PicL[i].at<uchar>(h, w)>temp && PicL[i].at<uchar>(h, w)>170){	//是否超過目前最小值&亮度大於160				
					temp = PicL[i].at<uchar>(h, w);					
					maxw = w;					
				}
			}

			if (maxw != -1){				//有找到左圖最亮的點再做
				lpoint = (Mat_<double>(3, 1) << maxw, h, 1);		//左圖的點		
				temp2 = 1;
				t = F*lpoint;	//epipolar line
				temp=-t.at<double>(2, 0) / t.at<double>(1, 0);
				temp1=-(t.at<double>(0, 0)*cols + t.at<double>(2, 0)) / t.at<double>(1, 0);
				ymax = max(temp, temp1) < rows ? max(temp, temp1) : rows;	//判斷線與邊點交點位置是否超越圖片大小
				ymin = min(temp, temp1) < 0 ? 0 : min(temp, temp1);
				
				for (int hh = ymin; hh < ymax; hh++)	//尋找右圖對應的點
				for (int ww = 0; ww < cols; ww++){
					if (PicR[i].at<uchar>(hh, ww)>170){		//亮度超過再做
						rpointc = (Mat_<double>(3, 1) << ww, hh, 1);	//當前的點
						t = rpointc.t()*F*lpoint;	//計算點與線的距離				
						if (abs(t.at<double>(0, 0)) < temp2 && abs(t.at<double>(0, 0)) < 0.01){		//小於目前最短距離&小於0.01
							temp2 = abs(t.at<double>(0, 0));
							rpoint = (Mat_<double>(3, 1) << ww, hh, 1);		//最終右圖的點
						}
					}
				}								
			}
			
			if (maxw != -1 && temp2 != 1){		//左圖點及右圖點都有再做
				int u = lpoint.at<double>(0, 0), v = lpoint.at<double>(1, 0);	
				int	up = rpoint.at<double>(0, 0), vp = rpoint.at<double>(1, 0);			

				a = u*p3.t() - p1.t();
				b = v*p3.t() - p2.t();
				c = up*pp3.t() - pp1.t();
				d = vp*pp3.t() - pp2.t();

				A = (Mat_<double>(4, 4) <<
					a.at<double>(0, 0), a.at<double>(0, 1), a.at<double>(0, 2), a.at<double>(0, 3),
					b.at<double>(0, 0), b.at<double>(0, 1), b.at<double>(0, 2), b.at<double>(0, 3),
					c.at<double>(0, 0), c.at<double>(0, 1), c.at<double>(0, 2), c.at<double>(0, 3),
					d.at<double>(0, 0), d.at<double>(0, 1), d.at<double>(0, 2), d.at<double>(0, 3)
					);
				
				SVD::compute(A, U, S, vt, SVD::FULL_UV);	//SVD分解A	
				V = vt.t();
				X = (Mat_<double>(4, 1) <<
					V.at<double>(0, 3) / V.at<double>(3, 3),V.at<double>(1, 3) / V.at<double>(3, 3),V.at<double>(2, 3) / V.at<double>(3, 3),1 );
				A = A*X;
				temp2 = sqrt(pow(A.at<double>(0, 0), 2) + pow(A.at<double>(1, 0), 2) + pow(A.at<double>(2, 0), 2) + pow(A.at<double>(3, 0), 2));				
				if (temp2<80)	//removing possible outlier
				outf << X.at<double>(0, 0) << " " << X.at<double>(1, 0) << " " << X.at<double>(2, 0) << endl;	//將點寫入XYZ檔
			}
		}
	}	
	cout << "done." << endl;
	system("pause");
}