#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <string.h>
using namespace std;

#define windows true
#define printTable true

#if windows
	#include <windows.h>
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	#include "setCursor.h"
#else
 	void setcursor(int, int) {} //For if windows is not an included directory
#endif

#define amountCylinders 8 //Static, if you change this you must change the variables: strokeNum, piston, IV, and EV to reflect the initial values for this amount of cylinders
#define TDC     		90 //450 or 90 depending on the stroke (450 @ power & 90 @ intake)
#define BDC    			270 //630 or 270 depending on the stroke (630 @ exhaust & 270 @ compression)

//Intake Valve
#define IO_BTDC 		48
#define IO      		TDC+IO_BTDC //During exhaust [270:90)
#define IC_ABDC 		80
#define IC      		(BDC+360)-IC_ABDC //During compression [630:450) (BDC is 630 in this case, hence +360)
#define Idur			(IO)+(720-(IC))

//Exhaust valve
#define EO_BBDC 		88
#define EO      		BDC+EO_BBDC //During power [450:270)
#define EC_ATDC 		40
#define EC      		TDC-EC_ATDC //During intake [90:1] - [720:630)
#define Edur			(EO)-(EC)

char strokeNames[4] = {'I', 'C', 'P', 'E'};

bool	IV[amountCylinders] 		= {0, 0, 1, 0, 0, 1, 0, 0},
		EV[amountCylinders] 		= {0, 0, 0, 0, 1, 0, 1, 0},
		increasing[amountCylinders] = {0, 0, 0, 1, 1, 0, 1, 1};
int		pistonPos[amountCylinders] 	= {9, 5, 5, 0, 4, 9, 0, 4}, //0-9
		strokeNum[amountCylinders] 	= {2, 2, 0, 1, 3, 0, 3, 1},
		pistonDeg[amountCylinders] 	= {450, 360, 720, 630, 180, 90, 270, 540},
		degInc = 10; //10, 5, 1 because the program uses %20 to find the piston position

void print();
void adjust();

int main(){
	char input;
	bool loop=true;

	if(windows){
		setcursor(0, 0);
		system("pause");
		system("cls");
	}
	else{
		printf("Press any key to continue . . . ");
		getch();
	}

    while(loop){
		setcursor(0, 0);

		if(printTable) print();

		adjust();

		if(kbhit()){
			input = getch();

			switch(input){
				case 'p':
				case 'P':{
					for(int i=0; i<amountCylinders/2; i++)
						for(int j=0; j<12; j++)
							printf("\n");
					printf("Press any key to continue...\r");
					getch();
					printf("                            \r");
					for(int i=0; i<amountCylinders/2; i++)
						for(int j=0; j<12; j++)
							printf("\r\b\r");
				} break;
				case 'q':
				case 'Q':{
					for(int i=0; i<amountCylinders/2; i++)
						for(int j=0; j<12; j++)
							printf("\n");
					printf("Quitting program, press any key to continue...\n");
					getch();
					loop = false;
				} break;
			}
		}
    }

    return 0;
}

void print(){
	for(int i=amountCylinders-1; i>=0; i--){
		if(strokeNum[i]==1        || strokeNum[i]==3)	increasing[i]=true;
		else if(strokeNum[i]==0   || strokeNum[i]==2)	increasing[i]=false;

		if(strokeNum[i-1]==1      || strokeNum[i-1]==3)	increasing[i-1]=true;
		else if(strokeNum[i-1]==0 || strokeNum[i-1]==2)	increasing[i-1]=false;

		if(pistonDeg[i]%20==0){
			if(increasing[i])	pistonPos[i]++;
			else				pistonPos[i]--;
		}

		if(pistonDeg[i-1]%20==0){
			if(increasing[i-1])	pistonPos[i-1]++;
			else				pistonPos[i-1]--;
		}

		printf("   Piston %d (%d, %c)   \t\t\t\t", i+1, pistonDeg[i], strokeNames[strokeNum[i]]);
		printf("   Piston %d (%d, %c)   \n", i, pistonDeg[i-1], strokeNames[strokeNum[i-1]]);

		for(int j=9; j>=0; j--){
			/* Part 1 */
				if(EV[i])				printf("O ");
				else					printf("C ");

				if(pistonPos[i]==j)		printf("|-----------------|");
				else					printf("|                 |");

				if(IV[i])				printf(" O\t\t\t\t");
				else					printf(" C\t\t\t\t");

			/* Part 2 */
				if(IV[i-1])				printf("O ");
				else					printf("C ");

				if(pistonPos[i-1]==j)	printf("|-----------------|");
				else					printf("|                 |");

				if(EV[i-1])				printf(" O\n");
				else					printf(" C\n");
		}

		printf("\n");

		i--;
	}

	for(int j=0; j<amountCylinders; j++) for(int i=0; i<=11; i++) printf("\r\b\r");

	return;
}

void adjust(){
	for(int i=0; i<amountCylinders; i++){
		pistonDeg[i] -= degInc;

		if(pistonDeg[i]<=0){
			pistonDeg[i]=720;
		}
		else if(pistonDeg[i]<=90 || pistonDeg[i]>630){ //Intake
			if(pistonDeg[i]<=EC)	EV[i]=0;

			strokeNum[i] = 0;
		}
		else if(pistonDeg[i]<=270){ //Exhaust
			if(pistonDeg[i]<=IO)	IV[i]=1;

			strokeNum[i] = 3;
		}
		else if(pistonDeg[i]<=450){ //Power
			if(pistonDeg[i]<=EO)	EV[i]=1;

			strokeNum[i] = 2;
		}
		else if(pistonDeg[i]<=630){ //Compression
			if(pistonDeg[i]<=IC)	IV[i]=0;

			strokeNum[i] = 1;
		}
	}

	return;
}
