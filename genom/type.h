#ifndef __TYPELIB_TYPE_H__
#define __TYPELIB_TYPE_H__

#include <string>
#include <list>
#include <map>
  
class Type;

/** A field in a struct or union */
class Field
{
    friend class Struct;
    friend class Union;

    std::string m_name;
    const Type* m_type;
    int m_offset;

protected:
    void setOffset(int offset);

public:
    Field(const std::string& name, const Type* base_type);

    std::string getName() const;
    const Type* getType() const;
    int getOffset() const;
};


class Type 
{
public:
    enum Category
    { Array, Pointer, SInt, UInt, Float, Struct, Union };

private:
    std::string m_name;

    size_t m_size;
    Category m_category;
    
    typedef std::map<std::string, Type*> TypeMap;
    static bool    s_init;
    static TypeMap s_typemap;

    const Type* m_next_type;

protected:
    typedef std::list<Field> FieldList;
    FieldList m_fields;

    void setName(const std::string& name);
    void setSize(int size);
    void setNextType(const Type* type);
    virtual void fieldsChanged();

    // Creates a basic type from \c name, \c size and \c category
    Type(const std::string& name, int size, Category category);

public:
    // Creates a type which is the same as \c from, but whose name differs
    Type(const std::string& name, const Type* from);
    virtual ~Type();
    
    std::string  getName() const;
    bool         isSimple() const;
    int          getSize() const;
    Category     getCategory() const;
    
    FieldList  getFields();
    void       addField(const Field& field);
    void       addField(const std::string& name, const Type* type);

    bool          isIndirect() const;
    const Type*   getNextType() const;

    virtual std::string toString(const std::string& prefix = "") const;

    bool operator == (const Type& type) const;
    bool operator != (const Type& type) const;
    
    /** Returns a Type object that describes the given name
     * If the type is not known yet, it tries to build one 
     * using known types
     * WARNING: not mt safe for now
     */
    static const Type* fromName(const std::string& name);
};

class Struct : public Type
{
protected:
    void fieldsChanged();

public:
    Struct(const std::string& name, bool deftype = false);
};

class Union : public Type
{
protected:
    void fieldsChanged();

public:
    Union(const std::string& name, bool deftype = false);
};

class Enum : public Type
{
public:
    Enum(const std::string& name);
};

class Array : public Type
{
    int m_dimension;
    std::string m_basename, m_dim_string;

public:
    Array(const Type* of, int dimension);

    int getDimension() const;
    std::string getBasename() const;
    std::string getDimString() const;
};

class Pointer : public Type
{
public:
    Pointer(const Type* on);
};

#endif

