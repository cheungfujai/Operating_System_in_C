#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>
#include<unistd.h>
#include"util.h"


int asgn2_pthread(Point * points, Point ** pPermissiblePoints, int number, int dim, int thread_number){
   

	int permissiblePointNum = 0;
	Point * permissiblePoints = NULL;

	int returnarray[number];
	int j = 0;
	int record[number];
	int counter = 0;
	int recordcount = 0;
	int smallrecord[dim];
	int count = 0;

	// for(int k = 0; k < number; k++){
		for(int i = 0; i < number; i++){
			for(int k = 0; k < number; k++){
				if (i == k){continue;}
				for(j = 0; j < dim; j++){
					if (count = 0){
						if(points[i].values[j] > points[k].values[j]){	
							count++;
						}
					}
					else{
						if(points[i].values[j] > points[k].values[j]){
							count++;
							if (count == dim - 1 && k == dim - 1){
								smallrecord[recordcount] = k + 1;
								recordcount++;
								if(points[k].values[j] < points[smallrecord[recordcount]].values[j]){
									smallrecord[recordcount] = k + 1;
								}
								else{
									break;
								}
							}

						}
						else if (points[i].values[j] < points[k].values[j]){
							count = 0;
							break;
						}
						else{
							break;
						}
					}
				}
				count = 0;
			}
		}
	// }

		for(int h = 0; h < recordcount; h++){
			printf("%d test", smallrecord[h]);
		}
		printf("\n");

		// for(int h = 0; h < number; h++){
		// 	printPoint(points[h], dim);
		// }

    // pPermissiblePoints -- your computed answer
	*pPermissiblePoints = permissiblePoints;
	return permissiblePointNum;


}