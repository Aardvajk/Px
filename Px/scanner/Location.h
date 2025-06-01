#ifndef LOCATION_H
#define LOCATION_H

class Location
{
public:
    using Id = unsigned;
    using Line = unsigned;
    using Column = unsigned;

    Location() : i(0), n(0), c(0) { }
    Location(Id id, Line line, Column column) : i(id), n(line), c(column) { }

    operator bool() const { return i; }

    Id id() const { return i; }
    Line line() const { return n; }
    Column column() const { return c; }

private:
    Id i;
    Line n;
    Column c;
};

#endif // LOCATION_H
