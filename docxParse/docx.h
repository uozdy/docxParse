#ifndef __DOCX_H__
#define __DOCX_H__
#include <stdio.h>
#include <iostream>
#include <string>

#include "TinyXML/tinyxml.h"
#include "DocxElement.h"

class CDocx 
{
public:
    CDocx();
    ~CDocx();
    CBaseElement* getRoot();
    
    bool Parse(const char* file);
    
protected:
    bool ParseNode(TiXmlElement* node, TiXmlElement* rels, CBaseElement* pParent);
private:
    TiXmlDocument   m_fXml;
    CBaseElement    m_root;
};

#endif
