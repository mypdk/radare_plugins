// Modified after Datasheet of MDT90P01
// http://www.yspringtech.com/Spec/MDT-OTHERS/90P01.pdf

// 0,5k words rom, 30 nibbles ram

static struct {
	char *op;
	char *name;
	ut8 args;
	char *comment;
} ops[] = {
// destination d (1-bit) 0: working register (W), 1: general register
// nth bit n (2-bit)
// memory address f (5 bit)
// immediate c (4 bit)
// address k (9 bit)
//"binary representation","mnemonic"(,"num parameters (could be inferred)"),"comment"
"100.0000.0000", "NOP", "", "No operation",
"100.0000.0001", "CLRWT", "", "Clear Watchdog timer",
"100.0000.0010", "SLEEP", "", "Sleep mode",
"100.0000.0011", "TMODEL", "", "Load W to TMRL register",
"100.0000.0101", "TMODEH", "", "Load W to TMRH register",
"100.1010.cccc", "RTWI", "", "Return, place immediate to W",
"100.0000.0rrr", "CPIO f", "", "Control I/O port register",
"111.100f.ffff", "STWR f", "", "Store W to register",
"1d1.101f.ffff", "LDR f, d", "", "Load register",
"100.1000.cccc", "LDWI c", "", "Load immediate to W",
"1d1.110f.ffff", "ADDWR f, d", "", "Add W and register",
"1d1.111f.ffff", "DECRSZ f, d", "", "Decrement register, skip if zero",
"1d1.001f.ffff", "ANDWR f, d", "", "AND W and register",
"100.0010.cccc", "ANDWI i", "", "AND W and immediate",
"1d1.011f.ffff", "IORWR f, d", "", "Inclusive OR W and register",
"100.0110.cccc", "IORWI i", "", "Inclusive OR W and immediate",
"1d1.010f.ffff", "XORWR f, d", "", "Exclusive OR W and register",
"100.0100.cccc", "XORWI i", "", "Exclusive OR W and immediate",
"1d1.000f.ffff", "RRR f, d", "", "Rotate right register",
"010.0nnf.ffff", "BCR f, b", "", "Bit clear",
"011.0nnf.ffff", "BSR f, b", "", "Bit set",
"010.1nnf.ffff", "BTSC f, b", "", "Bit test, skip if clear",
"011.1nnf.ffff", "BTSS f, b", "", "Bit test, skip if set",
"00k.kkkk.kkkk", "JUMP k", "", "JUMP to address",
"2222.2222.2222.2222", NULL, 0, "",
};
