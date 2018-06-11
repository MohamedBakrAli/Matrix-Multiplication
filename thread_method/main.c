#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/time.h>
#include <pthread.h>

double ** arr_a;
double ** arr_b;
double ** arr_c;
int row_a, col_a, row_b, col_b;

char * out_file_name_1;
char * out_file_name_2;
char * out_file_name_3;


struct index {
   int i; /* row */
   int j; /* column */
};

double** read_file(char* path,int *r, int *c);
void print_file_array(double** arr,char * path);;
void first_method();
void *calculate_cell (void * param);
void secod_method();
void * calculate_row(void * param);
void third_method();


int main(int argc, char *argv[])
{

    if (argc > 1){
        arr_a = read_file(argv[1],&row_a,&col_a);
        arr_b = read_file(argv[2],&row_b,&col_b);
        out_file_name_1 = malloc((strlen(argv[3]) + 7) * sizeof(char));
        out_file_name_2 = malloc((strlen(argv[3]) + 7) * sizeof(char));
        out_file_name_3 = malloc((strlen(argv[3]) + 7) * sizeof(char));
        strcpy(out_file_name_1,argv[3]);
        strcpy(out_file_name_2,argv[3]);
        strcpy(out_file_name_3,argv[3]);
    }
    else {
        arr_a = read_file("a_10000.txt",&row_a,&col_a);
        arr_b = read_file("a_10000.txt",&row_b,&col_b);
        out_file_name_1 = malloc(10 * sizeof(char));
        out_file_name_2 = malloc(10 * sizeof(char));
        out_file_name_3 = malloc(10 * sizeof(char));
        strcpy(out_file_name_1,"out");
        strcpy(out_file_name_2,"out");
        strcpy(out_file_name_3,"out");

    }
    if (col_a != row_b){
        printf("the number of rows in the first array is not equal the number of columns in the second array!");
        return 0;
    }
    strcat(out_file_name_1,"_1.txt");
    strcat(out_file_name_2,"_2.txt");
    strcat(out_file_name_3,"_3.txt");

    struct timeval stop,start;
    
    int i;
    arr_c = malloc(row_a * sizeof (double));
    for (i = 0; i < row_a; i++){
        arr_c[i] = malloc(col_b * sizeof(double));
    }

    // first method
    gettimeofday(&start, NULL); //start checking time
    first_method();
    gettimeofday(&stop, NULL); //end checking time
    printf("First method (%d threads): taken %lu Microseconds.\n", row_a * col_b, stop.tv_usec - start.tv_usec);
  
    printf("hhhhhhhhhhhhhhhhh\n");
    // second method
    gettimeofday(&start, NULL); //start checking time
    secod_method();
    gettimeofday(&stop, NULL); //end checking time
    printf("Second method (%d threads): taken %lu Microseconds.\n",row_a, stop.tv_usec - start.tv_usec);
    
    // third method 
    gettimeofday(&start, NULL); //start checking time
    third_method();
    gettimeofday(&stop, NULL); //end checking time
    printf("First method (%d threads): taken %lu Microseconds.\n", 1, stop.tv_usec - start.tv_usec);
    // main terminate
    pthread_exit(NULL);
    return 0;

}

// read input array from the file
double** read_file(char* path,int *r, int *c)
{
    int row, column;

    int i,j;
    FILE* input = fopen(path, "r");
    if (input == NULL){
        printf("the file [%s] is not found!\n",path);
        return NULL;
    }
    fscanf(input,"row=%d col=%d",&row,&column);
    *r = row;
    *c = column;
    double **arr = malloc(row * sizeof(double));
    for (i = 0; i < row; i++)
        arr[i] = malloc(column * sizeof(double));
    for (i = 0; i < row; i++) {
        for (j = 0; j < column; j++){
            fscanf(input,"%lf",&arr[i][j]);
        }
    }
    fclose(input);
    return arr;
}


//write output array in the file
void print_file_array(double** arr,char * path)
{
    int i,j;
    FILE* output = fopen(path, "a");
    for (i = 0; i < row_a; i++){
        for (j = 0; j < col_b; j++){
            fprintf(output, "%lf\t",arr[i][j]);
        }
        fprintf(output, "\n");
    }
    fclose(output);
}

// frist method using #elements (#rows * # cols) threads
void first_method()
{
    int i,j;
    int thread_cnt = row_a * col_b;
    int cnt =0;
    pthread_t threads[thread_cnt]; /* declare an array of type pthread_t*/
   
    for (i = 0; i < row_a; i++){
        for (j = 0; j < col_b; j++){
            struct index *data = (struct index *) malloc(sizeof(struct index));
            data->i = i;
            data->j = j;
			// create pthreads
         	pthread_create(&threads[i + j], NULL, calculate_cell, (void *)data);
         	//ake sure the parent waits for all thread to complete
         	pthread_join(threads[i + j], NULL);
        }

    }
	
    print_file_array(arr_c,out_file_name_1);
}

void *calculate_cell (void * param)
{

    struct index *data = (struct index *)param;
	int x, sum = 0;
    for (x = 0; x < col_a; x++){
        sum += arr_a[data->i][x] * arr_b[x][data->j];
    }
    arr_c[data->i][data->j] = sum;
}

// second method using # rows threads
void secod_method()
{
    int i;
    pthread_t threads[row_a];
    // creat row_a threads
    for (i = 0; i < row_a; i++){
        pthread_create(&threads[i], NULL, calculate_row,(void *)i);
    }
    // join pthreads
    for (i = 0; i < row_a; i++){
        pthread_join(threads[i], NULL);
    }
    print_file_array(arr_c,out_file_name_2);
}

void * calculate_row(void * param)
{
    int i = (int *) param;
    int j ,k ,sum = 0;
    for (j = 0; j < col_b ;j ++){
        for (k = 0; k < col_a; k++){
          sum += arr_a[i][k] * arr_b[k][j];
        }
        arr_c[i][j] = sum;
        sum = 0;
    }
}

// third method is the simple way
void third_method()
{
	int i;
	struct index *data = (struct index *) malloc(sizeof(struct index));
	for (int i = 0; i < row_a; i++){
		for (int j = 0; j < col_b; j++){
            data->i = i;
            data->j = j; 
            calculate_cell(data); 
		}
	}
	print_file_array(arr_c,out_file_name_3);
}
