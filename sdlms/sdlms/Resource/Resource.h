#pragma once

class Resource
{
public:
             Resource() = default;
    virtual ~Resource() = default;

private:
    unsigned long id;
};
