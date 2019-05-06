#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>
#include<unistd.h>
#include"util.h"
#include <omp.h>


int asgn2_pthread(Point * points, Point ** pPermissiblePoints, int number, int dim, int thread_number){

	int permissiblePointNum = 0;
	Point * permissiblePoints = malloc(sizeof(Point)*number);
	int recorder[number];
	int pCounter = 0;
	int t = 0;

	//int i = 0, j = 0, k = 0, h = 0;
	// 	int recorder[number];
	// 	int counter = 0;

	// 	int countbig = 0;
	// 	int countsmall = 0;
	// 	int equal = 0;

	omp_set_num_threads(4);
	for(t = 0; t < number; t++){
		recorder[t] = 0;
	}//0 -> no use, 1 -> permissible points

	for(int i = 0; i < number; i++){
		int tid = omp_get_thread_num();
		int h = 0, j = 0, k = 0;
		int equal = 0, counter = 0;
		int pEqual = 0; //pCounter = 0;

		for(k = 0; k < number; k++){
			if(i == k){
				continue;
			}
			if(recorder[i] == 1 || recorder[k] == 1){
				continue;
			}

			for(j = 0; j < dim; j++){
				if(points[i].values[j] < points[k].values[j]){
					counter++;
					pEqual++;
				}
				if(points[i].values[j] == points[k].values[j]){
					equal++;
					pEqual++;
				}
				//if(points[i].values[j] > points[k].values[j]){ //i > k
				// 	countbig++;
				// 	if(countbig == dim){
				// 		recorder[i] = 0; //0
				// 		recorder[k] = 1; //1 
				// 		countbig = 0;
				// 	}
				//}
			}
			if(equal != dim && counter == 0){
				recorder[i] = 1;
				recorder[k] = 0;
				pCounter++;
				// 	if(equal == dim){
				// 		recorder[i] = 1;//1
				// 		recorder[k] = 1;
				// 		equal = 0;
				// 	}
			}
			else if (pEqual == dim && counter != 0){
				recorder[i] = 0;
				recorder[k] = 1;
				pCounter++;
			}
			equal = 0;
			pEqual = 0;
			counter = 0;
		}
	}		
		// 	else if(countsmall > 0 && countbig > 0){
		// 		countsmall = 0;
		// 		countbig = 0;
		// 		break;
		// 	}
		// 	else if(equal + countsmall == dim){
		// 		recorder[i] = 0; //1
		// 		recorder[k] = 1; //0
		// 		countsmall = 0;
		// 		equal = 0;
		// 	}
		// 	else if(equal + countbig == dim){
		// 		recorder[k] = 0; //1
		// 		recorder[i] = 1; //0
		// 		countbig = 0;
		// 		equal = 0;
		// 		}
		// 	}
	

	for(t = 0; t < number; t++){
		//printf("%d ", recorder[t]);
		if(recorder[t] == 0){
			permissiblePoints[permissiblePointNum] = points[t];
			permissiblePointNum++;
		}
	}
	printf("\n");

	// for(i = 0; i < number; i++){
	// 	printPoint(points[i], dim);
	// }

	*pPermissiblePoints = realloc(permissiblePoints, sizeof(Point)*permissiblePointNum);
	return permissiblePointNum;


}

