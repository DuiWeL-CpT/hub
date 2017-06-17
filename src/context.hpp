#ifndef __CONTEXT_HPP
#define __CONTEXT_HPP

class Context
{
private:
    unsigned int _id;
public:
    Context(unsigned int id);
    
    ~Context() = default;

    unsigned int GetId()
    {
        return this->_id;
    }
};

#endif
