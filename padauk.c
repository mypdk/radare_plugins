/* Padauk (dis)assembler plugin for radare 3.1 git - DocBen 2018 */

#include <r_asm.h>
#include <r_lib.h>
#include <string.h>

static struct {
	ut16 op;
	char *name;
} ops[] = {
	{0x0000, "nop"},
	{0x0060, "addc a"},
	{0x0061, "subc a"},
	{0x0062, "izsn a"},
	{0x0063, "dzsn a"},
	{0x0064, "?"},
	{0x0065, "?"},
	{0x0066, "?"},
	{0x0067, "pcadd a"},
	{0x0068, "not a"},
	{0x0069, "neg a"},
	{0x006c, "src a"},
	{0x006d, "slc a"},
	{0x006a, "sr a"},
	{0x006b, "sl a"},
	{0x006e, "swap a"},
	{0x006f, "?"},
	{0x0070, "wdreset"},
	{0x0071, "?"},
	{0x0072, "pushaf"},
	{0x0073, "popaf"},
	{0x0074, "?"},
	{0x0075, "reset"},
	{0x0076, "stopsys"},
	{0x0077, "stopexe"},
	{0x0078, "engint"},
	{0x0079, "disgint"},
	{0x007b, "reti"},
	{0x007a, "ret"},
	{0x007c, "mul"},
	{0x007d, "?"},
	{0x007e, "?"},
	{0x007f, "?"},
	{0x4000, NULL}};

int _PADAUKDisass (RAsm *a, RAsmOp *op, const ut8 *buf, int len) {
	int i;
	op->size = 2;
	const char *buf_asm = "unknown";
	for (i=0; ops[i].name != NULL; i++) {
		if (ops[i].op == 0x100 * buf[0] + buf[1]) {
			buf_asm = sdb_fmt ("%s", ops[i].name);
			break;
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
			opbuf = ops[i].op;
			buf_hex = sdb_fmt ("0x%.4X\n", opbuf);
			break;
		}
	}
	r_strbuf_set (&op->buf_hex, buf_hex);
	return op->size;
}

RAsmPlugin r_asm_plugin_padauk = {
        .name = "padauk",
        .arch = "padauk",
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
