/*
This example demonstrates the Bridge design pattern.

The goal is to separate two independent dimensions of change:

1. Document abstraction:
   - Resume
   - Invoice
   - Research Paper
   - Contract

2. Document rendering implementation:
   - PDF
   - HTML
   - Markdown

The Bridge pattern avoids creating a separate class for every combination:

    PDFResume
    HTMLResume
    PDFInvoice
    HTMLInvoice
    ...

Instead, document types generate a format-independent DocumentData model,
which is consumed by independent DocumentGenerator implementations.

Adding a new document type should not require modifying generators.
Adding a new generator should not require modifying documents.
*/

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <ctime>

/*
Represents metadata associated with a document.

This information is independent of the output format.
A PDF generator, HTML generator, etc. should all be able to consume it.
*/
struct DocumentMetadata 
{
    std::string name;
    long size; // Bytes
    std::time_t modified_time;
};

// Represents a logical section within a document.
struct Section
{
    std::string header;
    std::vector<std::string> content;
};

/*
Intermediate representation (IR) of a document.

Document types such as Resume or Invoice convert themselves into this
format-neutral model.
*/
struct DocumentData
{
    std::string title;
    DocumentMetadata metadata;
    std::vector<Section> sections;
};


/*
Implementation hierarchy of the Bridge pattern.

DocumentGenerator defines the interface for rendering a DocumentData model.
Concrete implementations decide HOW the document is generated.
*/
class DocumentGenerator
{
public:
    virtual ~DocumentGenerator() = default;
    virtual void generate(const DocumentData& document_data) = 0;
};

/*
Concrete Bridge implementation.

Responsible only for PDF-specific generation logic.
It should not know whether the source document was a Resume,
Invoice, or another document type.
*/
class PDFDocumentGenerator : public DocumentGenerator
{
public:
    void generate(const DocumentData& document_data) override
    {
        std::cout << "Generating PDF" << std::endl;
    }
};

/*
Concrete Bridge implementation.
Responsible only for HTML-specific generation logic.
*/
class HTMLDocumentGenerator : public DocumentGenerator
{
public:
    void generate(const DocumentData& document_data) override
    {
        std::cout << "Generating HTML" << std::endl;
    }
};


/*
Abstraction side of the Bridge pattern.

Document owns the document-specific behavior and delegates
rendering/export behavior to DocumentGenerator.

The generator can be replaced dynamically, allowing the same
document representation to be exported using different formats.
*/
class Document
{
  public:
    virtual ~Document() = default;

    /*
    Converts the document into a format-independent representation
    and delegates generation to the selected implementation.
    */
    void export_document()
    {
        DocumentData document_data = render_document();
        if (document_generator)
        {
            document_generator->generate(document_data);
        }
    }

    // Allows changing the implementation side of the Bridge.
    void set_document_generator(
        std::unique_ptr<DocumentGenerator> generator)
    {
        document_generator = std::move(generator);
    }

  protected:
    virtual DocumentData render_document() const = 0;

  private:
    // Bridge connection between abstraction and implementation.
    std::unique_ptr<DocumentGenerator> document_generator;
};

class ResumeDocument: public Document
{
    protected:
        DocumentData render_document() const override
        {
            std::cout << "Rendering Resume" << std::endl;
            DocumentData dd; 
            dd.title = "Resume";
            return dd;
        }
};
