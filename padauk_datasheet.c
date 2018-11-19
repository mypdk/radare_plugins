/* Padauk (dis)assembler plugin for radare 3.1 git - DocBen 2018
now looking more like a datasheet
*/

#include <r_asm.h>
#include <r_lib.h>
#include <string.h>

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


void bitstring(uint16_t val, char buffer[]) {
	int size = 20;
	buffer[--size] = 0;
	while (size > 0) {
		buffer[--size] = (val % 2 ? '1' : '0');
		if ( size % 5 == 0 && size > 0) buffer[--size] = '.';
		val = val >> 1;
	}
}

int _PADAUKDisass (RAsm *a, RAsmOp *op, const ut8 *buf, int len) {
	int i;
	op->size = 2;
	const char *buf_asm = "unknown";
	char buf_bin[40];
	bitstring(0x100 * buf[0] + buf[1], buf_bin);
	for (i=0; ops[i].name != NULL ; i++) {
		for (int j = 0; j < 20; j++) {
			if (ops[i].op[j] != buf_bin[j]) {
				if (ops[i].op[j] != '0' && ops[i].op[j] != '1') { // treat all letters as dont care
					continue;
				} else {
					break;
				}
			}
			if (j == 19) {
				buf_asm = sdb_fmt ("%s = %s ; %s", buf_bin, ops[i].name, ops[i].comment);
				r_strbuf_set (&op->buf_asm, buf_asm);
				return op->size;
			}
		}
	}
	r_strbuf_set (&op->buf_asm, buf_asm);
	return op->size;
}

int _PADAUKAss (RAsm *a, RAsmOp *op, const char *buf) {
	int i;
	op->size = 2;
	ut16 opbuf = 0x4000;
	const char *buf_hex = "unknown";
	for (i = 0; ops[i].name != NULL; i++) {
		if (!strncmp(ops[i].name, buf, strlen(ops[i].name))) {
			//opbuf = ops[i].op;
			buf_hex = sdb_fmt ("0x%.4X\n", opbuf);
			break;
		}
	}
	r_strbuf_set (&op->buf_hex, buf_hex);
	return op->size;
}

RAsmPlugin r_asm_plugin_padauk = {
        .name = "padauk2",
        .arch = "padauk2",
        .license = "LGPL3",
        .bits = 16,
        .desc = "Padauk (dis)assembler",
        .disassemble = &_PADAUKDisass,
        .assemble = &_PADAUKAss,
};

#ifndef CORELIB
struct r_lib_struct_t radare_plugin = {
        .type = R_LIB_TYPE_ASM,
        .data = &r_asm_plugin_padauk
};
#endif
