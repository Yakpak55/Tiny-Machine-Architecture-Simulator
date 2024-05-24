#include <stdio.h>

#define MAXPROGRAMSIZE 1000 //maximum size of the program
#define MAXDATASIZE 1000 //maximum size of data memory

typedef struct { //// Define an instruction as opcoode abd addrewss
    int opCode;
    int address;
} Instruction;

Instruction IM[MAXPROGRAMSIZE]; //instruction memory
int DM[MAXDATASIZE]; //data memory

int IR, PC = 0, A = 0; //instruction register, program Counter, accumulator
int MAR, MDR; //memory address register, memory data register

void fetch() {
    MAR = PC;//puts program counter in the  mr
    PC++;//increment the program counter
    MDR = IM[MAR].opCode * 1000 + IM[MAR].address;
    IR = MDR; //intruction into IR
}

void execute() {
    int opcode = IR / 1000; //takes the opcode from instruction
    int address = IR % 1000; //takes the address from instruction

    switch (opcode) {
        case 1: // LOAD
            MAR = address; //put the address in the memory address register
            MDR = DM[MAR]; //retrieve the value from memory at that address
            A = MDR; //put the value into the accumulator
            break;
        case 2: // ADD
            MAR = address;
            MDR = DM[MAR];
            A += MDR;
            break;
        case 3: // STORE
            MAR = address;
            MDR = A;
            DM[MAR] = MDR;
            break;
        case 4: // SUB
            MAR = address;
            MDR = DM[MAR];
            A -= MDR;
            break;
        case 5: // IN
            printf("Enter a value: "); //asks the user for input
            scanf("%d", &MDR); //takes value and saves it in memory at that address
            A = MDR;
            break;
        case 6: // OUT
            printf("%d\n", A); //prints the valoue in the acc out
            break;
        case 7: // END
            printf("Program complete.\n");
            PC = MAXPROGRAMSIZE; //terminate the program
            break;
        case 8: // JMP
            PC = address;//set the program counter to the specified address
            break;
        case 9: // SKIPZ
            if (A == 0) {
                PC++; //incrementing the program counter if acc is 0
            }
            break;
        default:
            printf("Invalid opcode: %d\n", opcode);
            PC = MAXPROGRAMSIZE; //terminate the program by setting the program counter to the maximum program size
            break;
    }
}

void printState() { //prints whats hppening with the program
    printf("PC = %d | A = %d | DM = [", PC, A);
    for (int i = 0; i < MAXDATASIZE; i++) {
        printf("%d", DM[i]);
        if (i != MAXDATASIZE - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

int main(int argc, char *argv[]) {
    //check that the correct number of arguments are provided
    if (argc != 2) {
        printf("Usage: %s input_file\n", argv[0]);
        return 1;
    }
    
    //reads input file
    FILE* input_file = fopen(argv[1], "r");
    if (input_file == NULL) {
        printf("Unable to open input file: %s\n", argv[1]);
        return 1;
    }

    int opcode, address, i = 0; //starts the Instruction Memory (IM) array
    while (fscanf(input_file, "%d %d", &opcode, &address) != EOF && i < MAXPROGRAMSIZE) {
        IM[i].opCode = opcode;
        IM[i].address = address;
        i++;
    }
    fclose(input_file);

    printf("Assembling Program...\n"); //prints out to user and runs the program
    printf("Program Assembled.\n");
    printf("Run.\n");
    while (PC < MAXPROGRAMSIZE) {
        fetch();
        execute();
    }
    return 0;
}
