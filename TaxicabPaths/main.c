//
//  main.c
//  TaxicabPaths
//
//  Created by Mike Lewis on 4/26/15.
//  Copyright (c) 2015 Mike Lewis. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct cell {
    int row;
    int col;
    int level;
    int filled;
} cell;

int targetrow, targetcol, targetlev, startrow, startcol, startlev;


int getCellsIndex( int numrows, int numcols, int numlevs, int row, int col, int lev ) {
    int retval = (lev-1)*(numcols*numrows)+(row-1)*numcols + (col-1);
    
    return( retval );
}

void convertDirsToCells( char *dirs ) {
    int i,row,col,lev;
    
    row = startrow;
    col = startcol;
    lev = startlev;
    
    printf("[%d,%d,%d]",startrow,startcol,startlev);
    for (i = 0; i < strlen(dirs); i++){
        if (dirs[i] == 'N')
            row++;
        else if (dirs[i] == 'E')
            col++;
        else if (dirs[i] == 'S')
            row--;
        else if (dirs[i] == 'W')
            col--;
        else if (dirs[i] == 'U')
            lev++;
        else if (dirs[i] == 'D')
            lev--;
        printf(",[%d,%d,%d]",row,col,lev);
    }
    printf("\n");
}

void printCells( cell *cells, int numrows, int numcols, int numlevs, char *dirs ){
    int row, col, lev;
    
    printf("\n");
    for (lev = 1; lev <= numlevs; lev++) {
        for (row = numrows; row >= 1; row--){
            for (col = 1; col <= numcols; col++)
                printf("%d ", cells[getCellsIndex( numrows, numcols, numlevs, row, col, lev )].filled );
            printf("\n");
        }
        printf("\n");
    }
    printf("%s\n", dirs);
    convertDirsToCells( dirs );
}

void initCells( cell *cells, int numrows, int numcols, int numlevs ){
    int row, col, lev, idx;
    
    for (row = 1; row <= numrows; row++) {
        for (col = 1; col <= numcols; col++) {
            for (lev = 1; lev <= numlevs; lev++) {
                idx = getCellsIndex( numrows, numcols, numlevs, row, col, lev );
                cells[idx].row = row;
                cells[idx].col = col;
                cells[idx].level = lev;
                cells[idx].filled = 0;
            }
        }
    }
}

void fillCell( cell *cells, int numrows, int numcols, int numlevs, int row, int col, int lev ){
    int testrow, testcol, testlev, idx;
    
    for (testlev = 1; testlev <= numlevs; testlev++) {
        for (testrow = 1; testrow <= numrows; testrow++) {
            for (testcol = 1; testcol <= numcols; testcol++) {
                idx = getCellsIndex( numrows, numcols, numlevs, testrow, testcol, testlev );
                if (cells[idx].row == row &&
                    cells[idx].col == col &&
                    cells[idx].level == lev)
                    cells[idx].filled = 1;
            }
        }
    }
}

void nextCell( cell *cells, int numrows, int numcols, int numlevs, int currow, int curcol, int curlev, char *dirs ) {
    
    int dirslen;
    
    dirslen = strlen( dirs );
    
    // If we're at the target, print and return.
    if (currow == targetrow && curcol == targetcol && curlev == targetlev) {
        printCells(cells, numrows, numcols, numlevs, dirs );
        return;
    }
    else {
        if (currow + 1 <= numrows && cells[getCellsIndex(numrows, numcols, numlevs, currow+1, curcol, curlev)].filled == 0) {
            cells[getCellsIndex(numrows, numcols, numlevs, currow+1, curcol, curlev)].filled = 1;
            dirs[dirslen] = 'N';
            nextCell( cells, numrows, numcols, numlevs, currow+1, curcol, curlev, dirs);
            cells[getCellsIndex(numrows, numcols, numlevs, currow+1, curcol, curlev)].filled = 0;
            dirs[dirslen] = '\0';
        }
        if (curcol + 1 <= numcols && cells[getCellsIndex(numrows, numcols, numlevs, currow, curcol+1, curlev)].filled == 0) {
            cells[getCellsIndex(numrows, numcols, numlevs, currow, curcol+1, curlev)].filled = 1;
            dirs[dirslen] = 'E';
            nextCell( cells, numrows, numcols, numlevs, currow, curcol+1, curlev, dirs);
            cells[getCellsIndex(numrows, numcols, numlevs, currow, curcol+1, curlev)].filled = 0;
            dirs[dirslen] = '\0';
        }
        if (currow - 1 >= 1 && cells[getCellsIndex(numrows, numcols, numlevs, currow-1, curcol, curlev)].filled == 0) {
            cells[getCellsIndex(numrows, numcols, numlevs, currow-1, curcol, curlev)].filled = 1;
            dirs[dirslen] = 'S';
            nextCell( cells, numrows, numcols, numlevs, currow-1, curcol, curlev, dirs);
            cells[getCellsIndex(numrows, numcols, numlevs, currow-1, curcol, curlev)].filled = 0;
            dirs[dirslen] = '\0';
        }
        if (curcol - 1 >= 1 && cells[getCellsIndex(numrows, numcols, numlevs, currow, curcol-1, curlev)].filled == 0) {
            cells[getCellsIndex(numrows, numcols, numlevs, currow, curcol-1, curlev)].filled = 1;
            dirs[dirslen] = 'W';
            nextCell( cells, numrows, numcols, numlevs, currow, curcol-1, curlev, dirs);
            cells[getCellsIndex(numrows, numcols, numlevs, currow, curcol-1, curlev)].filled = 0;
            dirs[dirslen] = '\0';
        }
        if (curlev + 1 <= numlevs && cells[getCellsIndex(numrows, numcols, numlevs, currow, curcol, curlev+1)].filled == 0) {
            cells[getCellsIndex(numrows, numcols, numlevs, currow, curcol, curlev+1)].filled = 1;
            dirs[dirslen] = 'U';
            nextCell( cells, numrows, numcols, numlevs, currow, curcol, curlev+1, dirs);
            cells[getCellsIndex(numrows, numcols, numlevs, currow, curcol, curlev+1)].filled = 0;
            dirs[dirslen] = '\0';
        }
        if (curlev - 1 >= 1 && cells[getCellsIndex(numrows, numcols, numlevs, currow, curcol, curlev-1)].filled == 0) {
            cells[getCellsIndex(numrows, numcols, numlevs, currow, curcol, curlev-1)].filled = 1;
            dirs[dirslen] = 'D';
            nextCell( cells, numrows, numcols, numlevs, currow, curcol, curlev-1, dirs);
            cells[getCellsIndex(numrows, numcols, numlevs, currow, curcol, curlev-1)].filled = 0;
            dirs[dirslen] = '\0';
        }
    }
    
}

int main(int argc, const char * argv[]) {
    
    int numrows = atoi(argv[1]);
    int numcols = atoi(argv[2]);
    int numlevs = atoi(argv[3]);
    startrow = atoi(argv[4]);
    startcol = atoi(argv[5]);
    startlev = atoi(argv[6]);
    
    targetrow = atoi(argv[7]);
    targetcol = atoi(argv[8]);
    targetlev = atoi(argv[9]);
    
    cell cells[numrows*numcols*numlevs];
    char dirs[numrows*numcols*numlevs+1];
    
    int i;
    
    for (i = 0; i < numrows*numcols*numlevs+1; i++) {
        dirs[i] = '\0';
    }
    
    initCells(cells, numrows, numcols, numlevs);
    
    printCells(cells, numrows, numcols, numlevs, dirs );
    
    fillCell(cells, numrows, numcols, numlevs, startrow, startcol, startlev);
    printCells(cells, numrows, numcols, numlevs, dirs );
    
    nextCell( cells, numrows, numcols, numlevs, startrow, startcol, startlev, dirs );
    
    
    return 0;
}
