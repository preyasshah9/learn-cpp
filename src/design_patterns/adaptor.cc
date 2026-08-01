/*
This is an example of the Adapter design pattern.

The Adapter pattern allows existing code to work with a class that exposes an
incompatible interface without modifying either side.

The adapter translates the interface expected by the client into the interface
provided by an existing third-party or legacy component.

In this example:
- JSONexportable and Serializable represent interfaces expected by our application.
- OpenPages represents an existing third-party library with a different API.
- Pages acts as the Adapter, exposing the interfaces our application expects
  while delegating the implementation to OpenPages.
*/

#include <iostream>

struct ByteStream {
    int foo;
};

// Interface expected by the application for JSON export.
class JSONexportable
{
public:
    virtual ~JSONexportable() = default;

    virtual void exportToJson()
    {
        std::cout << "Exporting to JSON" << std::endl;
    }
};

// Interface expected by the application for serialization.
class Serializable 
{
public:
    virtual ~Serializable() = default;

    virtual void serialize(ByteStream& bs /* Unused */)
    {
        std::cout << "Serializing" << std::endl;
    }
};

class Document: public JSONexportable, public Serializable {};

// Existing third-party component.
// We cannot modify this interface, but we want to reuse its functionality.
class OpenPages {
public:
    void convertToBytes()
    {
        std::cout << "OpenPages converting data to bytes" << std::endl;
    }
};

// Existing third-party JSON export function.
void exportToJsonFormat(const OpenPages& pages)
{
    std::cout << "OpenPages exporting data to JSON" << std::endl;
}

// Adapter class.
// It implements the interfaces expected by our application and internally
// delegates operations to the third-party OpenPages implementation.
class Pages : public Document
{
public:
    void exportToJson() override
    {
        exportToJsonFormat(pages);
    }

    void serialize(ByteStream& bs /*, other args*/) override
    {
        pages.convertToBytes();
    }

private:
    OpenPages pages;
};

// An alternate implementation is to use private inheritance:
// class Pages : public JSONexportable, private OpenPages
//
// Private inheritance can be appropriate when the adapter needs direct access
// to protected members of OpenPages or needs to override its virtual behavior.
// However, prefer composition (holding OpenPages as a member) in most cases,
// since it keeps the adapter loosely coupled and clearly expresses a has-a
// relationship.

int main()
{
    Pages pages;
    ByteStream stream;
    pages.exportToJson();
    pages.serialize(stream);

    return 0;
}
