//
// Created by Sebastian Lettner on 31.12.17.
//

#ifndef CI_VECTORQUANTIZATION_H
#define CI_VECTORQUANTIZATION_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#import <time.h>

double training_data[1000][2];
int num_clusters;
int num_training_examples;
double max_input;

void read_input() {

    char inputLine[50];
    char delimiter[] = ",";
    char *ptr;
    int j = 0;
    int i = 0;
    max_input = 0;

    while(scanf("%s\n", inputLine) != EOF) {


        ptr = strtok(inputLine, delimiter);
        j = 1;
        while(ptr != NULL) {

            double num = atof(ptr);
            if(i == 0) {
                num_clusters = (int)num;
            }

            if( (i!=0) &&( j==1 || j==2)) {
                if (num>max_input) {
                    max_input = num;
                }
                training_data[i-1][j-1] = num;
            }

            ptr = strtok(NULL, delimiter);
            j++;
        }
        i++;
    }
    num_training_examples = i-1;

}

double double_rand(double min, double max) {


    double scale = (double)rand() / (double) RAND_MAX;
    return min + scale * (max-min);

}
int random_number(int min_num, int max_num) {
    int result = 0, low_num = 0, hi_num = 0;

    if (min_num < max_num)
    {
        low_num = min_num;
        hi_num = max_num + 1; // include max_num in output
    } else {
        low_num = max_num + 1; // include max_num in output
        hi_num = min_num;
    }

    result = (rand() % (hi_num - low_num)) + low_num;
    return result;
}

double** initialize_centroids(int num_clusters) {

    double** centroids = (double**)malloc(sizeof(double*)*num_clusters);
    for (int i = 0; i < num_clusters ; ++i) {
        centroids[i] = (double*)malloc(sizeof(double)*2);
    }
    for (int j = 0; j < num_clusters; ++j) {
        int random_n = random_number(0,num_training_examples-1);
        centroids[j][0] = training_data[random_n][0]; //double_rand(-max_input/(double)2,max_input/(double)2);
        centroids[j][1] = training_data[random_n][1];//double_rand(-max_input/(double)2,max_input/(double)2);
    }
    return centroids;
}

double euclid_norm(double a1, double a2, double b1, double b2) {

    double temp1 = a1 - b1;
    double temp2 = a2 - b2;
    temp1 = temp1 * temp1;
    temp2 = temp2 * temp2;
    temp1 = temp1 + temp2;
    return sqrt(temp1);

}

void k_means_algorithm() {

    double cost = 1000000;
    double** final_centroids;
    double curent_intis[num_clusters][2];

    for (int k1 = 0; k1 < 10; ++k1) {


        double **centroids = initialize_centroids(num_clusters);
        
        for (int i1 = 0; i1 < num_clusters; ++i1) {
            curent_intis[i1][0] = centroids[i1][0];
            curent_intis[i1][1] = centroids[i1][1];
        }

        FILE *g = fopen("/Users/sebastianlettner/Google Drive/5.Semester WS 17-18 /CompInt/CI_Data/HW3T1/init_centroids",
                        "w");
        if (g == NULL) {
            printf("Error opening file!\n");
            exit(1);
        }
        for (int n = 0; n < num_clusters; ++n) {
            fprintf(g, "%f,%f\n", centroids[n][0], centroids[n][1]);
        }
        fclose(g);



        int cluster_assignment[num_training_examples];

        for (int j = 0; j < 10; ++j) {

            for (int i = 0; i < num_training_examples; ++i) {
                double shortest_norm = 0;
                for (int k = 0; k < num_clusters; ++k) {

                    if (k == 0) {
                        shortest_norm = euclid_norm(training_data[i][0], training_data[i][1], centroids[k][0],
                                                    centroids[k][1]);
                        cluster_assignment[i] = k + 1;
                    }
                    else {
                        if (euclid_norm(training_data[i][0], training_data[i][1], centroids[k][0], centroids[k][1]) <
                            shortest_norm) {
                            shortest_norm = euclid_norm(training_data[i][0], training_data[i][1], centroids[k][0],
                                                        centroids[k][1]);
                            cluster_assignment[i] = k + 1;
                        }//end if
                    }//end else
                }//for k

            }//end for i

            for (int l = 1; l <= num_clusters; ++l) {

                double sum1 = 0;
                double sum2 = 0;
                int count = 0;
                for (int i = 0; i < num_training_examples; ++i) {

                    if (cluster_assignment[i] == l) {
                        sum1 += training_data[i][0];
                        sum2 += training_data[i][1];
                        count++;
                    }
                }
                // calc mean
                if (count != 0) {
                    sum1 = sum1 / ((double) count);
                    sum2 = sum2 / ((double) count);

                    centroids[l - 1][0] = sum1;
                    centroids[l - 1][1] = sum2;
                }
            }



        }
        double current_cost = 0;
        for (int m = 0; m < num_training_examples; ++m) {
            for (int i = 1; i <= num_clusters; ++i) {
                if (cluster_assignment[m] == i) {
                    current_cost += pow(euclid_norm(training_data[m][0], training_data[m][1], centroids[i - 1][0],
                                            centroids[i - 1][1]), 2);
                }
            }
        }
        current_cost = current_cost / ((double) num_training_examples);
        //printf("Cost: %f\n", current_cost);
        if (current_cost < cost) {
            cost = current_cost;
            final_centroids = centroids;
            FILE *g = fopen("/Users/sebastianlettner/Google Drive/5.Semester WS 17-18 /CompInt/CI_Data/HW3T1/init_centroids",
                            "w");
            if (g == NULL) {
                printf("Error opening file!\n");
                exit(1);
            }
            for (int n = 0; n < num_clusters; ++n) {
                fprintf(g, "%f,%f\n", centroids[n][0], centroids[n][1]);
            }
            fclose(g);
        }

    }


    FILE *f = fopen("/Users/sebastianlettner/Google Drive/5.Semester WS 17-18 /CompInt/CI_Data/HW3T1/centroids", "w");
    if (f == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    for (int n = 0; n < num_clusters; ++n) {
        fprintf(f, "%f,%f\n", final_centroids[n][0], final_centroids[n][1]);
    }
    fclose(f);


    for (int n = 0; n < num_clusters; ++n) {
        printf("%f,%f\n", final_centroids[n][0], final_centroids[n][1]);

    }


}


#endif //CI_VECTORQUANTIZATION_H
