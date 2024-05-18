#ifndef STACK_H_SENTRY
#define STACK_H_SENTRY

#include "generic_stack.h"

#define STACK_MAX_SIZE 1000

DECLARE_STACK(int, STACK_MAX_SIZE)
DECLARE_STACK(char, STACK_MAX_SIZE)

#endif
