#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_FILE_NAME_LENGTH 30
#define MAX_COURSE_NAME_LENGTH 30
#define MAX_LINE_LENGTH_IN_FILE 1000

int get_total_distinct_courses(FILE *fp);
void display_vertex_course_correspodence(char **course_arr, int total_courses);
int **alloc_2d(int rows, int columns);
int **build_adjacency_matrix(FILE *fp, char **course_arr, int num_of_courses, int **adj_mtx);
int get_index(char *course, char **course_arr, int num_of_courses);
void printMatrix(int **array, int rows, int columns);
void free_2d(int **array, int rows, int columns);
void free_2d_c(char **array, int rows, int columns);

int perform_dfs(int curr_node, int courses, int courses_count, int nodes_visit_status[courses_count], int **adj_mtx, int *final_order, int *unprocessed, int *processing, int *processed, int *cycle)
{
    nodes_visit_status[curr_node] = 1;
    processing[curr_node] = 1;
    unprocessed[curr_node] = 0;
    int adjacent_nodes[courses_count];
    int adjacent_nodes_size = 0;
    for (int j = 0; j < courses_count; j++)
    {
        if (adj_mtx[curr_node][j] == 1)
        {
            adjacent_nodes[adjacent_nodes_size] = j;
            adjacent_nodes_size += 1;
        }
    }
    for (int j = 0; j < adjacent_nodes_size; j++)
    {
        int next = adjacent_nodes[j];
        if (processed[next] == 1)
        {
            continue;
        }
        if (processing[next] == 1)
        {
            *cycle = 1;
        }
        if (courses > 0 && unprocessed[next] == 1)
        {
            courses = perform_dfs(next, courses, courses_count, nodes_visit_status, adj_mtx, final_order, unprocessed, processing, processed, cycle);
        }
    }
    processing[curr_node] = 0;
    processed[curr_node] = 1;
    final_order[courses - 1] = curr_node;
    return courses - 1;
}

int main()
{
    printf("This program will read, from a file, a list of courses and their prerequisites and will print the list in which to take cousres.\n");
    printf("Enter Filename:\n");
    char filename[MAX_FILE_NAME_LENGTH];
    scanf("%s", filename);
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        perror("Could not open file badName. Exit\n\n");
        printf("Failed to read from file. Program will terminate.\n");
        exit(-1);
    }
    int num_of_courses = get_total_distinct_courses(fp);
    printf("Number of vertices in built graph:  N = %d\n", num_of_courses);
    char **courses_arr = (char **)calloc(num_of_courses, sizeof(char *));
    for (int i = 0; i < num_of_courses; i++)
    {
        courses_arr[i] = (char *)calloc(MAX_COURSE_NAME_LENGTH, sizeof(char));
        fscanf(fp, "%s%*[^\n]", courses_arr[i]);
    }
    display_vertex_course_correspodence(courses_arr, num_of_courses);
    rewind(fp);
    int **adj_mtx = alloc_2d(num_of_courses, num_of_courses);
    build_adjacency_matrix(fp, courses_arr, num_of_courses, adj_mtx);
    printMatrix(adj_mtx, num_of_courses, num_of_courses);

    // DFS start

    int nodes_visit_status[num_of_courses];
    int final_order[num_of_courses];
    int unprocessed[num_of_courses];
    int processing[num_of_courses];
    int processed[num_of_courses];
    for (int i = 0; i < num_of_courses; i++)
    {
        unprocessed[i] = 1;
        processing[i] = 0;
        processed[i] = 0;
    }
    int cycle_exists = 0;
    for (int i = 0; i < num_of_courses; i++)
    {
        nodes_visit_status[i] = 0;
    }
    int vertices = num_of_courses;
    for (int i = 0; i < num_of_courses; i++)
    {
        if (nodes_visit_status[i] == 0)
        {
            vertices = perform_dfs(i, vertices, num_of_courses, nodes_visit_status, adj_mtx, final_order, unprocessed, processing, processed, &cycle_exists);
        }
    }
    if (cycle_exists)
    {
        printf("\n\nThere was at least one cycle. There is no possible ordering of the courses.\n");
    }
    else
    {
        printf("\nOrder in which to take courses:\n");
        for (int i = 0; i < num_of_courses; i++)
        {
            printf("%d. - %s (corresponds to graph vertex %d)\n", i + 1, courses_arr[final_order[i]], final_order[i]);
        }
    }

    free_2d_c(courses_arr, num_of_courses, num_of_courses);
    free_2d(adj_mtx, num_of_courses, num_of_courses);
    fclose(fp);
    return 0;
}

int get_total_distinct_courses(FILE *fp)
{
    int num_of_courses = 0;
    char course_name[MAX_COURSE_NAME_LENGTH];
    while (fscanf(fp, "%s%*[^\n]", course_name) != EOF)
    {
        num_of_courses++;
    }
    rewind(fp);
    return num_of_courses;
}

void display_vertex_course_correspodence(char **course_arr, int total_courses)
{
    printf("Vertex - coursename correspondence\n");
    for (int i = 0; i < total_courses; i++)
    {
        printf("%2d - %s\n", i, course_arr[i]);
    }
}

int **build_adjacency_matrix(FILE *fp, char **course_arr, int num_of_courses, int **adj_mtx)
{
    while (!feof(fp))
    {
        char file_content[MAX_LINE_LENGTH_IN_FILE];
        fscanf(fp, "%[^\n] ", file_content);
        if (file_content != NULL)
        {
            char *ptr;
            ptr = strtok(file_content, " ");
            if (ptr != NULL)
            {
                int column = get_index(ptr, course_arr, num_of_courses);
                ptr = strtok(NULL, " ");
                while (ptr != NULL)
                {
                    int row = get_index(ptr, course_arr, num_of_courses);
                    if (column == -1)
                    {
                        printf("Unable to build adjacency matrix");
                        exit(-1);
                    }
                    adj_mtx[row][column] = 1;
                    ptr = strtok(NULL, " ");
                }
            }
        }
    }
    return adj_mtx;
}

int get_index(char *course, char **course_arr, int num_of_courses)
{
    if (course != NULL)
    {
        for (int i = 0; i < num_of_courses; i++)
        {
            if (strcmp(course, course_arr[i]) == 0)
            {
                return i;
            }
        }
    }
    return -1;
}

int **alloc_2d(int rows, int columns)
{
    int **table = (int **)calloc(rows, sizeof(int *));

    for (int row = 0; row < rows; row++)
    {
        table[row] = (int *)calloc(columns, sizeof(int));
    }

    return table;
}

void free_2d(int **array, int rows, int columns)
{
    for (int row = 0; row < rows; row++)
    {
        free(array[row]);
    }
    free(array);
}

void free_2d_c(char **array, int rows, int columns)
{
    for (int row = 0; row < rows; row++)
    {
        free(array[row]);
    }
    free(array);
}

void printMatrix(int **array, int rows, int columns)
{
    printf("\nAdjacency matrix:\n");
    printf("%4s|", "");
    for (int i = 0; i < columns; i++)
    {
        printf("%5d", i);
    }
    printf("\n");
    for (int i = 0; i < (columns + 1) * 5; i++)
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
