#ifndef __DOCXELEMENT_H__
#define __DOCXELEMENT_H__
#include <iostream>
#include <vector>

#include "TinyXML/tinyxml.h"

typedef enum{
    CT_B,
    CT_P,
    CT_pPr,
    CT_R,
    CT_rPr,
    CT_oMath,
    CT_t,
    CT_tbl,
    CM_f,
    CM_rad,
    CM_den,
    CM_sSup,
    CM_sup,
    CM_sSub,
    CM_sub,
    CM_e,
    CM_d,
    CM_mr,
    CM_num,
    CM_pic,
}CT_TYPE;

class CBaseElement
{
public:
    CBaseElement();
    virtual ~CBaseElement();

    virtual CT_TYPE getType();
    virtual bool hasChildren();
    virtual const void addChild(CBaseElement* pNode);
    virtual bool hasChild(CT_TYPE type);
    virtual std::string toString();

    virtual CBaseElement* FirstChildNode(); 
    virtual CBaseElement* NextSiblingNode();

protected:
    std::vector<CBaseElement*>  m_pChildren;
    std::vector<CBaseElement*>::iterator    m_findIt;
};

class CCommonElement: public CBaseElement
{
public:
    CCommonElement(CT_TYPE type);
    ~CCommonElement();
    virtual CT_TYPE getType();
private:
    CT_TYPE m_type;
};

class CParagraph: public CBaseElement
{
public:
    CParagraph();
    ~CParagraph();
    virtual CT_TYPE getType();
};

class CRun: public CBaseElement
{
public:
    CRun(TiXmlElement* node, TiXmlElement* rels);
    ~CRun();
    virtual CT_TYPE getType();
    virtual std::string toString();
private:
    void makeChild(TiXmlElement* node, TiXmlElement* rels);
private:
    int     m_iVertAlign;
};

class CText: public CBaseElement
{
public:
    CText();
    ~CText();
    virtual CT_TYPE getType();
    virtual std::string toString();
public:
    std::string     text;
};

class CTable: public CBaseElement
{
public:
    CTable(TiXmlElement* node);
    ~CTable();
    virtual CT_TYPE getType();
    virtual std::string toString();
private:
    void makeChild(TiXmlElement* node);

private:
    std::string     m_strTable;
};

class CoMath: public CBaseElement
{
public:
    CoMath();
    ~CoMath();
    virtual CT_TYPE getType();
    virtual std::string toString();
};

class CFunction: public CBaseElement
{
public:
    CFunction();
    ~CFunction();
    virtual CT_TYPE getType();
    virtual std::string toString();
};

class CRad: public CBaseElement
{
public:
    CRad();
    ~CRad();
    virtual CT_TYPE getType();
    virtual std::string toString();
};

class CSup: public CBaseElement
{
public:
    CSup();
    ~CSup();
    virtual CT_TYPE getType();
    virtual std::string toString();
};

class CSub: public CBaseElement
{
public:
    CSub();
    ~CSub();
    virtual CT_TYPE getType();
    virtual std::string toString();
};

class Ce: public CBaseElement
{
public:
    Ce();
    ~Ce();
    virtual CT_TYPE getType();
    virtual std::string toString();
};

class Cd: public CBaseElement
{
public:
    Cd();
    ~Cd();
    virtual CT_TYPE getType();
    virtual std::string toString();
};

class Cmr: public CBaseElement
{
public:
    Cmr(int index);
    ~Cmr();
    virtual CT_TYPE getType();
    virtual std::string toString();
private:
    int m_iIndex;
};

class CNum: public CBaseElement
{
public:
    CNum();
    ~CNum();
    virtual CT_TYPE getType();
    virtual std::string toString();  
};

class Cden: public CBaseElement
{
public:
    Cden();
    ~Cden();
    virtual CT_TYPE getType();
    virtual std::string toString();
};

class CPic: public CBaseElement
{
public:
    CPic(TiXmlElement* node, TiXmlElement* rels);
    ~CPic();
    virtual CT_TYPE getType();
    virtual std::string toString();  

private:
    bool findRelsId(TiXmlElement* node, std::string& id);

private:
    std::string m_fImage;
};

#endif