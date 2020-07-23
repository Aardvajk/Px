#include "Prologue.h"

#include "common/OpCodes.h"

#include "application/Context.h"

pcx::data_ostream_patch<std::size_t> Prologue::generate(Context &c)
{
    pcx::data_ostream_patch<std::size_t> mp;

    c.dm.begin('F', { }, pcx::make_callback(&c, Context::dataPosition));

    c.dm("prologue");
    c.dm("{");

    c.dm("-program prologue");
    c.ds << OpCode::Op::SetRI << OpCode::Reg::Dx << mp;
    c.ds << OpCode::Op::Call << OpCode::Reg::Dx;

    c.dm("-terminate");
    c.ds << OpCode::Op::End;

    c.dm("-end");
    c.dm("}");
    c.dm.setCurrentSize(c.ds.position());

    return mp;
}
