#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<math.h>
int main()
{
// let's build knn
// we will mainly use the same preprocessing that we used in regression model.
// thus, skip to the next comment with nametag "Actual new stuff" when reading this.

    //GOAL 1 implementation.
    FILE *data;
    // n = amt of rows, param - amount of parameters. 
    int n;
    int param;

    printf("Enter the amount of rows in the matrix\n");
    scanf("%i", &n);
    printf("Enter the amount of parameters of the model. Supply this program only with the params you want to use in the data matrix.\n");
    scanf("%i", &param);
    data = fopen("param_values.csv", "r");
    if(data == NULL){
        printf("data file doesn't exist.\n");
        return 1;
    }

    //make the matrix layout
    double matrix[n][param];
    int r = 0;
    //Lets read the csv table
    char rows[1024];
    char *token;
    while (fgets(rows, sizeof(rows), data) != NULL){
        token = strtok(rows, ",");
        int c = 0;
        while(token != NULL){
            matrix[r][c] = strtod(token, NULL);
            token = strtok(NULL, ",");
            c++;
        }
        r++;
    }
    // X matrix of parameters ready
    printf("X matrix built\n");
    
    
    
    // Now let's get our target array going
    
    FILE *target_data;
    target_data = fopen("target.csv", "r");
    if(target_data == NULL){
        printf("Target dataset empty/wrong path");
        return 1;
    }
    int target[n];
    
    int c = 0;
    char *token_y;
    while (fgets(rows, sizeof(rows), target_data) != NULL){
        token_y = strtok(rows, ",");
        while(token_y != NULL){
            target[c] = strtod(token_y, NULL);
            token_y = strtok(NULL, ",");
            c++;
        }
    }
    // Y array of targets ready
    printf("Y array built\n");

    //closing data
    fclose(data);
    fclose(target_data);

    //GOAL 1 IMPLEMENTED


    //GOAL 2 
    
    //First, let's build the shuffling algoythm. 
    int i;
    int j;
    int z;
    

    // build indices list
    
    int indices[n];
    for(i=0; i<n; i++){
        indices[i]=i;
    }

    // shuffle the indices
    for(i=n-1; i>0; i--){
        
        j = rand() % (i+1);
        z = 0;
        z = indices[i];
        indices[i] = indices[j];
        indices[j] = z;
    }

    int old_row;
    int p;
    double new_matrix[n][param];
    int new_target[n];
    //assign values
    for(i=0;i<n;i++){
            old_row = indices[i];
            for(p=0;p<param;p++){
                new_matrix[i][p] = matrix[old_row][p];
            }
            new_target[i]=target[old_row];
        }
    
    // Train/val/test split

    //First, let's calculate the amount of rows that should go into each set
    double train_n = ((double)n / 100) * 70;
    int train = (int)train_n;
    int val; 
    int test;
    int diff = n - train;
    if (diff % 2 != 0){
        val = (diff / 2) + 1;
        test = diff - val; 
    }
    else{
        val = diff / 2;
        test = diff - val;
    }
    double train_matrix[train][param];
    double val_matrix[val][param];
    double test_matrix[test][param];

    int train_class[train];
    int val_class[val];
    int test_class[test];

    //Let's now asign new matrices their values.
    
    for(p=0; p<param; p++){
        for(i=0; i<train; i++){
            train_matrix[i][p] = new_matrix[i][p];
        }
    }
    for(i=0; i<train; i++){
        train_class[i] = new_target[i];
    }

   
    for(p=0; p<param; p++){
        for(i=0; i<val; i++){
            val_matrix[i][p] = new_matrix[train+i][p];
        }
    }
    for(i=0; i<val; i++){
        val_class[i] = new_target[train+i];
    }


    
    for(p=0; p<param; p++){
        for(i=0; i<test; i++){
            test_matrix[i][p] = new_matrix[train+val+i][p];
        }
    }
    for(i=0; i<test; i++){
        test_class[i] = new_target[train+val+i];
    }


    // Feature scaling. 

    // FIRST OF ALL - here feature updating based on user input.




    int keeping_check[param];
    printf("Enter keep or enter discard for each parameter\n");
    for(i=0; i<param; i++){
        char input[30];
        scanf("%29s", input);
        if(strcmp(input, "keep") == 0){
            keeping_check[i] = 1;
        }
        else if(strcmp(input, "discard") == 0){
            keeping_check[i] = 0;
        }
        else{
            printf("Please, enter a valid input, either keep or discard\n");
            i--;
            continue;
        }
    }
    int updated_param = 0;
    for(p=0;p<param;p++){
        if(keeping_check[p] == 1){
            updated_param += 1;
        }
    }
    int new_p = 0;
    
    double features_train[train][updated_param];
    double features_val[val][updated_param];
    double features_test[test][updated_param];
    
    
    for(p=0; p<param; p++){
        if (keeping_check[p] == 1){
                for(i=0; i<train; i++){
                    features_train[i][new_p] = train_matrix[i][p];
                }
                for(i=0; i<val; i++){
                    features_val[i][new_p] = val_matrix[i][p];
                }
                for(i=0; i<test; i++){
                    features_test[i][new_p] = test_matrix[i][p];
                }
                new_p +=1;
        }
    }
   
   //INPUT DONE NOW TO MEAN AND STUFF
   
   
    double train_sum[updated_param];

    // u and q for train
    
    for(p=0; p<updated_param; p++){
        train_sum[p]=0;
        for(i=0; i<train; i++){
            train_sum[p] += features_train[i][p];
        }
    }

    // Now, let's calculate u and q for each parameter.  
    // First, of course, we need the mean, since stdev can be calculated with mean.
    double u_train[updated_param];


    for(p=0; p<updated_param; p++){
        u_train[p]=train_sum[p] / train;    
    }

    
    // Hooray, we got our mean per each parameter in each matrix! now, we need to find st.dev. 
    // Plan - 1) Find squared diff 2) sum squared diff 3) divide the sum by count 4) take the root. How though, in C? I'll have to search for it a bit.

    double sq_diff_train_sum[updated_param];

    
    
    for(p=0; p<updated_param; p++){
        sq_diff_train_sum[p] = 0;
        for(i=0; i<train; i++){
            sq_diff_train_sum[p] += (features_train[i][p] - u_train[p]) * (features_train[i][p] - u_train[p]);
        }
    }


    // great, now divide by rows count and take the square root. 

    double q_train[updated_param];

    for(p=0; p<updated_param; p++){
        q_train[p] = sqrt(sq_diff_train_sum[p] / train);
        if (q_train[p] == 0){
            printf("You have a constant value in parameter %i. Please, in the next iteration of the program, drop that parameter.", p);
            return 1;
        }
    }



    // now, create scaled values. 
    double X_train[train][updated_param];
    double X_val[val][updated_param];
    double X_test[test][updated_param];
    
    for(p=0; p<updated_param; p++){
        for(i=0; i<train; i++){
            X_train[i][p]=(features_train[i][p] - u_train[p]) / q_train[p];
        }
        for(i=0; i<val; i++){
            X_val[i][p]=(features_val[i][p] - u_train[p]) / q_train[p];
        }
        for(i=0; i<test; i++){
            X_test[i][p]=(features_test[i][p] - u_train[p]) / q_train[p];
        }
    }

    // GOAL 3 IMPLEMENTED


    // Back to the regression.
    printf("Data pipeline built. knn parameters chosen: %d\n", updated_param);

    
    // Data pipeline built. now off to actual knn algorythm
    // for distance we can use euclidian distance formula, then we flag each point and write the distance into an array
    // then, we can use a basic sorting algorythm in order to sort them from lowest to highest(bubble sort should suffice).
    // then, we pick k number of those data points, starting from the lowest, from the array and flag them.
    // then, we loop through each data point's class and calculate the most frequent ones appearing by majority vote. 
    // then, the majority voted class gets assigned
    

    int k;
    double sum;

    struct Neighbor {
        double distance;
        int class;
    };

    struct Neighbor neighbors[train];
    for (i = 0; i < val; i++) {

    for (k = 0; k < train; k++) {

        sum = 0.0;

        for (p = 0; p < updated_param; p++) {
            double diff = X_val[i][p] - X_train[k][p];
            sum += diff * diff;
        }

        neighbors[k].distance = sqrt(sum);
        neighbors[k].class = train_class[k];
    }
    struct Neighbor temp;
    while (1) {
        bool swap_flag = false;
        for (k = 0; k < train - 1; k++) {
            if (neighbors[k].distance > neighbors[k + 1].distance) {
                temp = neighbors[k];
                neighbors[k] = neighbors[k + 1];
                neighbors[k + 1] = temp;
                swap_flag = true;
            }
        }
        if (swap_flag == 0) {
            break;
        }
    }



    // sort neighbors here
    // take first K here
    // majority vote here
    // assign prediction for X_val[i]
}







}