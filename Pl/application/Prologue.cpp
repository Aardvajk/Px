#include "Prologue.h"

#include "common/OpCodes.h"

#include "application/Context.h"

void Prologue::generate(Context &c)
{
    c.ds << OpCode::Op::End;
}
