#include "Prologue.h"

#include "common/OpCodes.h"

#include "application/Context.h"

pcx::data_ostream_patch<std::size_t> Prologue::generate(Context &c)
{
    pcx::data_ostream_patch<std::size_t> mp;

    c.ds << OpCode::Op::SetRI << OpCode::Reg::Dx << mp;
    c.ds << OpCode::Op::Call << OpCode::Reg::Dx;

    c.ds << OpCode::Op::End;

    return mp;
}
