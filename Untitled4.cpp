#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <windows.h> 

// ========================================================
// THEME DARK MODE (Không xanh lá, không tím, khung rõ)
// ========================================================

// Mã màu Windows API (0-15)
#define COLOR_BLACK          0
#define COLOR_DARK_BLUE      1
#define COLOR_DARK_GREEN     2
#define COLOR_DARK_CYAN      3
#define COLOR_DARK_RED       4
#define COLOR_DARK_MAGENTA   5
#define COLOR_DARK_YELLOW    6
#define COLOR_LIGHT_GRAY     7
#define COLOR_DARK_GRAY      8
#define COLOR_BRIGHT_BLUE    9
#define COLOR_BRIGHT_GREEN   10
#define COLOR_BRIGHT_CYAN    11
#define COLOR_BRIGHT_RED     12
#define COLOR_BRIGHT_MAGENTA 13
#define COLOR_BRIGHT_YELLOW  14
#define COLOR_WHITE          15

// Ð?nh nghia màu theo vai trò (KHÔNG xanh lá, KHÔNG tím)
#define COLOR_HEADER         COLOR_BRIGHT_CYAN      // Tiêu d? chính (xanh ng?c)
#define COLOR_SUB_HEADER     COLOR_BRIGHT_YELLOW    // Tên SV (vàng)
#define COLOR_BORDER         COLOR_BRIGHT_CYAN      // Ðu?ng vi?n b?ng (xanh ng?c - RÕ hon)
#define COLOR_TABLE_BORDER   COLOR_BRIGHT_CYAN      // Vi?n b?ng so sánh (xanh ng?c sáng)
#define COLOR_LABEL          COLOR_BRIGHT_YELLOW    // Nhãn S, D (vàng)
#define COLOR_DATA           COLOR_WHITE            // D? li?u thu?ng (tr?ng)
#define COLOR_VALUE          COLOR_BRIGHT_CYAN      // Giá tr? k?t qu? (xanh ng?c - thay xanh lá)
#define COLOR_STEP           COLOR_BRIGHT_YELLOW    // Các bu?c l?p (vàng - thay tím)
#define COLOR_WARNING        COLOR_BRIGHT_YELLOW    // C?nh báo (vàng)
#define COLOR_ERROR          COLOR_BRIGHT_RED       // L?i (d?)
#define COLOR_PROMPT         COLOR_BRIGHT_CYAN      // Câu h?i nh?p (xanh ng?c)
#define COLOR_MENU           COLOR_BRIGHT_YELLOW    // S? th? t? menu (vàng)
#define COLOR_MENU_TITLE     COLOR_BRIGHT_CYAN      // Tiêu d? menu (xanh ng?c)
#define COLOR_EXIT           COLOR_DARK_GRAY        // Thoát (xám)
#define COLOR_DELTA          COLOR_BRIGHT_RED       // Delta âm (d?)
#define COLOR_COMPARE_TITLE  COLOR_BRIGHT_YELLOW    // Tiêu d? b?ng so sánh (vàng)
#define COLOR_COMPARE_BORDER COLOR_BRIGHT_CYAN      // Vi?n b?ng so sánh (xanh ng?c sáng)

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

#define MAX 500

int supply[MAX], demand[MAX];
int cost[MAX][MAX], allocation[MAX][MAX];
int row, col;

void drawHeader() {
    setColor(COLOR_HEADER);
    printf("+===========================================================+\n");
    printf("|                PBL1 LAP TRINH TINH TOAN                   |\n");
    printf("|                DE TAI: BAI TOAN VAN TAI                   |\n");
    printf("+-----------------------------------------------------------+\n");
    setColor(COLOR_SUB_HEADER);
    printf("|      SVTH: 1. Phan Phu Quan        - MSSV: 102250088      |\n");
    printf("|            2. Hoang Phan Bao Trang - MSSV: 102250097      |\n");
    setColor(COLOR_HEADER);
    printf("+===========================================================+\n");
    setColor(COLOR_DATA);
}

void printLine(int columns, int width) {
    int k, w;
    setColor(COLOR_BORDER);
    for (k = 0; k < columns; k++) {
        printf("+");
        for (w = 0; w < width; w++) printf("-");
    }
    printf("+\n");
    setColor(COLOR_DATA);
}

void printTable(const char* title, bool showEmptyCost) {
    int cell_width = 12;
    int i, j;
    
    setColor(COLOR_HEADER);
    printf("\n===== %s =====\n", title);
    setColor(COLOR_DATA);

    printLine(col + 2, cell_width);

    // In Dong Header (D1, D2...)
    setColor(COLOR_BORDER);
    printf("|");
    setColor(COLOR_LABEL);
    printf("%-12s", "");
    setColor(COLOR_BORDER);
    printf("|");
    for (j = 0; j < col; j++) {
        setColor(COLOR_LABEL);
        printf(" D%-10d", j + 1);
        setColor(COLOR_BORDER);
        printf("|");
    }
    setColor(COLOR_LABEL);
    printf(" %-11s", "Supply");
    setColor(COLOR_BORDER);
    printf("|\n");

    printLine(col + 2, cell_width);

    // In Noi Dung
    for (i = 0; i < row; i++) {
        setColor(COLOR_BORDER);
        printf("|");
        setColor(COLOR_LABEL);
        printf("S%-11d", i + 1);
        setColor(COLOR_BORDER);
        printf("|");
        
        setColor(COLOR_DATA);
        for (j = 0; j < col; j++) {
            char cell[20];
            if (allocation[i][j] > 0) {
                snprintf(cell, sizeof(cell), "%d(%d)", cost[i][j], allocation[i][j]);
                setColor(COLOR_VALUE);
                printf(" %-11s", cell);
                setColor(COLOR_BORDER);
                printf("|");
                setColor(COLOR_DATA);
            } else {
                if (showEmptyCost) {
                    snprintf(cell, sizeof(cell), "%d", cost[i][j]);
                    printf(" %-11s", cell);
                } else {
                    printf(" %-11s", "");  
                }
                setColor(COLOR_BORDER);
                printf("|");
                setColor(COLOR_DATA);
            }
        }
        setColor(COLOR_LABEL);
        printf(" %-11d", supply[i]);
        setColor(COLOR_BORDER);
        printf("|\n");
        printLine(col + 2, cell_width);
    }

    // In chu Demand o goc duoi trai
    setColor(COLOR_BORDER);
    printf("|");
    setColor(COLOR_LABEL);
    printf("%-12s", "Demand");
    setColor(COLOR_BORDER);
    printf("|");
    
    for (j = 0; j < col; j++) {
        setColor(COLOR_LABEL);
        printf(" %-11d", demand[j]);
        setColor(COLOR_BORDER);
        printf("|");
    }
    printf(" %-11s", "");
    setColor(COLOR_BORDER);
    printf("|\n");

    printLine(col + 2, cell_width);
    setColor(COLOR_DATA);
}

bool readDataFromFile(const char* filename) {
    int i, j;
    FILE* file = fopen(filename, "r");
    if (!file) {
        setColor(COLOR_ERROR);
        printf("Khong mo duoc file %s\n", filename);
        setColor(COLOR_DATA);
        return false;
    }

    if (fscanf(file, "%d %d", &row, &col) != 2) {
        setColor(COLOR_ERROR);
        printf("Loi doc kich thuoc ma tran\n");
        setColor(COLOR_DATA);
        fclose(file);
        return false;
    }

    for (i = 0; i < row; i++) {
        if (fscanf(file, "%d", &supply[i]) != 1) {
            setColor(COLOR_ERROR);
            printf("Loi doc supply tai hang %d\n", i + 1);
            setColor(COLOR_DATA);
            fclose(file);
            return false;
        }
    }

    for (j = 0; j < col; j++) {
        if (fscanf(file, "%d", &demand[j]) != 1) {
            setColor(COLOR_ERROR);
            printf("Loi doc demand tai cot %d\n", j + 1);
            setColor(COLOR_DATA);
            fclose(file);
            return false;
        }
    }

    for (i = 0; i < row; i++) {
        for (j = 0; j < col; j++) {
            if (fscanf(file, "%d", &cost[i][j]) != 1) {
                setColor(COLOR_ERROR);
                printf("Loi doc chi phi tai o [%d][%d]\n", i + 1, j + 1);
                setColor(COLOR_DATA);
                fclose(file);
                return false;
            }
        }
    }

    fclose(file);
    return true;
}

void initializeAllocation() {
    int i, j;
    for (i = 0; i < row; i++) {
        for (j = 0; j < col; j++) {
            allocation[i][j] = 0;
        }
    }
}

int northwestCornerMethod() {
    int tempSupply[MAX], tempDemand[MAX];
    int i = 0, j = 0, step = 1;
    int totalCost = 0;
    int r, c;
    bool first;

    setColor(COLOR_HEADER);
    printf("\n=== PHUONG PHAP GOC TAY BAC ===\n");
    setColor(COLOR_DATA);
    
    initializeAllocation();
    printTable("Ma tran chi phi ban dau", true);
    
    for (i = 0; i < row; i++) tempSupply[i] = supply[i];
    for (j = 0; j < col; j++) tempDemand[j] = demand[j];
    
    i = 0;
    j = 0;
    while (i < row && j < col) {
        int quantity = (tempSupply[i] < tempDemand[j]) ? tempSupply[i] : tempDemand[j];
        allocation[i][j] = quantity;
        tempSupply[i] -= quantity;
        tempDemand[j] -= quantity;
        totalCost += quantity * cost[i][j];
        
        setColor(COLOR_STEP);
        printf("\nBuoc %d: Phan phoi %d don vi tai S%d-D%d\n", step++, quantity, i + 1, j + 1);
        setColor(COLOR_DATA);
        printf("Chi phi: %d * %d = %d\n", quantity, cost[i][j], quantity * cost[i][j]);
        printTable("Ket qua tam thoi", false);
        
        setColor(COLOR_WARNING);
        if (tempSupply[i] == 0) {
            printf("Da dung het cung o S%d, di chuyen xuong duoi\n", i + 1);
            i++;
        } else {
            printf("Da thoa man cau o D%d, di chuyen sang phai\n", j + 1);
            j++;
        }
        setColor(COLOR_DATA);
    }
    
    setColor(COLOR_VALUE);
    printf("\n=> TONG CHI PHI: T = ");
    first = true;
    for (r = 0; r < row; r++) {
        for (c = 0; c < col; c++) {
            if (allocation[r][c] > 0) {
                if (!first) printf(" + ");
                printf("(%d*%d)", cost[r][c], allocation[r][c]);
                first = false;
            }
        }
    }
    printf(" = %d\n", totalCost);
    setColor(COLOR_DATA);
    
    return totalCost;
}

int leastCostMethod() {
    int tempSupply[MAX], tempDemand[MAX];
    int step = 1;
    int totalCost = 0;
    int i, j, r, c;
    bool first;
    
    setColor(COLOR_HEADER);
    printf("\n=== PHUONG PHAP CHI PHI NHO NHAT ===\n");
    setColor(COLOR_DATA);
    
    initializeAllocation();
    printTable("Ma tran chi phi ban dau", true);
    
    for (i = 0; i < row; i++) tempSupply[i] = supply[i];
    for (j = 0; j < col; j++) tempDemand[j] = demand[j];
    
    while (true) {
        int minCost = INT_MAX;
        int minI = -1, minJ = -1;
        
        for (i = 0; i < row; i++) {
            if (tempSupply[i] == 0) continue;
            
            for (j = 0; j < col; j++) {
                if (tempDemand[j] == 0) continue;
                
                if (cost[i][j] < minCost) {
                    minCost = cost[i][j];
                    minI = i;
                    minJ = j;
                }
            }
        }
        
        if (minI == -1 || minJ == -1) break;
        
        int quantity = (tempSupply[minI] < tempDemand[minJ]) ? tempSupply[minI] : tempDemand[minJ];
        allocation[minI][minJ] = quantity;
        tempSupply[minI] -= quantity;
        tempDemand[minJ] -= quantity;
        totalCost += quantity * cost[minI][minJ];
        
        setColor(COLOR_STEP);
        printf("\nBuoc %d: Phan phoi %d don vi tai S%d-D%d (chi phi nho nhat %d)\n", step++, quantity, minI + 1, minJ + 1, minCost);
        setColor(COLOR_DATA);
        printf("Chi phi: %d * %d = %d\n", quantity, cost[minI][minJ], quantity * cost[minI][minJ]);
        printTable("Ket qua tam thoi", false); 
        
        setColor(COLOR_WARNING);
        if (tempSupply[minI] == 0) printf("Da dung het cung o S%d\n", minI + 1);
        if (tempDemand[minJ] == 0) printf("Da thoa man cau o D%d\n", minJ + 1);
        setColor(COLOR_DATA);
    }
    
    setColor(COLOR_VALUE);
    printf("\n=> TONG CHI PHI: T = ");
    first = true;
    for (r = 0; r < row; r++) {
        for (c = 0; c < col; c++) {
            if (allocation[r][c] > 0) {
                if (!first) printf(" + ");
                printf("(%d*%d)", cost[r][c], allocation[r][c]);
                first = false;
            }
        }
    }
    printf(" = %d\n", totalCost);
    setColor(COLOR_DATA);
    
    return totalCost;
}

int vogelApproximationMethod() {
    int tempSupply[MAX], tempDemand[MAX];
    int step = 1;
    int totalCost = 0;
    bool rowDone[MAX] = {false};
    bool colDone[MAX] = {false};
    int remainingRows = row, remainingCols = col;
    int i, j, r, c;
    bool first;
    
    setColor(COLOR_HEADER);
    printf("\n=== PHUONG PHAP VOGEL ===\n");
    setColor(COLOR_DATA);
    
    initializeAllocation();
    printTable("Ma tran chi phi ban dau", true);
    
    for (i = 0; i < row; i++) tempSupply[i] = supply[i];
    for (j = 0; j < col; j++) tempDemand[j] = demand[j];
    
    while (remainingRows > 0 && remainingCols > 0) {
        int rowPenalty[MAX] = {0};
        for (i = 0; i < row; i++) {
            if (rowDone[i]) continue;
            int min1 = INT_MAX, min2 = INT_MAX;
            for (j = 0; j < col; j++) {
                if (colDone[j]) continue;
                int currentCost = cost[i][j];
                if (currentCost < min1) { min2 = min1; min1 = currentCost; } 
                else if (currentCost < min2) { min2 = currentCost; }
            }
            rowPenalty[i] = (min2 == INT_MAX) ? 0 : (min2 - min1);
        }
        
        int colPenalty[MAX] = {0};
        for (j = 0; j < col; j++) {
            if (colDone[j]) continue;
            int min1 = INT_MAX, min2 = INT_MAX;
            for (i = 0; i < row; i++) {
                if (rowDone[i]) continue;
                int currentCost = cost[i][j];
                if (currentCost < min1) { min2 = min1; min1 = currentCost; } 
                else if (currentCost < min2) { min2 = currentCost; }
            }
            colPenalty[j] = (min2 == INT_MAX) ? 0 : (min2 - min1);
        }
        
        int maxPenalty = -1;
        bool isRow = true;
        int index = -1;
        
        for (i = 0; i < row; i++) {
            if (!rowDone[i] && rowPenalty[i] > maxPenalty) {
                maxPenalty = rowPenalty[i]; isRow = true; index = i;
            }
        }
        for (j = 0; j < col; j++) {
            if (!colDone[j] && colPenalty[j] > maxPenalty) {
                maxPenalty = colPenalty[j]; isRow = false; index = j;
            }
        }
        
        int minCost = INT_MAX;
        int bestI = -1, bestJ = -1;
        
        if (isRow) {
            for (j = 0; j < col; j++) {
                if (!colDone[j] && cost[index][j] < minCost) {
                    minCost = cost[index][j]; bestI = index; bestJ = j;
                }
            }
        } else {
            for (i = 0; i < row; i++) {
                if (!rowDone[i] && cost[i][index] < minCost) {
                    minCost = cost[i][index]; bestI = i; bestJ = index;
                }
            }
        }
        
        int quantity = (tempSupply[bestI] < tempDemand[bestJ]) ? tempSupply[bestI] : tempDemand[bestJ];
        allocation[bestI][bestJ] = quantity;
        tempSupply[bestI] -= quantity;
        tempDemand[bestJ] -= quantity;
        totalCost += quantity * cost[bestI][bestJ];
        
        setColor(COLOR_STEP);
        printf("\nBuoc %d: Phan phoi %d don vi tai S%d-D%d (sai so = %d)\n", step++, quantity, bestI + 1, bestJ + 1, maxPenalty);
        setColor(COLOR_DATA);
        printf("Chi phi: %d * %d = %d\n", quantity, cost[bestI][bestJ], quantity * cost[bestI][bestJ]);
        printTable("Ket qua tam thoi", false); 
        
        setColor(COLOR_WARNING);
        if (tempSupply[bestI] == 0) {
            rowDone[bestI] = true; remainingRows--;
            printf("Da dung het cung o S%d\n", bestI + 1);
        }
        if (tempDemand[bestJ] == 0) {
            colDone[bestJ] = true; remainingCols--;
            printf("Da thoa man cau o D%d\n", bestJ + 1);
        }
        setColor(COLOR_DATA);
    }

    setColor(COLOR_VALUE);
    printf("\n=> TONG CHI PHI: T = ");
    first = true;
    for (r = 0; r < row; r++) {
        for (c = 0; c < col; c++) {
            if (allocation[r][c] > 0) {
                if (!first) printf(" + ");
                printf("(%d*%d)", cost[r][c], allocation[r][c]);
                first = false;
            }
        }
    }
    printf(" = %d\n", totalCost);
    setColor(COLOR_DATA);
    
    return totalCost;
}

bool findCycle(int path[][2], int* pathLength, int startI, int startJ, bool isBasic[MAX][MAX]) {
    bool temp[MAX][MAX];
    int i, j;
    bool changed;
    int r = startI, c = startJ;
    int len = 0;
    bool moveHorizontal = true; 

    for (i = 0; i < row; i++)
        for (j = 0; j < col; j++)
            temp[i][j] = isBasic[i][j];
            
    temp[startI][startJ] = true; 
    
    do {
        changed = false;
        for (i = 0; i < row; i++) {
            int count = 0, lastJ = -1;
            for (j = 0; j < col; j++) {
                if (temp[i][j]) { count++; lastJ = j; }
            }
            if (count == 1) { temp[i][lastJ] = false; changed = true; } 
        }
        for (j = 0; j < col; j++) {
            int count = 0, lastI = -1;
            for (i = 0; i < row; i++) {
                if (temp[i][j]) { count++; lastI = i; }
            }
            if (count == 1) { temp[lastI][j] = false; changed = true; } 
        }
    } while (changed);
    
    do {
        path[len][0] = r;
        path[len][1] = c;
        len++;
        if (moveHorizontal) {
            for (j = 0; j < col; j++) {
                if (j != c && temp[r][j]) { c = j; break; }
            }
        } else {
            for (i = 0; i < row; i++) {
                if (i != r && temp[i][c]) { r = i; break; }
            }
        }
        moveHorizontal = !moveHorizontal; 
    } while (r != startI || c != startJ); 
    
    *pathLength = len;
    return true;
}

int modiMethod() {
    int choice;
    int initialCost;
    int iteration = 1;
    bool improved;
    int totalCost;
    bool isBasic[MAX][MAX] = {false};
    int i, j, k, r, c;
    bool first;

    setColor(COLOR_HEADER);
    printf("\n=== PHUONG PHAP THE VI (MODI) ===\n");
    setColor(COLOR_DATA);
    
    setColor(COLOR_SUB_HEADER);
    printf("\nChon phuong phap khoi tao nghiem ban dau:\n");
    setColor(COLOR_MENU);
    printf("1. Goc Tay Bac\n");
    printf("2. Chi phi nho nhat\n");
    printf("3. Vogel\n");
    
    setColor(COLOR_PROMPT);
    printf("Lua chon cua ban: ");
    setColor(COLOR_DATA);
    scanf("%d", &choice);
    
    switch (choice) {
        case 1: initialCost = northwestCornerMethod(); break;
        case 2: initialCost = leastCostMethod(); break;
        case 3: initialCost = vogelApproximationMethod(); break;
        default: 
            setColor(COLOR_ERROR);
            printf("Lua chon khong hop le. Su dung Goc Tay Bac mac dinh.\n");
            setColor(COLOR_DATA);
            initialCost = northwestCornerMethod();
    }
    
    setColor(COLOR_HEADER);
    printf("\nBat dau toi uu hoa bang phuong phap The vi...\n");
    setColor(COLOR_DATA);
    
    totalCost = initialCost;
    
    for (i = 0; i < row; i++)
        for (j = 0; j < col; j++)
            if (allocation[i][j] > 0) isBasic[i][j] = true;
    
    do {
        int u[MAX], v[MAX];
        bool calculateMore;
        improved = false;
        
        do {
            bool updated;
            calculateMore = false;
            for (i = 0; i < row; i++) u[i] = INT_MAX;
            for (j = 0; j < col; j++) v[j] = INT_MAX;
            u[0] = 0; 
            
            do {
                updated = false;
                for (i = 0; i < row; i++) {
                    for (j = 0; j < col; j++) {
                        if (isBasic[i][j]) { 
                            if (u[i] != INT_MAX && v[j] == INT_MAX) {
                                v[j] = cost[i][j] - u[i]; updated = true;
                            } else if (v[j] != INT_MAX && u[i] == INT_MAX) {
                                u[i] = cost[i][j] - v[j]; updated = true;
                            }
                        }
                    }
                }
            } while (updated);
            
            for (i = 0; i < row; i++) {
                for (j = 0; j < col; j++) {
                    if (!isBasic[i][j] && ((u[i] != INT_MAX && v[j] == INT_MAX) || (u[i] == INT_MAX && v[j] != INT_MAX))) {
                        isBasic[i][j] = true; calculateMore = true; goto next_calc;
                    }
                }
            }
            next_calc:;
        } while (calculateMore);
        
        setColor(COLOR_STEP);
        printf("\nBuoc %d: Cac the vi\n", iteration);
        setColor(COLOR_DATA);
        
        printf("u[i]: ");
        setColor(COLOR_VALUE);
        for (i = 0; i < row; i++) printf("%d ", u[i]);
        setColor(COLOR_DATA);
        printf("\nv[j]: ");
        setColor(COLOR_VALUE);
        for (j = 0; j < col; j++) printf("%d ", v[j]);
        setColor(COLOR_DATA);
        printf("\n");
        
        int bestI = -1, bestJ = -1;
        int minDelta = 0;
        
        for (i = 0; i < row; i++) {
            for (j = 0; j < col; j++) {
                if (!isBasic[i][j]) { 
                    int delta = cost[i][j] - (u[i] + v[j]);
                    if (delta < minDelta) { minDelta = delta; bestI = i; bestJ = j; }
                }
            }
        }
        
        if (minDelta >= 0) {
            setColor(COLOR_VALUE);
            printf("Khong tim thay Delta am. Nghiem da toi uu.\n");
            setColor(COLOR_DATA);
            break;
        }
        
        setColor(COLOR_DELTA);
        printf("Tim thay Delta[%d][%d] = %d (am nhat)\n", bestI + 1, bestJ + 1, minDelta);
        setColor(COLOR_DATA);
        
        static int path[MAX*MAX][2];
        int pathLength = 0;
        
        if (!findCycle(path, &pathLength, bestI, bestJ, isBasic)) {
            setColor(COLOR_ERROR);
            printf("Khong tim thay chu trinh dieu chinh. Ket thuc.\n");
            setColor(COLOR_DATA);
            break;
        }
        
        int minQty = INT_MAX;
        for (k = 1; k < pathLength; k += 2) {
            int r_idx = path[k][0], c_idx = path[k][1];
            if (allocation[r_idx][c_idx] < minQty) minQty = allocation[r_idx][c_idx];
        }
        
        setColor(COLOR_STEP);
        printf("Chu trinh dieu chinh (%d o): ", pathLength);
        setColor(COLOR_DATA);
        for (k = 0; k < pathLength; k++) {
            printf("S%d-D%d", path[k][0] + 1, path[k][1] + 1);
            if (k < pathLength - 1) printf(" -> ");
        }
        
        printf("\nSo luong dieu chinh: ");
        setColor(COLOR_WARNING);
        printf("%d\n", minQty);
        setColor(COLOR_DATA);
        
        for (k = 0; k < pathLength; k++) {
            int r_idx = path[k][0], c_idx = path[k][1];
            if (k % 2 == 0) allocation[r_idx][c_idx] += minQty;
            else allocation[r_idx][c_idx] -= minQty;
        }
        
        isBasic[bestI][bestJ] = true;
        bool removed = false;
        for (k = 1; k < pathLength; k += 2) {
            int r_idx = path[k][0], c_idx = path[k][1];
            if (!removed && allocation[r_idx][c_idx] == 0) {
                isBasic[r_idx][c_idx] = false; removed = true;
            }
        }
        
        improved = true;
        iteration++;
        
        totalCost = 0;
        for (i = 0; i < row; i++) {
            for (j = 0; j < col; j++) {
                if (allocation[i][j] > 0) totalCost += allocation[i][j] * cost[i][j];
            }
        }
        
        printTable("Bang phan phoi sau dieu chinh", false); 
        
    } while (improved);

    setColor(COLOR_VALUE);
    printf("\n=> TONG CHI PHI TOI UU: T = ");
    first = true;
    for (r = 0; r < row; r++) {
        for (c = 0; c < col; c++) {
            if (allocation[r][c] > 0) {
                if (!first) printf(" + ");
                printf("(%d*%d)", cost[r][c], allocation[r][c]);
                first = false;
            }
        }
    }
    printf(" = %d\n", totalCost);
    setColor(COLOR_DATA);
    
    return totalCost;
}

int main() {
    int choice;
    bool keepRunning = true; 
    
    const char* methodNames[4] = {
        "Goc Tay Bac",
        "Chi phi nho nhat",
        "Vogel",
        "The vi (MODI)"
    };
    
    // Ð?t màu n?n console (0F = n?n den, ch? tr?ng)
    system("color 0F");
    
    drawHeader();

    do {
        int totalSupply = 0, totalDemand = 0;
        int i, j;
        int results[4] = {0}; 
        char filename[100];

        setColor(COLOR_PROMPT);
        printf("\nNhap ten file hoac duong dan: ");
        setColor(COLOR_DATA);
        scanf("%s", filename);
        
        if (!readDataFromFile(filename)) { 
            setColor(COLOR_ERROR);
            printf("-> Khong the doc du lieu. Vui long kiem tra lai ten file!\n");
            setColor(COLOR_DATA);
            continue; 
        }
        
        for (i = 0; i < row; i++) totalSupply += supply[i];
        for (j = 0; j < col; j++) totalDemand += demand[j];
        
        if (totalSupply != totalDemand) {
            setColor(COLOR_ERROR);
            printf("-> Loi: Tong cung (%d) khac tong cau (%d). Bai toan khong can bang, khong the giai.\n", totalSupply, totalDemand);
            setColor(COLOR_DATA);
            continue; 
        }
        
        setColor(COLOR_HEADER);
        printf("\nDu lieu bai toan (%s):\n", filename);
        setColor(COLOR_DATA);
        
        initializeAllocation();
        printTable("Bai toan van tai", true);
        
        do {
            setColor(COLOR_HEADER);
            printf("\nCHON PHUONG PHAP GIAI:\n");
            setColor(COLOR_MENU);
            printf("1. Goc Tay Bac\n");
            printf("2. Chi phi nho nhat\n");
            printf("3. Vogel\n");
            printf("4. The vi (MODI)\n");
            printf("5. So sanh tat ca phuong phap\n");
            printf("6. Chon file khac\n");
            setColor(COLOR_EXIT);
            printf("0. Thoat chuong trinh\n");
            
            setColor(COLOR_PROMPT);
            printf("\nLua chon cua ban: ");
            setColor(COLOR_DATA);
            scanf("%d", &choice);
            
            switch (choice) {
                case 1: results[0] = northwestCornerMethod(); break;
                case 2: results[1] = leastCostMethod(); break;
                case 3: results[2] = vogelApproximationMethod(); break;
                case 4: results[3] = modiMethod(); break;
                case 5: {
                    int bestMethod = -1;
                    int minCost = INT_MAX;
                    
                    setColor(COLOR_COMPARE_TITLE);
                    printf("\n========== SO SANH CAC PHUONG PHAP ==========\n");
                    setColor(COLOR_COMPARE_BORDER);
                    printf("+------------------------------+----------------------+\n");
                    printf("|         Phuong phap          |     Tong chi phi     |\n");
                    printf("+------------------------------+----------------------+\n");
                    setColor(COLOR_DATA);
                    for (i = 0; i < 4; i++) {
                        if (results[i] > 0) {
                            printf("| %-28s |   %10d         |\n", methodNames[i], results[i]);
                        }
                    }
                    setColor(COLOR_COMPARE_BORDER);
                    printf("+------------------------------+----------------------+\n");
                    setColor(COLOR_DATA);
                
                    for (i = 0; i < 4; i++) {
                        if (results[i] > 0 && results[i] < minCost) {
                            minCost = results[i]; bestMethod = i;
                        }
                    }
                    
                    if (bestMethod != -1) {
                        setColor(COLOR_VALUE);
                        printf("\n-> Phuong phap toi uu nhat: %s (Chi phi: %d)\n", methodNames[bestMethod], minCost);
                        setColor(COLOR_DATA);
                    } else {
                        setColor(COLOR_ERROR);
                        printf("\nBan chua chay phuong phap nao de so sanh!\n");
                        setColor(COLOR_DATA);
                    }
                    break;
                }
                case 6:
                    setColor(COLOR_WARNING);
                    printf("\n-> Dong file %s. Chuyen sang chon file moi...\n", filename);
                    setColor(COLOR_DATA);
                    break; 
                case 0:
                    setColor(COLOR_EXIT);
                    printf("-> Dang thoat chuong trinh...\n");
                    setColor(COLOR_DATA);
                    keepRunning = false; 
                    break;
                default:
                    setColor(COLOR_ERROR);
                    printf("Lua chon khong hop le. Vui long chon lai.\n");
                    setColor(COLOR_DATA);
            }
        } while (choice != 6 && choice != 0); 
        
    } while (keepRunning); 
    
    setColor(COLOR_VALUE);
    printf("\nKet thuc chuong trinh. Chuc hai ban bao ve do an thanh cong!\n");
    setColor(COLOR_DATA);
    return 0;
}
