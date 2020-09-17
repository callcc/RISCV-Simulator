#include <stdio.h>
#include <stdint.h>

#define MEM_SIZE             409600                     /* Size in bytes. Divide it by 1024 to get the size in KB */
#define SCREEN_OFFSET             4                     /* Address in memory where the screen data starts */
#define SCREEN_SIZE            7600                     /* Size of the number of bytes in memory that the screen buffer uses */
#define KEYBOARD_START   (SCREEN_OFFSET + SCREEN_SIZE)  /* Address in memory where the keyboard data is held */
#define KEYBOARD_LENGTH           1                     /* Number of bytes that a key uses */
#define STARTING_INDEX            0                     /* Address in memory that points to where you should start */
#define START_INDEX_BYTE_READ     4                     /* How many Bytes should be read to get the address */
#define CLEAR_TO_READ           0x7F                    /* Use this like (NUM & CLEAR_TO_READ) to get be able to read the opcode*/

typedef uint32_t REGISTER;

uint8_t memory[MEM_SIZE];
uint8_t *screen = &memory[SCREEN_OFFSET];
uint8_t *keyboard = &memory[KEYBOARD_START];

REGISTER registers[32]; // Something important to note is that registers[0] is always zero and is not writable to.

enum RISCV_TYPE{R, I, S, B, U, J, INVALID};

const int OPCODETABLE[128] = {
 /* 0000000 */ INVALID,
 /* 0000001 */ INVALID,
 /* 0000010 */ INVALID,
 /* 0000011 */ I,
 /* 0000100 */ INVALID,
 /* 0000101 */ INVALID,
 /* 0000110 */ INVALID,
 /* 0000111 */ INVALID,
 /* 0001000 */ INVALID,
 /* 0001001 */ INVALID,
 /* 0001010 */ INVALID,
 /* 0001011 */ INVALID,
 /* 0001100 */ INVALID,
 /* 0001101 */ INVALID,
 /* 0001110 */ INVALID,
 /* 0001111 */ I,
 /* 0010000 */ INVALID,
 /* 0010001 */ INVALID,
 /* 0010010 */ INVALID,
 /* 0010011 */ I,
 /* 0010100 */ INVALID,
 /* 0010101 */ INVALID,
 /* 0010110 */ INVALID,
 /* 0010111 */ U,
 /* 0011000 */ INVALID,
 /* 0011001 */ INVALID,
 /* 0011010 */ INVALID,
 /* 0011011 */ INVALID,
 /* 0011100 */ INVALID,
 /* 0011101 */ INVALID,
 /* 0011110 */ INVALID,
 /* 0011111 */ INVALID,
 /* 0100000 */ INVALID,
 /* 0100001 */ INVALID,
 /* 0100010 */ INVALID,
 /* 0100011 */ S,
 /* 0100100 */ INVALID,
 /* 0100101 */ INVALID,
 /* 0100110 */ INVALID,
 /* 0100111 */ INVALID,
 /* 0101000 */ INVALID,
 /* 0101001 */ INVALID,
 /* 0101010 */ INVALID,
 /* 0101011 */ INVALID,
 /* 0101100 */ INVALID,
 /* 0101101 */ INVALID,
 /* 0101110 */ INVALID,
 /* 0101111 */ INVALID,
 /* 0110000 */ INVALID,
 /* 0110001 */ INVALID,
 /* 0110010 */ INVALID,
 /* 0110011 */ R,
 /* 0110100 */ INVALID,
 /* 0110101 */ INVALID,
 /* 0110110 */ INVALID,
 /* 0110111 */ U,
 /* 0111000 */ INVALID,
 /* 0111001 */ INVALID,
 /* 0111010 */ INVALID,
 /* 0111011 */ INVALID,
 /* 0111100 */ INVALID,
 /* 0111101 */ INVALID,
 /* 0111110 */ INVALID,
 /* 0111111 */ INVALID,
 /* 1000000 */ INVALID,
 /* 1000001 */ INVALID,
 /* 1000010 */ INVALID,
 /* 1000011 */ INVALID,
 /* 1000100 */ INVALID,
 /* 1000101 */ INVALID,
 /* 1000110 */ INVALID,
 /* 1000111 */ INVALID,
 /* 1001000 */ INVALID,
 /* 1001001 */ INVALID,
 /* 1001010 */ INVALID,
 /* 1001011 */ INVALID,
 /* 1001100 */ INVALID,
 /* 1001101 */ INVALID,
 /* 1001110 */ INVALID,
 /* 1001111 */ INVALID,
 /* 1010000 */ INVALID,
 /* 1010001 */ INVALID,
 /* 1010010 */ INVALID,
 /* 1010011 */ INVALID,
 /* 1010100 */ INVALID,
 /* 1010101 */ INVALID,
 /* 1010110 */ INVALID,
 /* 1010111 */ INVALID,
 /* 1011000 */ INVALID,
 /* 1011001 */ INVALID,
 /* 1011010 */ INVALID,
 /* 1011011 */ INVALID,
 /* 1011100 */ INVALID,
 /* 1011101 */ INVALID,
 /* 1011110 */ INVALID,
 /* 1011111 */ INVALID,
 /* 1100000 */ INVALID,
 /* 1100001 */ INVALID,
 /* 1100010 */ INVALID,
 /* 1100011 */ B,
 /* 1100100 */ INVALID,
 /* 1100101 */ INVALID,
 /* 1100110 */ INVALID,
 /* 1100111 */ I,
 /* 1101000 */ INVALID,
 /* 1101001 */ INVALID,
 /* 1101010 */ INVALID,
 /* 1101011 */ INVALID,
 /* 1101100 */ INVALID,
 /* 1101101 */ INVALID,
 /* 1101110 */ INVALID,
 /* 1101111 */ J,
 /* 1110000 */ INVALID,
 /* 1110001 */ INVALID,
 /* 1110010 */ INVALID,
 /* 1110011 */ I,
 /* 1110100 */ INVALID,
 /* 1110101 */ INVALID,
 /* 1110110 */ INVALID,
 /* 1110111 */ INVALID,
 /* 1111000 */ INVALID,
 /* 1111001 */ INVALID,
 /* 1111010 */ INVALID,
 /* 1111011 */ INVALID,
 /* 1111100 */ INVALID,
 /* 1111101 */ INVALID,
 /* 1111110 */ INVALID,
 /* 1111111 */ INVALID,
};

void decode(uint32_t instruction) {
 int8_t opcode = instruction & 0x7F;
 if(opcode > 128){
  return 0;
 }


}



void initScreen(uint8_t state){
 //printf("%d\n", state);
 for(int i = 0; i < (SCREEN_OFFSET + SCREEN_SIZE); i++){
  screen[i] = state;
 }
}

void readFile(char *filePath){
 FILE *file;
 file = fopen(filePath, "r");
 long filesize;
 fseek(file, 0, SEEK_END);
 filesize = ftell(file);
 rewind(file);
 size_t ret;

 if(filesize < MEM_SIZE){
  long diff = MEM_SIZE - filesize;
  diff += 1;
  ret = fread(memory, 1, filesize, file);
  for(int i = 1; i <= diff; i++){
   memory[filesize + i] = 0; // Padding it out. Maybe change this later to NOP?
   fclose(file);
  }
 } else{
  ret = fread(memory, 1, MEM_SIZE, file);
  fclose(file);
 }
}



uint32_t getStart(){
 int retVal = 0;
 for(int i = 0; i < START_INDEX_BYTE_READ; i++){
  retVal += ((uint32_t)(memory[STARTING_INDEX + i])) << (8*i);
 }
 return retVal;
}

int main(int argc, char *argv[]){
 if(argc != 2){
  printf("Usage is %s [FILE]\n", argv[0]);
  return 0;
 }
 readFile(argv[1]);
 initScreen(0);
}
