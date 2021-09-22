#include "docx.h"
#include "unzip.h"
CDocx::CDocx()
{
    
}

CDocx::~CDocx()
{

}

CBaseElement* CDocx::getRoot()
{
    return &m_root;
}

bool CDocx::Parse(const char* file)
{
    const char* p = strrchr(file, '/');
    if (p) p++;
    else p = file;

    char path[128] = {0}, filename[128] = {0};
    if (1 != sscanf(p, "%[^.].docx", &path[1])) {
        return false;
    }
    path[0] = '.';

    ZIPENTRY ze; 
	HZIP hz = OpenZip(file, 0);
    GetZipItem(hz, -1, &ze); 

    int count = ze.index;
    for (int i=0; i<count; i++) { 
        GetZipItem(hz, i, &ze);
        sprintf(filename, "%s/%s", path, ze.name);
        UnzipItem(hz, i, filename);
    }

    CloseZip(hz);

    sprintf(filename, "%s/word/document.xml", path);
    bool bLoad = m_fXml.LoadFile(filename);	
    if (!bLoad) return false;

    TiXmlElement* pRoot = m_fXml.RootElement();
    if (!pRoot) return false;

    TiXmlElement* pRels = NULL;
    sprintf(filename, "%s/_rels/document.xml.rels", path);
    TiXmlDocument rels("_rels/document.xml.rels");
    bool bRels = rels.LoadFile();
    if (bRels) {
        pRels = rels.RootElement();
    }
    return ParseNode(pRoot, pRels, &m_root);
}

bool CDocx::ParseNode(TiXmlElement* node, TiXmlElement* rels, CBaseElement* pParent)
{
    if (node->NoChildren()) {
        return false;
    }

    int iMr = 0;
	for(TiXmlElement* pEle = node->FirstChildElement(); pEle != NULL; pEle = pEle->NextSiblingElement()) {
        const char* pValue = pEle->Value();
        CT_TYPE type = CT_B;
        CBaseElement* pNode = NULL;
        if( strcmp(pValue, "m:mr") != 0 ) {
            iMr = 0;
        }
        
        if( strcmp(pValue, "w:p") == 0 ) {
            CParagraph* paragraph = new CParagraph();
            pNode = paragraph;
        }else if( strcmp(pValue, "w:t") == 0 || strcmp(pValue, "m:t") == 0) {
            if (pEle->GetText()) {
                CText* text = new CText();
                text->text = pEle->GetText();
                pNode = text;
            }else{
                continue;
            }
        }else if( strcmp(pValue, "m:oMath") == 0 ) {
            CoMath* math = new CoMath();
            pNode = math;
        }else if( strcmp(pValue, "m:f") == 0 ) {
            CFunction* fun = new CFunction();
            pNode = fun;
        }else if( strcmp(pValue, "m:rad") == 0 ) {
            CRad* rad = new CRad();
            pNode = rad;
        }else if( strcmp(pValue, "m:num") == 0 ) {
            CNum* num = new CNum();
            pNode = num;
        }else if( strcmp(pValue, "m:sup") == 0 ) {
            CSup* sup = new CSup();
            pNode = sup;
        }else if( strcmp(pValue, "m:sub") == 0 ) {
            CSub* sub = new CSub();
            pNode = sub;
        }else if( strcmp(pValue, "m:d") == 0 ) {
            Cd* d = new Cd();
            pNode = d;
        }else if( strcmp(pValue, "m:mr") == 0 ) {
            Cmr* mr = new Cmr(iMr++);
            pNode = mr;
        }else if( strcmp(pValue, "m:den") == 0 ) {
            Cden* den = new Cden();
            pNode = den;
        }else if ( strcmp(pValue, "w:r") == 0 ) {
            CRun* run = new CRun(pEle, rels);
            pParent->addChild(run);
            continue;
        }else if ( strcmp(pValue, "w:tbl") == 0 ) {
            CTable* table = new CTable(pEle);
            pParent->addChild(table);
            continue;
        }else if( (strcmp(pValue, "w:pPr") == 0 && (type = CT_pPr)) 
                || (strcmp(pValue, "m:sSup") == 0 && (type = CM_sSup))
                || (strcmp(pValue, "m:sSub") == 0 && (type = CM_sSub))
                || (strcmp(pValue, "m:e") == 0 && (type = CM_e))
                || ( (strcmp(pValue, "m:rPr") == 0 || strcmp(pValue, "w:rPr") ==0 ) && (type = CT_rPr))
            ) {
            CCommonElement* pComm = new CCommonElement(type);
            pNode = pComm;
        }else if ( strcmp(pValue, "pic:pic") == 0 ) {
            if (!rels) continue;
            CPic* pic = new CPic(pEle, rels);
            pParent->addChild(pic);
            continue;
        }

        if (pNode) {
            pParent->addChild(pNode);
            ParseNode(pEle, rels, pNode);
        }else {
            ParseNode(pEle, rels, pParent);
        }
	}

    return true;
}

