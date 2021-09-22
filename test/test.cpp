#include <stdio.h>
#include "docx.h"
#ifdef _LINUX
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
int remove_dir(const char *dir)
{
	char dir_name[256];
	DIR *dirp;
	struct dirent *dp;
	struct stat dir_stat;

	if ( 0 != access(dir, F_OK) ) {
		return 0;
	}

	if ( 0 > stat(dir, &dir_stat) ) {
		printf("Can't get %s stat\n", dir);
		return -1;
	}

	if ( S_ISREG(dir_stat.st_mode) ) {
		remove(dir);
	} else if ( S_ISDIR(dir_stat.st_mode) ) {
		dirp = opendir(dir);
		while ( (dp=readdir(dirp)) != NULL ) {
			// 忽略 . 和 ..
			if ( (0 == strcmp(".", dp->d_name)) || (0 == strcmp("..", dp->d_name)) ) {
				continue;
			}
			
			sprintf(dir_name, "%s/%s", dir, dp->d_name);
			remove_dir(dir_name); 
		}
		closedir(dirp);
		rmdir(dir);
	}else{
		printf("Unknow file(%s) type\n", dir);
	}
	return 0;
}
#endif

int main(int argc, char* argv[])
{
    CDocx docx;
    if ( !docx.Parse(argv[1]) )
        return 0;

    bool bGetExamName = false;
    bool bNextStart = true;
    int iCurrentType = -1;
    CBaseElement* pRoot = docx.getRoot();
    for(CBaseElement* pEle = pRoot->FirstChildNode(); pEle != NULL; pEle = pRoot->NextSiblingNode()) {
        std::string str = pEle->toString();
        if ( str.empty() ) {
            continue;
        }

        if (!bGetExamName) {
            //获取试卷名称
            printf("试卷名称:%s</br>\n", str.c_str());
            bGetExamName = true;
            continue;
        }

        if (bNextStart) {
            if ( (strstr( str.c_str(), "选择题") > 0) || (strstr( str.c_str(), "单选题") > 0) ) {
                iCurrentType = 1;
                continue;
            }else if ( strstr( str.c_str(), "多选题") > 0 ) {
                iCurrentType = 2;
                continue;
            }else if ( strstr( str.c_str(), "填空题") > 0 ) {
                iCurrentType = 3;
                continue;
            }else if ( (strstr( str.c_str(), "简答题") > 0) || (strstr( str.c_str(), "解答题") > 0) ) {
                iCurrentType = 4;
                continue;
            }

            switch(iCurrentType) {
                case 1:printf("选择题:</br>\n");break;
                case 2:printf("多选题:</br>\n");break;
                case 3:printf("填空题:</br>\n");break;
                case 4:printf("简答题:</br>\n");break;
            }

            //解析题号，分数。  模板:22．（7分）
            int id, scl;
            if ( 2 == sscanf(str.c_str(), "%d．（%d分）%*[]", &id, &scl) ) {
                printf("题号:%d</br>\n分数:%d</br>\n题目:%s</br>\n", id, scl, strstr(str.c_str(), "分）")+strlen("分）"));
                bNextStart = false;
            }
            continue;
        }

        if (!bNextStart) {
            const char* p = NULL;
            if ( (p = strstr( str.c_str(), "【答案】")) > 0 ) {
                printf("\n答案:%s", p + strlen("【答案】"));
            }else if ( (p = strstr( str.c_str(), "【考点】")) > 0 ) {
                printf("\n考点:%s", p + strlen("【考点】"));
            }else if ( (p = strstr( str.c_str(), "【分析】")) > 0 ) {
                printf("\n分析:%s", p + strlen("【分析】"));
            }else if ( (p = strstr( str.c_str(), "【解答】")) > 0 ) {
                printf("\n解答:%s", p + strlen("【解答】"));
            }else if ( (p = strstr( str.c_str(), "【难度】")) > 0 ) {
                printf("\n难度:%s", p + strlen("【难度】"));
                bNextStart = true;
                static int iii = 0;
                iii++;
                //if (iii == 22) exit(0);
            }else {
                printf(str.c_str());
            }
            printf("</br>\n");
        }
    }
    return 0;
}
