#ifndef SYM_H
#define SYM_H


class Sym
{
public:
    enum class Type
    {
        Namespace,
        Func,
        Class,
        Invalid
    };

    Sym();

    static const char *toString(Type v);
};

#endif // SYM_H
