#include <stdio.h>
#include <stdint.h>
#include "types.h"
#include "opcode.h"

#define MEM_SIZE             409600                     /* Size in bytes. Divide it by 1024 to get the size in KB */
#define SCREEN_OFFSET             4                     /* Address in memory where the screen data starts */
#define SCREEN_SIZE            7600                     /* Size of the number of bytes in memory that the screen buffer uses */
#define KEYBOARD_START   (SCREEN_OFFSET + SCREEN_SIZE)  /* Address in memory where the keyboard data is held */
#define KEYBOARD_LENGTH           1                     /* Number of bytes that a key uses */
#define STARTING_INDEX            0                     /* Address in memory that points to where you should start */
#define START_INDEX_BYTE_READ     4                     /* How many Bytes should be read to get the address */
#define CLEAR_TO_READ           0x7F                    /* Use this like (NUM & CLEAR_TO_READ) to get be able to read the opcode*/


uint8_t memory[MEM_SIZE];
uint8_t *screen = &memory[SCREEN_OFFSET];
uint8_t *keyboard = &memory[KEYBOARD_START];

REGISTER registers[32]; // Something important to note is that registers[0] is always zero and is not writable to.


INSTRUCTION decode(uint32_t instruction) {
 uint8_t opcode = (instruction & 0x7F);
 if(opcode > 128){
  return;
 }

 if(OPCODETABLE[opcode] == INVALID){
  printf("instruction invalid");
  return;
 }
 struct INSTRUCTION ret_inst;
 if(OPCODETABLE[opcode] == I){
  uint16_t imm = (instruction >> 20) & 0xFFF;
  uint8_t rs1 = (instruction >> 15) & 0x1F;
  uint8_t rd = (instruction >> 7) & 0x1F;
  uint8_t funct3 = (instruction >> 12) & 0x07;

  if(opcode == 0x13){
   switch(funct3){
    case 0:
     /* ADDI */
     ret_inst.instruction = ADDI;
     ret_inst.data.rd = rd;
     ret_inst.data.rs1 = rs1;
     ret_inst.data.m_imm = imm;
     break;
    case 2:
     /* SLTI */
     ret_inst.instruction = SLTI;
     ret_inst.data.rd = rd;
     ret_inst.data.rs1 = rs1;
     ret_inst.data.m_imm = imm;
   }
  }
 }
 if(OPCODETABLE[opcode] == R){
  uint8_t rd = (instruction >> 7) & 0x1F;
  uint8_t funct3 = (instruction >> 12) & 0x07;
  uint8_t rs1 = (instruction >> 15) & 0x1F;
  uint8_t rs2 = (instruction >> 20) & 0x1F;
  uint8_t funct7 = (instruction >> 25) & 0x7F;
 }
 if(OPCODETABLE[opcode] == S){
  uint8_t imm1 = (instruction >> 7) & 0x1F;
  uint8_t imm2 = (instruction >> 25) & 0x7F;
  uint8_t funct3 = (instruction >> 12) & 0x07;
  uint8_t rs1 = (instruction >> 15) & 0x1F;
  uint8_t rs2 = (instruction >> 20) & 0x1F;
 }
 if(OPCODETABLE[opcode] == B){

 }
 if(OPCODETABLE[opcode] == U){

 }
 if(OPCODETABLE[opcode] == J){

 }
 return ret_inst;
}



void initScreen(uint8_t state){
 for(int i = 0; i < SCREEN_SIZE; i++){
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
  }
 } else{
  ret = fread(memory, 1, MEM_SIZE, file);
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
 INSTRUCTION inst = decode(0x2013); // For testing purposes
 printf("Instruction: %d", inst.instruction); // For testing purposes
 printf("SLTI: %d", SLTI); // For testing purposes
}
