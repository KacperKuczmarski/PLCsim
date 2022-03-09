/*
===================================|
Kacper Kuczmarski	|

30.11.2021          |

PLCsim IL           |

SP	Project			|
===================================|
*/

#include<iostream>
#include<conio.h>
#include<chrono>
#include<windows.h>

using namespace std;
using namespace chrono;

//======= Const Period ========
auto Ts = 100;

bool runningRun = true;

bool running = true;

bool M[10];
bool I[10];
bool Q[10];

bool accumulator;


/*
===================================|
all are 2 ARG (DATA SPACE, NUMBER)	|
===================================|

0 - no instruction
1 - LD_BOOL
2 - ST_BOOL
3 - AND
4 - ANDN
5 - OR 
6 - ORN

===================================|

1 - M
2 - I
3 - Q

===================================|
*/

short program[100] = {0};

int char2int(char instruction) {
	for (int i = 0; i < 9; i++)
		if (instruction == 48 + i) 
			return i;
}
int char2bool(char instruction) {
	if (instruction == 48)
		return false;
	else
		return true;
}

void setInstructionNum(char *instruction, int instructionNum, int programNum){
	program[programNum] = instructionNum;

	cin >> instruction;

	if (instruction[0] == 'M')
		program[programNum + 1] = 1;
	if (instruction[0] == 'I')
		program[programNum + 1] = 2;
	if (instruction[0] == 'Q')
		program[programNum + 1] = 3;

	program[programNum + 2] = char2int(instruction[1]);
}

void programme() {
	char instruction[100];//typed in instruction 

	int num = 0; //present number of line of code

	while (running) {
		cin >> instruction;


		//main run instruction (not working yet)
		if (!strcmp(instruction, "run"))
			running = false;


		//print instructions 
		else if (!strcmp(instruction, "printM"))
			for (int i = 0; i < 9; i++)
				cout << endl << "M[" << i << "] = " << M[i] << endl;

		else if (!strcmp(instruction, "printI"))
			for (int i = 0; i < 9; i++)
				cout << endl << "I[" << i << "] = " << I[i] << endl;

		else if (!strcmp(instruction, "printQ"))
			for (int i = 0; i < 9; i++)
				cout << endl << "Q[" << i << "] = " << Q[i] << endl;

		else if (!strcmp(instruction, "printP"))
			for (int i = 0; i < 20; i++)
				cout << endl << "P[" << i << "] = " << program[i] << endl;

		else if (!strcmp(instruction, "clear"))
			for(int i = 0;i<sizeof(program)/2; i++)
				program[i] = 0;
		
		//main interpreter
		else if (instruction[0] == '%') {
			if (instruction[1] == 'M' && instruction[3] == '=')
				M[char2int(instruction[2])] = char2bool(instruction[4]);

			if (instruction[1] == 'I')
				I[char2int(instruction[2])] = char2bool(instruction[4]);
		}

		//main instructions

		else if (!strcmp(instruction, "LD_BOOL")) {
			setInstructionNum(instruction, 1, num);
			num = num + 3;
		}
		else if (!strcmp(instruction, "ST_BOOL")) {
			setInstructionNum(instruction, 2, num);
			num = num + 3;
		}
		else if (!strcmp(instruction, "AND")) {
			setInstructionNum(instruction, 3, num);
			num = num + 3;
		}
		else if (!strcmp(instruction, "ANDN")) {
			setInstructionNum(instruction, 4, num);
			num = num + 3;
		}
		else if (!strcmp(instruction, "OR")) {
			setInstructionNum(instruction, 5, num);
			num = num + 3;
		}
		else if (!strcmp(instruction, "ORN")) {
			setInstructionNum(instruction, 6, num);
			num = num + 3;
		}

		else
			cout << "Unknown command";
		
		cout << endl;
	}
}

void interpreter() {
	for (int i = 0; i < (sizeof(program) / 2);i++) {
		//new command 
		if (i % 3 == 0) {
			switch (program[i]) {
			//=========|
			//no instruction
			//=========|
			case 0:
				break;
			
			//=========|
			//1 - LD_BOOL	
			//=========|
			case 1:
				switch (program[i + 1]) {
				case 1:
					//Load M 
					accumulator = M[program[i + 2]];
					break;
				case 2:
					//Load I
					accumulator = I[program[i + 2]];
					break;
				case 3:
					//Load Q
					accumulator = Q[program[i + 2]];
					break;
				default:
					cout << "memory error (LD_BOOL)" << i << program[i] << program[i + 1] << program[i + 2] << endl;
					break;
				}
				break;
			
			//=========|
			//2 - ST_BOOL
			//=========|
			case 2:
				switch (program[i + 1]) {
				case 1:
					//Store to M 
					M[program[i + 2]] = accumulator;
					break;
				case 2:
					//Store to I
					I[program[i + 2]] = accumulator;
					break;
				case 3:
					//Store to Q
					Q[program[i + 2]] = accumulator;
					break;
				default:
					cout << "memory error (ST_BOOL)" << i << program[i] << program[i + 1] << program[i + 2] << endl;
					break;
				}
				break;
			
			//=========|
			//3 - AND
			case 3:
				switch (program[i + 1]) {
				case 1:
					//Store to accumulator acc AND M
					accumulator = accumulator & M[program[i + 2]];
					break;
				case 2:
					//Store to accumulator acc AND I
					accumulator = accumulator & I[program[i + 2]];
					break;
				case 3:
					//Store to accumulator acc AND Q
					accumulator = accumulator & Q[program[i + 2]];
					break;
				default:
					cout << "memory error";
					break;
				}
				break;
			
			//=========|	
			//4 - ANDN
			//=========|
			case 4:
				switch (program[i + 1]) {
				case 1:
					//Store to accumulator acc ANDNOT M
					accumulator = !(accumulator & M[program[i + 2]]);
					break;
				case 2:
					//Store to accumulator acc ANDNOT I
					accumulator = !(accumulator & I[program[i + 2]]);
					break;
				case 3:
					//Store to accumulator acc ANDNOT Q
					accumulator = !(accumulator & Q[program[i + 2]]);
					break;
				default:
					cout << "memory error";
					break;
				}
				break;
			
			//=========|
			//5 - OR
			//=========|
			case 5:
				switch (program[i + 1]) {
				case 1:
					//Store to accumulator acc OR M
					accumulator = accumulator | M[program[i + 2]];
					break;
				case 2:
					//Store to accumulator acc OR I
					accumulator = accumulator | I[program[i + 2]];
					break;
				case 3:
					//Store to accumulator acc OR Q
					accumulator = accumulator | Q[program[i + 2]];
					break;
				default:
					cout << "memory error";
					break;
				}
				break;
			
			//=========|	
			//6 - ORN
			//=========|
			case 6:
				switch (program[i + 1]) {
				case 1:
					//Store to accumulator acc ORNOT M
					accumulator = !(accumulator | M[program[i + 2]]);
					break;
				case 2:
					//Store to accumulator acc ORNOT I
					accumulator = !(accumulator | I[program[i + 2]]);
					break;
				case 3:
					//Store to accumulator acc ORNOT Q
					accumulator = !(accumulator | Q[program[i + 2]]);
					break;
				default:
					cout << "memory error";
					break;
				}
				break;
			default:
				cout << "command error";
				break;
			}
				
		}
	}
}

void run(){
	runningRun = true;

	char instruction[100];

	while (runningRun) {
		//start measuring time
		auto start = high_resolution_clock::now();
		
		interpreter();

		for (int i = 0; i < 10; i++) 
			cout << "M[" << i << "] = " << M[i] <<'\t' <<"I[" << i << "] = " << I[i] << '\t' << "Q[" << i << "] = " << Q[i] << endl;
		
		//stop measuring time
		auto stop = high_resolution_clock::now();
		auto time = stop - start;
		auto elapsed = duration_cast<milliseconds>(stop - start);
		cout << "Elapsed time: " << elapsed.count() << "ms" << endl;

		if (_kbhit()) {
			fflush(stdin); //empty _kbhit() buffer
			cin >> instruction;

			if (!strcmp(instruction, "exit"))
				runningRun = false;
			else if (instruction[0] == 'M' && instruction[1] >= 48 && instruction[1] <= 57)
				M[char2int(instruction[1])] = !M[char2int(instruction[1])];
			else if (instruction[0] == 'Q' && instruction[1] >= 48 && instruction[1] <= 57)
				Q[char2int(instruction[1])] = !Q[char2int(instruction[1])];
			else if (instruction[0] == 'I' && instruction[1] >= 48 && instruction[1] <= 57)
				I[char2int(instruction[1])] = !I[char2int(instruction[1])];
			else if (!strcmp(instruction, "return")) {
				running = true;
				system("cls");
				programme();
			}
				

			else
				cout << "unknown command";
			
		}
		//========= constant Ts =========
		Sleep(Ts - elapsed.count());
		system("cls");
	}
	cout << "Thanks for using my program! \nKacper Kuczmarski" ;
}

int main() {
	programme();
	run();
	return 0;
}