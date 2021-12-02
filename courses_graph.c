#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#define MAX_FILE_NAME_LENGTH 30
#define MAX_COURSE_NAME_LENGTH 30
#define MAX_LINE_LENGTH_IN_FILE 1000



int get_total_distinct_courses(FILE * fp);
void display_vertex_course_correspodence(char ** course_arr, int total_courses);
int ** alloc_2d(int rows, int columns);
int ** build_adjacency_matrix(FILE * fp,char ** course_arr,int num_of_courses,int** adj_mtx);
int get_index(char * course,char ** course_arr,int num_of_courses);
void printMatrix(int ** array, int rows, int columns);
void free_2d(int ** array, int rows, int columns);
void free_2d_c(char ** array, int rows, int columns);

typedef struct struct_graph * graphPT;

// creates a graph with the specified number of vertices
graphPT newGraph(int N, int undirected);

// deallocates the memory allocated to graph g.
void destroyGraph(graphPT g);

// returns the number of vertices in g.
int numVertices(graphPT g);

// connects vertices v1 and v2 in the graph.
// returns 1 if successful, 0 if errors occurred.
int addEdge(graphPT g, int v1, int v2);

// disconnects vertices v1 and v2 in the graph.
// returns 1 if successful, 0 if errors occurred.
int removeEdge(graphPT g, int v1, int v2);

// returns 1 if the specified edge exists, 0 otherwise.
int edgeExists(graphPT g, int v1, int v2);

// deallocates the memory allocated to graph g.
void destroyGraph(graphPT g);


struct struct_graph
{
   int undirected; // value 1 means it is undirected, value other than 1 means it is directed.
   int N;    // number of vertices
   int ** E;  // edges
};



int main()
{
    printf("This program will read, from a file, a list of courses and their prerequisites and will print the list in which to take cousres.\n");
    printf("Enter Filename:\n");
    char filename[MAX_FILE_NAME_LENGTH],file_content[MAX_LINE_LENGTH_IN_FILE+1];
    scanf("%s",filename);
    FILE* fp = fopen(filename,"r");
    if(fp==NULL)
        {
            perror("Could not open file badName. Exit\n\n");
            printf("Failed to read from file. Program will terminate.\n");
            exit(-1);
        }
    int num_of_courses = get_total_distinct_courses(fp);
    rewind(fp);
    printf("Number of vertices in built graph:  N = %d\n",num_of_courses);
    char ** courses_arr = (char**) calloc(num_of_courses,sizeof(char*));
    int n=0;
    char* token;m
     while (fgets(file_content, sizeof(file_content), fp) != NULL)
    {
        courses_arr[n] = (char*)calloc(MAX_COURSE_NAME_LENGTH,sizeof(char));
        token = strtok(file_content, " \n");
        strcpy(courses_arr[n++], token);
    }
    /*for(int i=0; i<num_of_courses; i++)
        {
            courses_arr[i] = (char*)calloc(MAX_COURSE_NAME_LENGTH,sizeof(char));
            fscanf(fp,"%s%*[^\n]",courses_arr[i]);
        }*/
    display_vertex_course_correspodence(courses_arr,num_of_courses);
    rewind(fp);
    graphPT graph = newGraph(num_of_courses,0);
     //int** adj_mtx = alloc_2d(num_of_courses,num_of_courses);
    //build_adjacency_matrix(fp,courses_arr,num_of_courses,adj_mtx);
    read_file(fp,graph,courses_arr);
    printMatrix(graph->E,num_of_courses,num_of_courses);
    free_2d_c(courses_arr,num_of_courses,num_of_courses);
    //free_2d(adj_mtx,num_of_courses,num_of_courses);
    destroyGraph(graph);
    fclose(fp);
    return 0;
}


// creates a graphPT with the specified number of vertices
graphPT newGraph(int N, int undirected)
{
   graphPT result = (graphPT)malloc(sizeof(*result));
   result->undirected = undirected;
   result->N = N;
   result->E = alloc_2d(N,N);
   return result;
}

void read_file(FILE *fp, graphPT graph_ptr, char **course_arr)
{
    char file_content[MAX_LINE_LENGTH_IN_FILE+1];
    char *course;
    int i=0;
    int num_of_courses = numVertices(graph_ptr);
    while (fgets(file_content, sizeof(file_content), fp) != NULL)
    {
        course = strtok(file_content, " \n");
        while (course != NULL)
        {
            if (strcmp(course_arr[i], course) != 0)
            {
                int index = get_index(course,course_arr,num_of_courses);
                add_edge(graph_ptr, index, i);
            }
            course = strtok(NULL, " \n");
        }
        i++;
    }
}

void add_edge(graphPT graph_ptr , int v1, int v2)
{
    graph_ptr->E[v1][v2] = 1;
}


void destroyGraph(graphPT g)
{
   if (g == NULL) return;
   free_2d(g->E, g->N, g->N);
   free(g);
}

int get_total_distinct_courses(FILE * fp)
{
    int num_of_courses = 0;
    char file_content[MAX_LINE_LENGTH_IN_FILE+1];
    while (fgets(file_content, sizeof(file_content), fp) != NULL)
        {
            num_of_courses++;
        }
    rewind(fp);
    return num_of_courses;
}

void display_vertex_course_correspodence(char ** course_arr, int total_courses)
{
    printf("Vertex - coursename correspondence\n");
    for(int i=0; i<total_courses; i++)
        {
            printf("%2d - %s\n",i,course_arr[i]);
        }
}




int get_index(char * course,char ** course_arr,int num_of_courses)
{
    if(course!=NULL)
        {
            for(int i=0; i<num_of_courses; i++)
                {
                    if(strcmp(course,course_arr[i])==0)
                        {
                            return i;
                        }
                }
        }
    return -1;
}

int numVertices(graphPT g)
{
   if (g == NULL) return 0;
   return g->N;
}

//Matrix Utility Functions
int ** alloc_2d(int rows, int columns)
{
    int ** table = (int**)calloc(rows, sizeof(int *));

    for (int row = 0; row < rows; row++)
        { table[row] = (int*)calloc(columns, sizeof(int)); }

    return table;
}


void free_2d(int ** array, int rows, int columns)
{
    for (int row = 0; row < rows; row++)
        {
            free(array[row]);
        }
    free(array);
}

void free_2d_c(char ** array, int rows, int columns)
{
    for (int row = 0; row < rows; row++)
        {
            free(array[row]);
        }
    free(array);
}

void printMatrix(int ** array, int rows, int columns)
{
    printf("\nAdjacency matrix:\n");
    printf("%4s|", "");
    for(int i=0; i<columns; i++)
        {
            printf("%5d", i);
        }
    printf("\n");
    for(int i=0; i<(columns+1)*5; i++)
        {
            printf("%c", '-');
        }
    printf("\n");
    for (int row = 0; row < rows; row++)
        {
            printf("%4d|", row);
            for (int col = 0; col < columns; col++)
                {
                    printf("%5d", array[row][col]);
                }
            printf("\n");
        }
    printf("\n");
}





