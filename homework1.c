#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#ifdef __linux
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#endif

#ifdef _WIN32
#include <windows.h>
#endif

#define STACK_SIZE 100
struct direction {
	int horiz, vert;
};
struct element {
	int row, col, dir;
};
struct direction move[8]={{0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}};
void path(int row_size, int col_size, int *maze) {
	int i, j, row, col, next_row, next_col, dir, top=1, cur;
	bool found;
	struct element position;
	struct element stack[STACK_SIZE];
	bool *mark, *maze_path;
#ifdef _WIN32
	HANDLE hConsole;
	bool is_color=false;
#endif
	mark=(bool *)malloc(row_size*col_size*sizeof(bool));
	memset(mark, 0, row_size*col_size*sizeof(bool));
	stack[0].row=0;
	stack[0].col=0;
	stack[0].dir=0;
	while (top>-1 && !found) {
		position=stack[--top];
		row=position.row;
		col=position.col;
		dir=position.dir;
		while (dir<8 && !found) {
			next_row=row+move[dir].vert;
			next_col=col+move[dir].horiz;
			if (next_row==row_size-1 && next_col==col_size-1)
				found=true;
			else if (next_row>-1 && next_row<row_size &&
					next_col>-1 && next_col<col_size &&
					!maze[next_row*col_size+next_col] &&
					!mark[next_row*col_size+next_col]) {
				mark[next_row*col_size+next_col]=true;
				position.row=row;
				position.col=col;
				position.dir=++dir;
				stack[top++]=position;
				row=next_row;
				col=next_col;
				dir=0;
			}
			else
				++dir;
		}
	}
	if (found) {
#ifdef _WIN32
		hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
#endif
		maze_path=(bool *)malloc(row_size*col_size*sizeof(bool));
		memset(maze_path, 0, row_size*col_size*sizeof(bool));
		for (i=0;i<top;i++)
			maze_path[stack[i].row*col_size+stack[i].col]=true;
		maze_path[row*col_size+col]=true;
		maze_path[(row_size-1)*col_size+col_size-1]=true;
		for (i=0;i<row_size;++i) {
			for (j=0;j<col_size;++j) {
				cur=i*col_size+j;
#ifdef __linux
				if (maze_path[cur])
					printf(ANSI_COLOR_RED"%d"ANSI_COLOR_RESET" ", maze[cur]);
				else
					printf("%d ", maze[cur]);
#endif
#ifdef _WIN32
				if (maze_path[cur] && !is_color) {
					SetConsoleTextAttribute(hConsole, 12);
					is_color=true;
				}
				else if (!maze_path[cur] && is_color) {
					SetConsoleTextAttribute(hConsole, 15);
					is_color=false;
				}
				printf("%d ", maze[cur]);
#endif
			}
			printf("\n");
		}
		free(maze_path);
	}
	else
		printf("The maze does not have a path\n");
	free (mark);
}
int main() {
	int row, col, i, j;
	int *maze;
	scanf("%d %d", &row, &col);
	maze=(int *)malloc(row*col*sizeof(int));
	for (i=0;i<row;++i) {
		for (j=0;j<col;++j)
			scanf("%d", &maze[i*col+j]);
	}
	path(row, col, maze);
	free(maze);
	return 0;
}
