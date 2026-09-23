#include <stdio.h>
int main(){
    
    //reading the data x values and y target values.

    //Let's start by creating a simple function that we are trying to model. 
    int x[100] = {4, 16, -16, -16, 11, 4, 8, -12, 8, 2, 14, 18, -11, -15, -18, -15, 9, -12, -1, -20, -2, 16, -6, 9, 2, -2, 10, 13, 5, 16, -7, 13, 14, 10, 1, 15, -15, -6, 0, 17, -4, -13, 17, -11, -15, -5, 11, -12, 14, 8, -18, -14, 10, 0, -13, -3, -15, 18, 19, 3, 10, 18, -2, -16, -16, 20, -4, -6, -18, 8, -12, 3, 8, -12, 13, 0, 16, 4, 4, 14, -16, -4, 10, -14, 9, 6, 12, -19, -14, -15, -11, -13, -8, 14, 18, 6, -9, -16, 0, -14};
    double y[100];
    int k;
    for (k=0; k<100; k++){
        y[k] = 7 * (double)x[k] + 3;
    }

    
    // currently, we don't know the optimal amount parameters, nor their values. but - we can build the basics. 
    // let's suppose that we are working with the simplest model, where the format is y=wx+b.
    // Then, we can write the loss function. 
    double w = 1.0;
    double b = 0.0;
    
    
    //let's create a prediction array, which we will compare later. 
    double pred[100];
    

    // now, we need to build the mechanism for prediction and choosing optimal parameters. Let's build our basic loops.
    int i;
    double error[100];
    double sum = 0;
    double gradient_error_w[100];
    double gradient_w = 0;
    double gradient_b = 0;
    double dw;
    double db;
    double loss = 0;  
    double updated_w;
    double updated_b;
    double a_w = 0.00001;
    double a_b = 0.01;
    int epoch = 100000;
    int j;
    double error_root[100];
    
    //lets have here our initial training loop.
    for(i=0; i<100; i++){
        pred[i]=w*x[i] + b;
        }
    for(i=0; i<100; i++){
            error[i]=(y[i]-pred[i]) * (y[i]-pred[i]);
            error_root[i]= error[i] / (y[i]-pred[i]);


            gradient_error_w[i] = error_root[i] * (-2*x[i]);
        }
    for(i=0; i<100; i++){
            sum += error[i];
            gradient_w += gradient_error_w[i];
            gradient_b += -2 * error_root[i];
        }
    loss = sum / 100;
    dw = gradient_w / 100;
    db = gradient_b / 100;

    updated_w = w - a_w * dw;
    updated_b = b - a_b * db;
    printf("Initial loop completed\n");
    printf("Initial W is %lf\n", w);
    printf("Updated W is %lf\n", updated_w);
    

    
    
    //training loop
     for(j=1; j<=epoch; j++){
        loss = 0;
        dw = 0;
        db = 0;
        w = updated_w;
        b = updated_b;
        sum = 0;
        gradient_w = 0;
        gradient_b = 0;

        for(i=0; i<100; i++){
        pred[i]=w*x[i] + b;
        }
        for(i=0; i<100; i++){
            error[i]=(y[i]-pred[i]) * (y[i]-pred[i]);
            error_root[i]= error[i] / (y[i]-pred[i]);
            gradient_error_w[i] = error_root[i] * (-2 * x[i]);
        }
        for(i=0; i<100; i++){
            sum += error[i];
            gradient_w += gradient_error_w[i];
            gradient_b += -2 * error_root[i];
        }
        loss = sum / 100;
        dw = gradient_w / 100;
        db = gradient_b / 100;


        updated_w = w - a_w * dw;
        updated_b = b - a_b * db;
        printf("loss in epoch %i is %lf\n", j, loss);
        printf("w in epoch %i is %lf\n", j, updated_w);
        printf("dw is - %lf\n", dw);
        printf("db is - %lf\n", db);
        printf("a*dw is - %lf\n",a_w * dw);
        printf("a*db is - %lf\n",a_b * db);

    }
    printf("Training complete\n");
    printf("Final regression parameters -> w = %lf, b=%lf\n", updated_w, updated_b);



}