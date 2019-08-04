
/***********************************************************************/
/* Student Name: Panagiotis Goulas, Student ID: 886001                 */
/* Assignment 2: Subject: ADS - Comp20003                              */
/* Time and Date Submitted: 8:30   am - 20/10/2018                     */
/*                        Programming is Fun                           */
/***********************************************************************/


#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/time.h>

#define PUZZLE_TILES           16

/**
 * READ THIS DESCRIPTION
 *
 * node data structure: containing state, g, f
 * you can extend it with more information if needed
 */
typedef struct node{
    int state[PUZZLE_TILES];
    int g;
    int f;
} node;

/**
 * Global Variables
 */

// used to track the position of the blank in a state,
// so it doesn't have to be searched every time we check if an operator is
// applicable
// When we apply an operator, blank_pos is updated
int blank_pos;
int saved = 0;

// Initial node of the problem
node initial_node;

// Statistics about the number of generated and expendad nodes
unsigned long generated;
unsigned long expanded;


/**
 * The id of the four available actions for moving the blank (empty slot). e.x.
 * Left: moves the blank to the left, etc.
 */


#define POSSIBLE_DIRECTIONS     4
#define LEFT                    0
#define RIGHT                   1
#define UP                      2
#define DOWN                    3
#define FIRST                   1
#define FIFTH                   5
#define MINUS_TWO              -2
#define TWO_HUNDRED_FIFTY_SIX 256
#define TWO                     2
#define ONE_THOUSAND         1000
#define FOUR                    4
#define ONE_MILION        1000000


/*
 * Helper arrays for the applicable function
 * applicability of operators: 0 = left, 1 = right, 2 = up, 3 = down
 */
int ap_opLeft[]  = { 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1 };
int ap_opRight[]  = { 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0 };
int ap_opUp[]  = { 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
int ap_opDown[]  = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 };
int *ap_ops[] = { ap_opLeft, ap_opRight, ap_opUp, ap_opDown };

/* manhattan distance */
int manhattan( int* array1 ) {
	int a = 0, b = 0, c = 0, d = 0, result = 0, i = 0;
	while (i<PUZZLE_TILES) {
		if (array1[i] != 0) {
			a = i/POSSIBLE_DIRECTIONS;
			b = i % POSSIBLE_DIRECTIONS;
			c = array1[i] / POSSIBLE_DIRECTIONS;
			d  = array1[i] % POSSIBLE_DIRECTIONS;
			result = result + abs(a - c) + abs(d - b);
		}
		i += 1;
	}
    return(result);
}
/* copy array2 in array1 */
void arraycopy(int array1[], int array2[]) {
	int i = 0;
	while (i<PUZZLE_TILES){
		array1[i] = array2[i];
		i++;
	}
}
/* find minimum of 2 ints */
int min(int a, int b){
	if (a<b){
		return a;
	} else {
		return b;
	}
}

/* check state */
int check_state(int i, int saved) {
	int sum_num = 0;
	int condition = 0;
	sum_num = i + saved;
	if (sum_num != FIFTH){
		if (sum_num != FIRST) {
			condition = 1;
		}
	}
	return condition;
}

/* set to INT_MAX */
int set_inf(int num){
	num = INT_MAX;
	return num;
}

/* int b to int a */
int swap_num(int a, int b){
	int c = 0;
	c = b;
	a = c;
	return a;
}

/* initialize array */
void initialize_array(int array[],int n){
	int k = 0;
	while (k < n) {
		array[k] = 0;
		k += 1;
	}
}

/* print state */
void print_state( int* s )
{
    int i;

    for( i = 0; i < PUZZLE_TILES; i++ )
        printf( "%2d%c", s[i], ((i+1) % 4 == 0 ? '\n' : ' ') );
}

void printf_comma (long unsigned int n) {
    if (n < 0) {
        printf ("-");
        printf_comma (-n);
        return;
    }
    if (n < ONE_THOUSAND) {
        printf ("%lu", n);
        return;
    }
    printf_comma (n/ONE_THOUSAND);
    printf (",%03lu", n%ONE_THOUSAND);
}

/* return 1 if op is applicable in state, otherwise return 0 */
int applicable( int op )
{
        return( ap_ops[op][blank_pos] );
}


/* apply operator */
void apply( node* n, int op )
{
    int t;

    //find tile that has to be moved given the op and blank_pos
    t = blank_pos + (op == 0 ? -1 : (op == 1 ? 1 : (op == TWO ? -FOUR : FOUR)));

    //apply op
    n->state[blank_pos] = n->state[t];
    n->state[t] = 0;

    //update blank pos
    blank_pos = t;
}

/* Recursive IDA */
node* ida( node* oldNode, int threshold, int* newThreshold )
{
	/**
	 * FILL WITH YOUR CODE
	 *
	 * Algorithm in Figure 2 of handout
	 */
    // initialize Variables
	// struct node* curState = NULL;
	// curState->g = oldNode->g;
	// curState->f = oldNode->f;
	/* initialize values */
	int saved_n[PUZZLE_TILES], saved2,condition = 0,j=0;
	int saved_g = 0, saved_f = 0, save = 0,count5 = 0;
	arraycopy(saved_n, oldNode->state);
    saved2 = 0;
	save = saved;
	saved2 = save;
	int count = 0;
	node* r = NULL;
	/* save */
	saved_g = oldNode->g;
	saved_f = oldNode ->f;

	/* all possibilities */
	while (j<FOUR){
    /* when the movement reached the edage, reset the data to the previous
        *  status
        */
	    /* copy */
		arraycopy(oldNode->state, saved_n);
		save = saved2;
		saved = save;
		oldNode -> g = saved_g;
        oldNode -> f = saved_f;
		condition = check_state(j, saved);
		if ((applicable(j)) && (condition == 1)) {
			apply(oldNode, j);
			oldNode  -> g = oldNode  -> g + 1;
			oldNode  -> f = oldNode  -> g + manhattan(oldNode -> state);
			save = j;
			saved = save;
			if (oldNode -> f > threshold){
				*newThreshold = min(oldNode -> f, *newThreshold);
			}
			else {
				
				if (manhattan(oldNode -> state) == 0){
					return oldNode;
				}
				count5 += 1;
				/* run */
				r = ida(oldNode, threshold, newThreshold);
				if (r!=NULL){
					return r;
				}
				expanded = expanded + 1;
			}
			generated = generated + 1;
		}
	/* count how many times passes from here for debug */
	count +=1;
	j +=1;
	}
	return( NULL );	
}

/* main IDA control loop */
int IDA_control_loop(){
    int threshold = 0;
    /* compute initial threshold B */
	threshold = manhattan( initial_node.state );
	initial_node.f = threshold;
	printf( "Initial Estimate = %d\nThreshold = %d", threshold, threshold);
	/**
	 * FILL WITH YOUR CODE
	 *
	 * Algorithm in Figure 1 of handout
	 */
	/* initializing and creating variables */
	int count2 = 0;
	int count3 = 0;
	int saved_nn[PUZZLE_TILES];
	
	/* initialize statistics */
    generated = 0;
    expanded = 0;

	node* r = NULL;
	int newThreshold =0;
	arraycopy(saved_nn, initial_node.state);
    while (r==NULL){
        newThreshold = set_inf(newThreshold);
      	initial_node.g = 0;
		arraycopy(initial_node.state, saved_nn);
        saved = set_inf(saved);
        r = ida (&initial_node, threshold, &newThreshold);

        if (r==NULL){
            threshold = newThreshold;
            printf (" %d", threshold);
			count3 += 0;
        }
		count2 +=1;
    }
    if(r)
        return r -> g;
    else
        return -1;
}

static inline float compute_current_time()
{
	struct rusage r_usage;
	
	getrusage( RUSAGE_SELF, &r_usage );	
	float diff_time = (float) r_usage.ru_utime.tv_sec;
	diff_time += (float) r_usage.ru_stime.tv_sec;
	diff_time += (float) r_usage.ru_utime.tv_usec / (float)ONE_MILION;
	diff_time += (float) r_usage.ru_stime.tv_usec / (float)ONE_MILION;
	return diff_time;
}

int main( int argc, char **argv )
{
	int i, solution_length;

	/* check we have a initial state as parameter */
	if( argc != TWO )
	{
		fprintf( stderr, "usage: %s \"<initial-state-file>\"\n", argv[0] );
		return( -1 );
	}


	/* read initial state */
	FILE* initFile = fopen( argv[1], "r" );
	char buffer[TWO_HUNDRED_FIFTY_SIX];

	if( fgets(buffer, sizeof(buffer), initFile) != NULL ){
		char* tile = strtok( buffer, " " );
		for( i = 0; tile != NULL; ++i )
			{
				initial_node.state[i] = atoi( tile );
				blank_pos = (initial_node.state[i] == 0 ? i : blank_pos);
				tile = strtok( NULL, " " );
			}		
	}
	else{
		fprintf( stderr, "Filename empty\"\n" );
		return( MINUS_TWO );

	}
       
	if( i != PUZZLE_TILES )
	{
		fprintf( stderr, "invalid initial state\n" );
		return( -1 );
	}

	/* initialize the initial node */
	initial_node.g=0;
	initial_node.f=0;

	print_state( initial_node.state );


	/* solve */
	float t0 = compute_current_time();
	
	solution_length = IDA_control_loop();				

	float tf = compute_current_time();

	/* report results */
	printf( "\nSolution = %d\n", solution_length);
	printf( "Generated = ");
	printf_comma(generated);		
	printf("\nExpanded = ");
	printf_comma(expanded);		
	printf( "\nTime (seconds) = %.2f\nExpanded/Second = ", tf-t0 );
	printf_comma((unsigned long int) expanded/(tf+0.00000001-t0));
	printf("\n\n");

	/* aggregate all executions in a file named report.dat, */
	/* for marking purposes */
	FILE* report = fopen( "report.dat", "a" );

	fprintf( report, "%s", argv[1] );
	fprintf( report, "\n\tSoulution = %d, Generated = %lu, Expanded = %lu",
					 solution_length, generated, expanded);
	fprintf( report, ", Time = %f, Expanded/Second = %f\n\n", tf-t0,
					 (float)expanded/(tf-t0));
	fclose(report);
	
	return( 0 );
}

/*                        Programming is Fun                           */
