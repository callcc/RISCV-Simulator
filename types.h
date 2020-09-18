typedef uint32_t REGISTER;

enum RISCV_TYPE{R, I, S, B, U, J, INVALID};

enum Instruction{
Undefined,
ADDI,
SLTI,
SLTIU,
XORI,
ORI,
ANDI,
SLLI,
SRLI,
SRAI,
LUI,
AUIPC,
JAL,
JALR,
BEQ,
BNE,
BLT,
BGE,
BLTU,
BGEU,
LB,
LH,
LW,
LBU,
LHU,
SB,
SH,
SW,
ADD,
SUB,
SLL,
SLT,
SLTU,
XOR,
SRL,
SRA,
OR,
AND,
FENCE,
ECALL,
EBREAK
};


struct instruction_data {
 uint8_t rd;
 uint8_t rs1;
 uint8_t rs2;
 uint8_t imm_1;
 uint8_t imm_2;
 uint16_t m_imm;
 int32_t l_imm;

};

struct INSTRUCTION{
 uint8_t instruction;
 struct instruction_data data;
};

typedef struct INSTRUCTION INSTRUCTION;
