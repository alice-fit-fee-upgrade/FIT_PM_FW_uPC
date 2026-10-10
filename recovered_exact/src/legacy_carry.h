#ifndef PM_LEGACY_CARRY_H
#define PM_LEGACY_CARRY_H

/* Instruction-string primitive, expanded inside the caller's exact extended
 * ASM statement. No extra call, trampoline or bool materialization is added.
 * The caller retains all original operand constraints and clobbers.
 * C=1 is callee-specific: parser failure, password rejection, or FLASH transfer
 * completion. Other SREG bits are NOT promised preserved. See legacy_abi.md.
 * REFERENCE_C_INFERRED (value/control only, not a GNU ABI implementation):
 *     status = call_private_entry(target);
 *     if (status.carry) goto destination;
 * The complete flag-aware transition model in tests/logical_models/legacy_carry.c is separately tested. */
#define PM_CALL_BRANCH_CARRY_SET_ASM(target, destination) \
    "rcall " target "\n\tbrcs " destination
#define PM_INTEGER_PARSE_OR_BRANCH_ASM(destination) \
    PM_CALL_BRANCH_CARRY_SET_ASM("cli_get_integer", destination)
#endif
