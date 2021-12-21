#include "SymPrinter.h"

#include "application/Context.h"

#include "syms/Sym.h"

namespace
{

bool hasScope(const Sym *sym)
{
    if(sym->type() == Sym::Type::Func || sym->type() == Sym::Type::Class || sym->type() == Sym::Type::TemplateClass)
    {
        return sym->property("defined").value<bool>();
    }

    return sym->type() == Sym::Type::Namespace || sym->type() == Sym::Type::Scope;
}

void dump(Context &c, int tab, const Sym *sym, std::ostream &os)
{
    auto ts = std::string(std::size_t(tab * 4), ' ');

    os << ts << Sym::toString(sym->type()) << " " << sym->fullname();

    if(auto t = sym->property("type"))
    {
        os << ":" << t.to<Type*>()->description();
    }

    os << "\n";

    if(hasScope(sym))
    {
        os << ts << "{\n";

        for(auto s: sym->children())
        {
            dump(c, tab + 1, s, os);
        }

        os << ts << "}\n";
    }
}

}

void SymPrinter::print(Context &c, const Sym *root, std::ostream &os)
{
    dump(c, 0, root, os);
}
