static struct {
	char *op;
	char *name;
	ut8 args;
	char *comment;
} ops[] = {
// memory mapped io i (6-bit)
// nth bit n (3-bit)
// memory address m (6 bit)
// memory address M (7 bit)
// immediate c (8 bit)
// address k (11 bit)
//"binary representation","mnemonic"(,"num parameters (could be inferred)"),"comment"
"0000.0000.0000.0000", "nop", 0, "does nothing (tm)",
"0000.0000.0110.0000", "addc a", 1, "A ← A + CF",
"0000.0000.11ii.iiii", "xor io,a", 2, "IO ← IO ^ A",
"0000.0001.10ii.iiii", "mov io,a", 2, "IO ← A",
"0000.0001.11ii.iiii", "mov a,io", 2, "A ← IO",
"0000.0010.cccc.cccc", "ret c", 1, "",
"0000.0011.1MMM.MMM0", "idxm M, a", 2, "[M] ← A (last bit of M set to 0, M must be word aligned, 2 cycles)",
"0000.0011.1MMM.MMM1", "idxm a, M", 2, "a ← [M] (last bit of M set to 1, M must be word aligned, 2 cycles)",
"0000.010n.nnii.iiii", "swapc io.n", 2, "",
"0000.0110.0MMM.MMMM", "comp a, M", 2, "",
"0010.000n.nnmm.mmmm", "t0sn m.n", 2, "",
"0011.0kkk.kkkk.kkkk", "goto k", 1, "goto address",
"0011.1kkk.kkkk.kkkk", "call k", 1, "call address",
"2222.2222.2222.2222", NULL, 0, "",
};
