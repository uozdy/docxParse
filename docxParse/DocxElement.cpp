#include "DocxElement.h"

/////////////////////////////////////////////////////
// CBaseElement
/////////////////////////////////////////////////////
CBaseElement::CBaseElement()
{

}

CBaseElement::~CBaseElement() 
{

}

CT_TYPE CBaseElement::getType() 
{
    return CT_B; 
}

bool CBaseElement::hasChildren() 
{
    return m_pChildren.size();
}

const void CBaseElement::addChild(CBaseElement* pNode) 
{
    m_pChildren.push_back(pNode);
}

bool CBaseElement::hasChild(CT_TYPE type) 
{
    std::vector<CBaseElement*>::iterator it = m_pChildren.begin();
    for (; it != m_pChildren.end(); ++it) {
        if ( (*it)->getType() == type ) {
            return true;
        }else if ((*it)->hasChild(type)) {
            return true;
        }
    }
    return false;
}

std::string CBaseElement::toString()
{
    std::string myStr = "";
    std::vector<CBaseElement*>::iterator it = m_pChildren.begin();
    for (; it != m_pChildren.end(); ++it) {
        myStr += (*it)->toString();
    }
    return myStr;
}

CBaseElement* CBaseElement::FirstChildNode()
{
    m_findIt = m_pChildren.begin();
    if (m_findIt != m_pChildren.end()) return *m_findIt;
    else return NULL;
}

CBaseElement* CBaseElement::NextSiblingNode()
{
    ++m_findIt;
    if (m_findIt != m_pChildren.end()) return *m_findIt;
    else return NULL;
}

/////////////////////////////////////////////////////
// CCommonElement
/////////////////////////////////////////////////////
CCommonElement::CCommonElement(CT_TYPE type)
: m_type(type)
{

}

CCommonElement::~CCommonElement()
{

}

CT_TYPE CCommonElement::getType()
{
    return m_type;
}

/////////////////////////////////////////////////////
// CParagraph
/////////////////////////////////////////////////////
CParagraph::CParagraph()
{

}

CParagraph::~CParagraph()
{

}

CT_TYPE CParagraph::getType() 
{ 
    return CT_P; 
}

/////////////////////////////////////////////////////
// CRun
/////////////////////////////////////////////////////
CRun::CRun(TiXmlElement* node, TiXmlElement* rels)
    : m_iVertAlign(0)
{
    makeChild(node, rels);
}

CRun::~CRun()
{

}

CT_TYPE CRun::getType() 
{ 
    return CT_R; 
}

void CRun::makeChild(TiXmlElement* node, TiXmlElement* rels)
{
    for(TiXmlElement* pEle = node->FirstChildElement(); pEle != NULL; pEle = pEle->NextSiblingElement()) {
        const char* pValue = pEle->Value();
        if ( strcmp(pValue, "w:vertAlign") == 0 ) {
            std::string align;
            if (TIXML_SUCCESS == pEle->QueryStringAttribute("w:val", &align)) {
                if (strcmp(align.c_str(), "superscript") == 0) {
                    m_iVertAlign = 1;
                }else if (strcmp(align.c_str(), "subscript") == 0) {
                    m_iVertAlign = 2;
                }
            }
        }else if ( strcmp(pValue, "w:t") == 0 ) {
            if (pEle->GetText()) {
                CText* text = new CText();
                text->text = pEle->GetText();
                addChild(text);
            }
            continue;
        }else if ( strcmp(pValue, "pic:pic") == 0 ) {
            if (!rels) continue;
            CPic* pic = new CPic(pEle, rels);
            addChild(pic);
            continue;
        }

        makeChild(pEle, rels);
    }
}


std::string CRun::toString()
{
    if (m_pChildren.size() <= 0) return "";
    
    if (m_iVertAlign == 1) {
        std::string myStr = "<sup>";
        myStr += CBaseElement::toString();
        myStr += "</sup>";
        return myStr;
    }else if (m_iVertAlign == 2) {
        std::string myStr = "<sub>";
        myStr += CBaseElement::toString();
        myStr += "</sub>";
        return myStr;
    }else {
        return CBaseElement::toString();
    }
}

/////////////////////////////////////////////////////
// CText
/////////////////////////////////////////////////////
CText::CText()
{

}

CText::~CText()
{

}

CT_TYPE CText::getType() 
{ 
    return CT_t; 
}

std::string CText::toString()
{
    return text;
}

/////////////////////////////////////////////////////
// CTable
/////////////////////////////////////////////////////
CTable::CTable(TiXmlElement* node)
{
    makeChild(node);
}

CTable::~CTable()
{

}

CT_TYPE CTable::getType() 
{ 
    return CT_tbl; 
}

void CTable::makeChild(TiXmlElement* node)
{
    for(TiXmlElement* pEle = node->FirstChildElement(); pEle != NULL; pEle = pEle->NextSiblingElement()) {
        const char* pValue = pEle->Value();
        if ( strcmp(pValue, "w:tr") == 0 ) {
            m_strTable += "<tr>";
            makeChild(pEle);
            m_strTable += "</tr>";
        }else if ( strcmp(pValue, "w:tc") == 0 ) {
            m_strTable += "<th>";
            makeChild(pEle);
            m_strTable += "</th>";
        }else if ( strcmp(pValue, "w:t") == 0 ) {
            if (pEle->GetText()) {
                m_strTable += pEle->GetText();
            }
            continue;
        }else{
            makeChild(pEle);
        }
    }
}

std::string CTable::toString()
{
    std::string myStr = "<table>";
    myStr += m_strTable;
    myStr += "</table>";
    return myStr;
}

/////////////////////////////////////////////////////
// CoMath
/////////////////////////////////////////////////////
CoMath::CoMath()
{

}

CoMath::~CoMath()
{

}

CT_TYPE CoMath::getType() 
{ 
    return CT_oMath; 
}

std::string CoMath::toString()
{
    std::string myStr = "`";
    myStr += CBaseElement::toString();
    myStr += "`";
    return myStr;
}

/////////////////////////////////////////////////////
// CFunction
/////////////////////////////////////////////////////
CFunction::CFunction()
{

}

CFunction::~CFunction()
{

}

CT_TYPE CFunction::getType() 
{ 
    return CM_f; 
}

std::string CFunction::toString()
{
    std::string myStr = "";
    if (hasChild(CM_den)) {
        myStr = "\\frac";
    }
    myStr += CBaseElement::toString();
    return myStr;
}

/////////////////////////////////////////////////////
// CRad
/////////////////////////////////////////////////////
CRad::CRad()
{

}

CRad::~CRad()
{

}

CT_TYPE CRad::getType() 
{ 
    return CM_rad; 
}

std::string CRad::toString()
{
    std::string myStr = "\\sqrt{";
    myStr += CBaseElement::toString();
    myStr += "}";
    return myStr;
}

/////////////////////////////////////////////////////
// CSup
/////////////////////////////////////////////////////
CSup::CSup()
{

}

CSup::~CSup()
{

}

CT_TYPE CSup::getType() 
{ 
    return CM_sup; 
}

std::string CSup::toString()
{
    std::string myStr = "^{";
    myStr += CBaseElement::toString();
    myStr += "}";
    return myStr;
}

/////////////////////////////////////////////////////
// CSub
/////////////////////////////////////////////////////
CSub::CSub()
{

}

CSub::~CSub()
{

}

CT_TYPE CSub::getType() 
{ 
    return CM_sub; 
}

std::string CSub::toString()
{
    std::string myStr = "_{";
    myStr += CBaseElement::toString();
    myStr += "}";
    return myStr;
}

/////////////////////////////////////////////////////
// Ce
/////////////////////////////////////////////////////
Ce::Ce()
{

}

Ce::~Ce()
{

}

CT_TYPE Ce::getType() 
{ 
    return CM_e; 
}

std::string Ce::toString()
{
    //std::string myStr = "{";
    //myStr += CBaseElement::toString();
    //myStr += "}";
    //return myStr;
    return CBaseElement::toString();
}

/////////////////////////////////////////////////////
// Cd
/////////////////////////////////////////////////////
Cd::Cd()
{

}

Cd::~Cd()
{

}

CT_TYPE Cd::getType() 
{
    return CM_d; 
}

std::string Cd::toString()
{
    std::string myStr = "{";
    myStr += CBaseElement::toString();
    myStr += ":}";
    return myStr;
}

/////////////////////////////////////////////////////
// Cmr
/////////////////////////////////////////////////////
Cmr::Cmr(int index)
: m_iIndex(index)
{

}

Cmr::~Cmr()
{

}

CT_TYPE Cmr::getType() 
{ 
    return CM_mr; 
}

std::string Cmr::toString()
{
    std::string myStr = "";
    if (m_iIndex) myStr = ",";
    myStr += "(";
    myStr += CBaseElement::toString();
    myStr += ")";
    return myStr;
}

/////////////////////////////////////////////////////
// CNum
/////////////////////////////////////////////////////
CNum::CNum()
{

}

CNum::~CNum()
{

}

CT_TYPE CNum::getType() 
{ 
    return CM_num; 
}

std::string CNum::toString()
{
    std::string myStr = "{";
    myStr += CBaseElement::toString();
    myStr += "}";
    return myStr;
}

/////////////////////////////////////////////////////
// CNum
/////////////////////////////////////////////////////
Cden::Cden()
{

}

Cden::~Cden()
{

}

CT_TYPE Cden::getType() 
{ 
    return CM_den; 
}

std::string Cden::toString()
{
    std::string myStr = "{";
    myStr += CBaseElement::toString();
    myStr += "}";
    return myStr;
}

/////////////////////////////////////////////////////
// CPic
/////////////////////////////////////////////////////
CPic::CPic(TiXmlElement* node, TiXmlElement* rels)
{
    std::string relId;
    if (!findRelsId(node, relId)) {
        m_fImage = "none";
        return;
    }

    for(TiXmlElement* pEle = rels->FirstChildElement(); pEle != NULL; pEle = pEle->NextSiblingElement()) {
        std::string id;
        if (TIXML_SUCCESS == pEle->QueryStringAttribute("Id", &id)) {
            if ( strcmp(id.c_str(), relId.c_str()) == 0 ) {
                if ( TIXML_SUCCESS != pEle->QueryStringAttribute("Target", &m_fImage) ) {
                    m_fImage = "none";
                }
            }
        }
    }
}

CPic::~CPic()
{

}

CT_TYPE CPic::getType() 
{ 
    return CM_pic; 
}

bool CPic::findRelsId(TiXmlElement* node, std::string& id)
{
    for(TiXmlElement* pEle = node->FirstChildElement(); pEle != NULL; pEle = pEle->NextSiblingElement()) {
        if (TIXML_SUCCESS == pEle->QueryStringAttribute("r:embed", &id)) {
            return true;
        }else if ( findRelsId(pEle, id) ){
            return true;
        }
    }
    return false;
}

std::string CPic::toString()
{
    std::string myStr = "<img src=\"";
    myStr += m_fImage;
    myStr += "\" />";
    return myStr;
}