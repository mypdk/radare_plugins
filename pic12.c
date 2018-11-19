//modified after https://en.wikipedia.org/wiki/PIC_instruction_listings - Baseline core devices (12 bit)

static struct {
	char *op;
	char *name;
	ut8 args;
	char *comment;
} ops[] = {
// direction d (1 bit)
// tri-state register t (2 bit)
// nth bit b (3-bit)
// register bank (3 bit)
// register f (5 bit)
// immediate c (8 bit)
// address k (8 bit)
// address K (9 bit)
//"binary representation","mnemonic"(,"num parameters"), "comment"
"0000.0000.0000.0000", "nop", 0, "No operation (MOVW 0,W)",
"0000.0000.0000.0010", "option", 0, "Copy W to OPTION register",
"0000.0000.0000.0011", "sleep", 0, "Go into standby mode",
"0000.0000.0000.0100", "clrwdt", 0, "Restart watchdog timer",
"0000.0000.0000.01tt", "tris t", 0, "Copy W to tri-state register (f = 1, 2 or 3)",
"0000.0000.0001.0BBB", "movlb k", 1, "Set bank select register to k",
"0000.0000.0001.1110", "return", 0, "Return from subroutine, W unmodified",
"0000.0000.0001.1111", "retfie", 0, "Return from interrupt; return & enable interrupts",
"0000.0000.001f.ffff", "movwf f", 1, "dest ← W",
"0000.0000.01df.ffff", "clr f,d", 2, "dest ← 0, usually written CLRW or CLRF f",
"0000.0000.10df.ffff", "subwf f,d", 2, "dest ← f−W (dest ← f+~W+1)",
"0000.0000.11df.ffff", "decf f,d", 2, "dest ← f−1",
"0000.0001.00df.ffff", "iorwf f,d", 2, "dest ← f | W, logical inclusive or",
"0000.0001.01df.ffff", "andwf f,d", 2, "dest ← f & W, logical and",
"0000.0001.10df.ffff", "xorwf f,d", 2, "dest ← f ^ W, logical exclusive or",
"0000.0001.11df.ffff", "addwf f,d", 2, "dest ← f+W",
"0000.0010.00df.ffff", "movwf f,d", 2, "dest ← f",
"0000.0010.01df.ffff", "comf f,d", 2, "dest ← ~f, bitwise complement",
"0000.0010.10df.ffff", "incf f,d", 2, "dest ← f+1",
"0000.0010.11df.ffff", "decfsz f,d", 2, "dest ← f−1, then skip if zero",
"0000.0011.00df.ffff", "rrf f,d", 2, "dest ← CARRY<<7 | f>>1, rotate right through carry",
"0000.0011.01df.ffff", "rlf f,d", 2, "dest ← F<<1 | CARRY, rotate left through carry",
"0000.0011.10df.ffff", "swapf f,d", 2, "dest ← f<<4 | f>>4, swap nibbles",
"0000.0011.11df.ffff", "incfsz f,d", 2, "dest ← f+1, then skip if zero",
"0000.0100.bbbf.ffff", "bcf f,b", 2, "Clear bit b of f",
"0000.0101.bbbf.ffff", "bsf f,b", 2, "Set bit b of f",
"0000.0110.bbbf.ffff", "btfsc f,b", 2, "Skip if bit b of f is clear",
"0000.0111.bbbf.ffff", "btfss f,b", 2, "Skip if bit b of f is set",
"0000.1000.cccc.cccc", "retlw c", 2, "Set W ← k, then return from subroutine",
"0000.1001.kkkk.kkkk", "call k", 2, "Call subroutine, 8-bit address k",
"0000.101K.KKKK.KKKK", "goto k", 2, "Jump to 9-bit address k",
"0000.1100.cccc.cccc", "movlw c", 2, "W ← c",
"0000.1101.cccc.cccc", "iorlw c", 2, "W ← c | W, bitwise logical or",
"0000.1110.cccc.cccc", "andlw c", 2, "W ← c & W, bitwise and",
"0000.1111.cccc.cccc", "xorlw c", 2, "W ← c ^ W, bitwise exclusive or",
"2222.2222.2222.2222", NULL, 0, "",
};
