#include "header.h"

#define EXAMPLAR_NUM 1200
#define EXAMPLAR_DIM 2

int main(int argc, char *argv[])
{
	srand((unsigned)time(NULL));
	clock_t start=clock();
	 ExamplarSet exm_set;
//	exm_set.create(EXAMPLAR_NUM);
	exm_set.readData("310130.tlopt");
	clock_t end=clock();

	cout<< "read data use " <<(float)(end-start)/ (float)CLOCKS_PER_SEC *1000 << "ms" <<endl;
	//for(int i=0;i<EXAMPLAR_NUM;i++)
	//{
	//	 cout << '(' <<exm_set[i][0] << ','
	//		 << exm_set[i][1] << ','<< (exm_set.getData())[i].output <<	endl
	//		<< (exm_set.getData())[i].getString() <<  ')' << endl;
	//}
	start=clock();
	 KDTree kdtree;
	kdtree.create(exm_set);
	end=clock();

	cout<< "Create a tree use " <<(float)(end-start)/ (float)CLOCKS_PER_SEC<< "s"<<endl;

	Point exm;
	start=clock();
	TopkSet ts;
	int k=10;
	int num = 1000;

	float *distance = new float[k];
	float *value = new float[k];
	string *strtime = new string[k];

	for(int i=0;i<num;i++){
		exm[0]=((float)rand()/(float)RAND_MAX)*50.0f+50.0f;
		exm[1]=((float)rand()/(float)RAND_MAX)*150.0f+600.0f;
		kdtree.findKNN(k, exm, ts, Mahalanobis(0.0990f, -0.0011f, 0.0008f) );
		kdtree.interpret(ts,distance,value,strtime);
		ts.clear();
	}

	end=clock();
	cout<< "KNN use " << (float)((end-start)/ (float)CLOCKS_PER_SEC) / (float)num *1000 << "ms" << endl;

	cout << "Query: x= " <<exm[0]<<","<<"y= "<<exm[1]<<endl;
	
    int id;
//	ExamplarSet ee ;
//		ee = kdtree.getExamplar();
	for(int i=0;i<k;i++)
	{
		id = ts.getData()[i].first;
		
		cout<< "Point is " << kdtree.getExamplar()[id][0] << "," << kdtree.getExamplar()[id][1]<<endl;
		cout<< "distance = " <<distance[i] << ',' << "output = "<< value[i] << endl;
		cout<< strtime[i] <<endl;
	}

	delete []distance;
	delete []value;
	delete []strtime;
//	ee.destroy();
	ts.destroy();
	exm_set.destroy();
	kdtree.destroy();
//	ee.destroy();
//	system("PAUSE"); */
	return 0;
}

