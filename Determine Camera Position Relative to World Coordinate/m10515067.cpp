#include <stdio.h>
#include <math.h>
#include <sstream>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include<iomanip>

using namespace std;
using namespace cv;

int main(){
	Mat square = (Mat_<double>(4, 2) <<
		0,0,
		0,50,
		50,50,
		50,0
		);

	//キ畒夹
	Mat a = (Mat_<double>(4, 2) <<
		401,289,
		205,382,
		127,60,
		401,57
		);
	Mat b = (Mat_<double>(4, 2) <<
		401, 289,
		401, 57,
		686, 60,
		605, 375
		);
	Mat c = (Mat_<double>(4, 2) <<
		401, 289,
		605, 375,
		422, 577,
		205, 382
		);
	
	//Homography
	Mat ha = findHomography(square, a);
	Mat hb = findHomography(square, b);
	Mat hc = findHomography(square, c);
	
	//hap
	Mat hap = ha.t();
	Mat hbp = hb.t();
	Mat hcp = hc.t();
	//璸衡A痻皚
	Mat A = (Mat_<double>(6, 6) <<
		hap.at<double>(0, 0)*hap.at<double>(1, 0), hap.at<double>(0, 0)*hap.at<double>(1, 1) + hap.at<double>(0, 1)*hap.at<double>(1, 0), hap.at<double>(0, 0)*hap.at<double>(1, 2) + hap.at<double>(0, 2)*hap.at<double>(1, 0), hap.at<double>(0, 1)*hap.at<double>(1, 1), hap.at<double>(0, 1)*hap.at<double>(1, 2) + hap.at<double>(0, 2)*hap.at<double>(1, 1), hap.at<double>(0, 2)*hap.at<double>(1, 2),
		hap.at<double>(0, 0)*hap.at<double>(0, 0) - hap.at<double>(1, 0)*hap.at<double>(1, 0), 2 * (hap.at<double>(0, 0)*hap.at<double>(0, 1) - hap.at<double>(1, 0)*hap.at<double>(1, 1)), 2 * (hap.at<double>(0, 0)*hap.at<double>(0, 2) - hap.at<double>(1, 0)*hap.at<double>(1, 2)), hap.at<double>(0, 1)*hap.at<double>(0, 1) - hap.at<double>(1, 1)*hap.at<double>(1, 1), 2 * (hap.at<double>(0, 1)*hap.at<double>(0, 2) - hap.at<double>(1, 1)*hap.at<double>(1, 2)), hap.at<double>(0, 2)*hap.at<double>(0, 2) - hap.at<double>(1, 2)*hap.at<double>(1, 2),
		hbp.at<double>(0, 0)*hbp.at<double>(1, 0), hbp.at<double>(0, 0)*hbp.at<double>(1, 1) + hbp.at<double>(0, 1)*hbp.at<double>(1, 0), hbp.at<double>(0, 0)*hbp.at<double>(1, 2) + hbp.at<double>(0, 2)*hbp.at<double>(1, 0), hbp.at<double>(0, 1)*hbp.at<double>(1, 1), hbp.at<double>(0, 1)*hbp.at<double>(1, 2) + hbp.at<double>(0, 2)*hbp.at<double>(1, 1), hbp.at<double>(0, 2)*hbp.at<double>(1, 2),
		hbp.at<double>(0, 0)*hbp.at<double>(0, 0) - hbp.at<double>(1, 0)*hbp.at<double>(1, 0), 2 * (hbp.at<double>(0, 0)*hbp.at<double>(0, 1) - hbp.at<double>(1, 0)*hbp.at<double>(1, 1)), 2 * (hbp.at<double>(0, 0)*hbp.at<double>(0, 2) - hbp.at<double>(1, 0)*hbp.at<double>(1, 2)), hbp.at<double>(0, 1)*hbp.at<double>(0, 1) - hbp.at<double>(1, 1)*hbp.at<double>(1, 1), 2 * (hbp.at<double>(0, 1)*hbp.at<double>(0, 2) - hbp.at<double>(1, 1)*hbp.at<double>(1, 2)), hbp.at<double>(0, 2)*hbp.at<double>(0, 2) - hbp.at<double>(1, 2)*hbp.at<double>(1, 2),
		hcp.at<double>(0, 0)*hcp.at<double>(1, 0), hcp.at<double>(0, 0)*hcp.at<double>(1, 1) + hcp.at<double>(0, 1)*hcp.at<double>(1, 0), hcp.at<double>(0, 0)*hcp.at<double>(1, 2) + hcp.at<double>(0, 2)*hcp.at<double>(1, 0), hcp.at<double>(0, 1)*hcp.at<double>(1, 1), hcp.at<double>(0, 1)*hcp.at<double>(1, 2) + hcp.at<double>(0, 2)*hcp.at<double>(1, 1), hcp.at<double>(0, 2)*hcp.at<double>(1, 2),
		hcp.at<double>(0, 0)*hcp.at<double>(0, 0) - hcp.at<double>(1, 0)*hcp.at<double>(1, 0), 2 * (hcp.at<double>(0, 0)*hcp.at<double>(0, 1) - hcp.at<double>(1, 0)*hcp.at<double>(1, 1)), 2 * (hcp.at<double>(0, 0)*hcp.at<double>(0, 2) - hcp.at<double>(1, 0)*hcp.at<double>(1, 2)), hcp.at<double>(0, 1)*hcp.at<double>(0, 1) - hcp.at<double>(1, 1)*hcp.at<double>(1, 1), 2 * (hcp.at<double>(0, 1)*hcp.at<double>(0, 2) - hcp.at<double>(1, 1)*hcp.at<double>(1, 2)), hcp.at<double>(0, 2)*hcp.at<double>(0, 2) - hcp.at<double>(1, 2)*hcp.at<double>(1, 2)
		);
	
	Mat u, s, vt;
	SVD::compute(A, u, s, vt, SVD::FULL_UV);	//SVDだ秆A
	vt = vt.t();
	
	//璸衡W痻皚
	Mat w = (Mat_<double>(3, 3) <<
		vt.at<double>(0, 5), vt.at<double>(1, 5), vt.at<double>(2, 5),
		vt.at<double>(1, 5), vt.at<double>(3, 5), vt.at<double>(4, 5),
		vt.at<double>(2, 5), vt.at<double>(4, 5), vt.at<double>(5, 5)
		);	

	Mat inw = w.inv();
	inw = inw/ inw.at<double>(2, 2);	
	
	double cc = inw.at<double>(0, 2);	
	double ee = inw.at<double>(1, 2);
	double dd = sqrt(inw.at<double>(1, 1)-ee*ee);	
	double bb = (inw.at<double>(0,1)-cc*ee)/dd;
	double aa = sqrt(inw.at<double>(0, 0)-bb*bb-cc*cc);

	//衡K
	Mat k = (Mat_<double>(3, 3) << 
		aa,bb,cc,
		0,dd,ee,
		0,0,1		
		);	
	cout <<"ず场把计:"<<endl<< k<<endl;

	Mat ra = k.inv()*ha;
	Mat rb = k.inv()*hb;
	Mat rc = k.inv()*hc;	

	double r1_length = sqrt(ra.at<double>(0, 0)*ra.at<double>(0, 0) + ra.at<double>(1, 0)*ra.at<double>(1, 0) + ra.at<double>(2, 0)*ra.at<double>(2, 0));
	double r2_length = sqrt(ra.at<double>(0, 1)*ra.at<double>(0, 1) + ra.at<double>(1, 1)*ra.at<double>(1, 1) + ra.at<double>(2, 1)*ra.at<double>(2, 1));
		
	Mat r1 = (Mat_<double>(3, 1) <<
		ra.at<double>(0, 0) / r1_length,
		ra.at<double>(1, 0) / r1_length,
		ra.at<double>(2, 0) / r1_length
		);
	Mat r2 = (Mat_<double>(3, 1) <<
		ra.at<double>(0, 1) / r2_length,
		ra.at<double>(1, 1) / r2_length,
		ra.at<double>(2, 1) / r2_length
		);
	Mat r3 = r1.cross(r2);	
	r2 = r3.cross(r1);
	Mat h3 = (Mat_<double>(3, 1) <<
		ha.at<double>(0, 2),
		ha.at<double>(1, 2),
		ha.at<double>(2, 2)
		);
	Mat t = k.inv()*h3 / r1_length;
	//art
	Mat a_rt = (Mat_<double>(3, 4) <<
		r1.at<double>(0, 0), r2.at<double>(0, 0), r3.at<double>(0, 0), t.at<double>(0, 0),
		r1.at<double>(1, 0), r2.at<double>(1, 0), r3.at<double>(1, 0), t.at<double>(1, 0),
		r1.at<double>(2, 0), r2.at<double>(2, 0), r3.at<double>(2, 0), t.at<double>(2, 0)
		);	

	r1_length = sqrt(rb.at<double>(0, 0)*rb.at<double>(0, 0) + rb.at<double>(1, 0)*rb.at<double>(1, 0) + rb.at<double>(2, 0)*rb.at<double>(2, 0));
	r2_length = sqrt(rb.at<double>(0, 1)*rb.at<double>(0, 1) + rb.at<double>(1, 1)*rb.at<double>(1, 1) + rb.at<double>(2, 1)*rb.at<double>(2, 1));
	
	r1 = (Mat_<double>(3, 1) <<
		rb.at<double>(0, 0) / r1_length,
		rb.at<double>(1, 0) / r1_length,
		rb.at<double>(2, 0) / r1_length
		);
	r2 = (Mat_<double>(3, 1) <<
		rb.at<double>(0, 1) / r2_length,
		rb.at<double>(1, 1) / r2_length,
		rb.at<double>(2, 1) / r2_length
		);
	r3 = r1.cross(r2);
	r2 = r3.cross(r1);
	h3 = (Mat_<double>(3, 1) <<
		hb.at<double>(0, 2),
		hb.at<double>(1, 2),
		hb.at<double>(2, 2)
		);
	t = k.inv()*h3 / r1_length;
	//brt
	Mat b_rt = (Mat_<double>(3, 4) <<
		r1.at<double>(0, 0), r2.at<double>(0, 0), r3.at<double>(0, 0), t.at<double>(0, 0),
		r1.at<double>(1, 0), r2.at<double>(1, 0), r3.at<double>(1, 0), t.at<double>(1, 0),
		r1.at<double>(2, 0), r2.at<double>(2, 0), r3.at<double>(2, 0), t.at<double>(2, 0)
		);

	r1_length = sqrt(rc.at<double>(0, 0)*rc.at<double>(0, 0) + rc.at<double>(1, 0)*rc.at<double>(1, 0) + rc.at<double>(2, 0)*rc.at<double>(2, 0));
	r2_length = sqrt(rc.at<double>(0, 1)*rc.at<double>(0, 1) + rc.at<double>(1, 1)*rc.at<double>(1, 1) + rc.at<double>(2, 1)*rc.at<double>(2, 1));	

	r1 = (Mat_<double>(3, 1) <<
		rc.at<double>(0, 0) / r1_length,
		rc.at<double>(1, 0) / r1_length,
		rc.at<double>(2, 0) / r1_length
		);
	r2 = (Mat_<double>(3, 1) <<
		rc.at<double>(0, 1) / r2_length,
		rc.at<double>(1, 1) / r2_length,
		rc.at<double>(2, 1) / r2_length
		);
	r3 = r1.cross(r2);
	r2 = r3.cross(r1);
	h3 = (Mat_<double>(3, 1) <<
		hc.at<double>(0, 2),
		hc.at<double>(1, 2),
		hc.at<double>(2, 2)
		);
	t = k.inv()*h3 / r1_length;
	//crt
	Mat c_rt = (Mat_<double>(3, 4) <<
		r1.at<double>(0, 0), r2.at<double>(0, 0), r3.at<double>(0, 0), t.at<double>(0, 0),
		r1.at<double>(1, 0), r2.at<double>(1, 0), r3.at<double>(1, 0), t.at<double>(1, 0),
		r1.at<double>(2, 0), r2.at<double>(2, 0), r3.at<double>(2, 0), t.at<double>(2, 0)
		);
	//衡X
	Mat X = (Mat_<float>(4, 4) <<
		c_rt.at<double>(0, 0), c_rt.at<double>(0, 1), c_rt.at<double>(0, 2), c_rt.at<double>(0, 3),
		c_rt.at<double>(1, 0), c_rt.at<double>(1, 1), c_rt.at<double>(1, 2), c_rt.at<double>(1, 3),
		c_rt.at<double>(2, 0), c_rt.at<double>(2, 1), c_rt.at<double>(2, 2), c_rt.at<double>(2, 3),
		0, 0, 0, 1
		);
	X = X.inv();
	cout << "场把计:" << endl << c_rt << endl;
	cout << "Camera Position:" << "(" << X.at<float>(0, 3)<<"," << X.at<float>(1, 3)<<"," << X.at<float>(2, 3)<<")" << endl;
	system("pause");
}
