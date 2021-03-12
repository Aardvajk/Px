#include "Prologue.h"

#include "common/Vm.h"

#include "application/Context.h"

pcx::data_ostream_patch<std::size_t> Prologue::generate(Context &c)
{
    pcx::data_ostream_patch<std::size_t> mp;

    c.ds << Vm::Op::SetRI << Vm::Reg::Dx << mp;
    c.ds << Vm::Op::Call << Vm::Reg::Dx;

    c.ds << Vm::Op::End;

    return mp;
}
