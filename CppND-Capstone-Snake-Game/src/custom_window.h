#ifndef CUSTOM_WINDOW_H
#define CUSTOM_WINDOW_H

#include <iostream>

#include <memory>

#include <string>


class pointerClass
{
    std::unique_ptr<float>m_upI;
    std::unique_ptr<bool>m_upS;
    public:
    pointerClass(float i, bool s)
    {
        m_upI.reset(new float(i));
        m_upS.reset(new bool(s));
    }
    ~pointerClass()
     { 
         m_upI.reset(nullptr);
         m_upS.reset(nullptr);
     }
     float getFloat()
     {
         return (*m_upI);
     }
     bool getBool()
     {    return (*m_upS);}
};
        
#endif