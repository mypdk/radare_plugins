static struct {
	char *name;
	char *size;
} registers[] = {
	"a", "8",
	"pc", "?",
};

static struct {
	char *name;
	char *howto;
} flags[] = {
	"cf", "(result & 0x80) ? ((var_a & 0x80) & (var_b & 0x80)) : ((var_a & 0x80) | (var_b & 0x80));",
	"ovf", "(var_a ^ result) & (var_b ^ result) & 0x80;",
	"zf", "(var_a == 0) ? 0 : 1;",
	"ac", "(((var_a & 0x0f) + (var_b & 0x0f)) & 0x10) ? 1 : 0;",
};

static struct {
	char *name;
	char *address;
	char *comment;
} io_space[] = {
	"flags", "0x00", "ACC Status Flag Register",
	"?", "0x01", "?",
	"sp", "0x02", "Stack Pointer Register",
	"clkmd", "0x03", "Clock Mode Register",
	"inten", "0x04", "Interrupt Enable Register",
	"intrq", "0x05", "Interrupt Request Register",
	"t16m", "0x06", "Timer 16 mode Register",
	"timer16", "?", "Timer 16 value Register", // is there such a thing ?
	"?", "0x07", "?",
	"?", "0x08", "?",
	"tm2b", "0x09", "Timer2 Bound Register",
	"eoscr", "0x0a", "External Oscillator setting Register",
	"?", "0x0b", "?",
	"integs", "0x0c", "Interrupt Edge Select Register",
	"padier", 0x0d", "Port A Digital Input Enable Register",
	"?", "0x0e", "?",
	"?", "0x0f", "?",		
	"pa", "0x10", "Port A Data Registers",
	"pac", "0x11", "Port A Control Registers",
	"paph", "0x12", "Port A Pull-High Registers",
	"?", "0x13", "?",
	"?", "0x14", "?",
	"?", "0x15", "?",
	"?", "0x16", "?",
	"tm2s", "0x17", "Timer2 Scalar Register",
	"?", "0x18", "?",
	"?", "0x19", "?",
	"gpcc", "0x1a", "Comparator Control Register",
	"misc", "0x1b", "MISC Register",
	"tm2c", "0x1c", "Timer2 Control Register",
	"tm2ct", "0x1d", "Timer2 Counter Register",
	"gpcs", "0x1e", "Comparator Selection Register",
	"?", "0x1f", "?", // valid is derived from maximum entry
	"?", NULL, "",
};

static struct {
	char* valid;
} data_mem[] = {
	"0x00 - 0x3F",
};

static struct {
	char* valid;
	char* reserved;
} program_mem[] = {
	"0x000 - 0x3FF", "0x3F0 - 0x3FF",
};

// modified after DDunfield 
// https://www.eevblog.com/forum/blog/eevblog-1144-padauk-programmer-reverse-engineering/msg1976534/#msg1976534

static struct {
	char *op;
	char *name;
	ut8 args;
	char *comment;
} ops[] = {
// memory mapped io i (5-bit)
// nth bit n (3-bit)
// memory address m (4 bit) -> 0 .. 0x0f
// memory address M (6 bit) 
// immediate c (8 bit)
// address k (10 bit)
//"binary representation","mnemonic"(,"num parameters (could be inferred)"),"comment", "pseudo code"
"0000.0000.101i.iiii", "mov a,io", 2, "", "a = io_space[io]; if (io == 0) z = 0; cycle(1);",
"0000.0000.100i.iiii", "mov io,a", 2, "", "io[space] = a; cycle(1);",
"0001.0111.cccc.cccc", "mov a,c", 2, "", "a = c; cycle(1);",
"0000.0111.11MM.MMMM", "mov a,M", 2, "", "a = data_mem[M]; if (io == 0) z = 0; cycle(1);"
"0000.0101.11MM.MMMM", "mov M,a", 2, "", "data_mem[M] = a; cycle(1);",
"0000.0000.110m.mmm1", "ldt16 M", 1, "", "data_mem[m] = timer16; cycle(1);",
"0000.0000.110m.mmm0", "stt16 M", 1, "", "timer16 = data_mem[m]; cycle(1);",
"0000.0000.111m.mmm1", "idxm a,M", 2, "", "a = data_mem[[m]]; cycle(2);",
"0000.0000.111m.mmm0", "idxm M,a", 2, "", "data_mem[[m]] = a; cycle(2);",
"0000.1001.11MM.MMMM", "xch M", 1, "", "temp = a; a = data_mem[M]; data_mem[M] = a; cycle(1);",
"0000.0000.0011.0010", "pushaf", 0, "", "data_mem[sp] = a; sp = sp + 1; data_mem[sp] = flags; sp = sp + 1; cycle(1);",
"0000.0000.0011.0011", "popaf", 0, "", "sp = sp - 1; flags = data_mem[sp]; sp = sp - 1; a = data_mem[sp]; cycle(1);",
"0001.0000.cccc.cccc", "add a,c", 2, "", "a = a + c; all_flags(); cycle(1);",
"0000.0110.00MM.MMMM", "add a,M", 2, "", "a = a + data_mem[M]; all_flags(); cycle(1);",
"0000.0100.00MM.MMMM", "add M,a", 2, "", "data_mem[M] = a + data_mem[M]; all_flags(); cycle(1);",
"0000.0000.0001.0000", "addc a", 1, "", "a = a + cf; all_flags(); cycle(1);",
"0000.0110.10MM.MMMM", "addc a,M", 2, "", "a = a + cf + data_mem[M]; all_flags(); cycle(1);",
"0000.0100.10MM.MMMM", "addc M,a", 2, "", "data_mem[M] = a + cf + data_mem[M]; all_flags(); cycle(1);",
"0001.0001.cccc.cccc", "sub a,c", 2, "", "a = a - c; all_flags(); cycle(1);",
"0000.0110.01MM.MMMM", "sub a,M", 2, "", "a = a - data_mem[M]; all_flags(); cycle(1);",
"0000.0100.01MM.MMMM", "sub M,a", 2, "", "data_mem[M] = a - data_mem[M]; all_flags(); cycle(1);",
"0000.0000.0001.0001", "subc a", 2, "", "a = a - cf; all_flags(); cycle(1);",
"0000.0110.11MM.MMMM", "subc a,M", 2, "", "a = a - cf - data_mem[M]; all_flags(); cycle(1);",
"0000.0100.11MM.MMMM", "subc M,a", 2, "", "memory[M] = a - cf - memory[M]; all_flags(); cycle(1);",
"0000.1001.00MM.MMMM", "inc M", 1, "", "data_mem[M] = data_mem[M] + 1; all_flags(); cycle(1);",
"0000.1001.01MM.MMMM", "dec M", 1, "", "data_mem[M] = data_mem[M] - 1; all_flags(); cycle(1);",
"0000.1001.10MM.MMMM", "clear M", 1, "", "data_mem[M] = 0; cycle(1);",
"0000.0000.0001.1010", "sr a", 1, "", "a = a >> 1; cf; cycle(1);",
"0000.1010.10MM.MMMM", "sr M", 1, "", "data_mem[M] = data_mem[M] >> 1; cf; cycle(1);",
"0000.0000.0001.1100", "src a", 1, "", "data_mem[M] = cf << 8 + data_mem[M] >> 1; cf; cycle(1);",
"0000.1011.00MM.MMMM", "src M", 1, "",
"0000.0000.0001.1011", "sl a", 1, "",
"0000.1010.11MM.MMMM", "sl M", 1, "",
"0000.0000.0001.1101", "slc a", 1, "",
"0000.1011.01MM.MMMM", "slc M", 1, "",
"0000.0000.0001.1110", "swap a", 1, "",
"0001.0100.cccc.cccc", "and a,c", 2, "",
"0000.0111.00MM.MMMM", "and a,M", 2, "",
"0000.0101.00MM.MMMM", "and M,a", 2, "",
"0001.0101.cccc.cccc", "or a,c", 2, "",
"0000.0111.01MM.MMMM", "or a,M", 2, "",
"0000.0101.01MM.MMMM", "or M,a", 2, "",
"0001.0110.cccc.cccc", "xor a,c", 2, "",
"0000.0111.10MM.MMMM", "xor a,M", 2, "",
"0000.0101.10MM.MMMM", "xor M,a", 2, "",
"0000.0000.011i.iiii", "xor io,a", 2, "",
"0000.0000.0001.1000", "not a", 1, "",
"0000.1010.00MM.MMMM", "not M", 1, "",
"0000.0000.0001.1001", "neg a", 1, "",
"0000.1010.01MM.MMMM", "neg M", 1, "",
"0000.1110.nnni.iiii", "set0 io.n", 2, "",
"0000.1111.nnni.iiii", "set1 io.n", 2, "",
"0000.0011.nnn0.mmmm", "set0 M.n", 2, "",
"0000.0011.nnn1.mmmm", "set1 M.n", 2, "",
"0001.0010.cccc.cccc", "ceqsn a,c", 2, "",
"0000.1011.10MM.MMMM", "ceqsn a,M", 2, "",
"0000.1100.nnni.iiii", "t0sn io.n", 2, "",
"0000.1101.nnni.iiii", "t1sn io.n", 2, "",
"0000.0010.nnn0.mmmm", "t0sn M.n", 2, "",
"0000.0010.nnn1.mmmm", "t1sn M.n", 2, "",
"0000.0000.0001.0010", "izsn a", 1, "",
"0000.0000.0001.0011", "dzsn a", 1, "",
"0000.1000.10MM.MMMM", "izsn M", 1, "",
"0000.1000.11MM.MMMM", "dzsn M", 1, "",
"0001.10kk.kkkk.kkkk", "goto k", 1, "",
"0001.11kk.kkkk.kkkk", "call k", 1, "",
"0000.0001.cccc.cccc", "ret c", 1, "",
"0000.0000.0011.1010", "ret", 0, "",
"0000.0000.0011.1011", "reti", 0, "",
"0000.0000.0000.0000", "nop", 0, "",
"0000.0000.0001.0111", "pcadd a", 1, "",
"0000.0000.0011.1000", "engint", 0, "",
"0000.0000.0011.1001", "disgint", 0, "",
"0000.0000.0011.0110", "stopsys", 0, "",
"0000.0000.0011.0111", "stopexe", 0, "",
"0000.0000.0011.0101", "reset", 0, "",
"0000.0000.0011.0000", "wdreset", 0, "",
"0000.0000.0000.0000", "trap", 0, "",
"2222.2222.2222.2222", NULL, 0, "",
};
