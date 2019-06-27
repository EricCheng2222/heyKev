//
//  symbolTable.h
//  
//
//  Created by Eric Cheng on 5/5/19.
//




#ifndef symbolTable_h
#define symbolTable_h

#include <vector>
#include <string>
#include <string.h>
#include <sstream>
using namespace std;

int const variableKind  = 0;
int const parameterKind = 1;
int const functionKind  = 2;
int const constantKind  = 3;


int const floatType = -100;
int const doubleType = -101;
int const intType = -102;
int const stringType = -103;
int const boolType = -104;
int const voidType = -105;


struct instance {
    instance(char* n, int knd, int tpe, vector<int> &depth){
        strcpy(name, n);
        kind = knd;
        type = tpe;
        for(int i=0; i<depth.size(); i++)
            typeDepth.push_back(depth[i]);
    }
    instance(char* n, int knd, int tpe, vector<instance> &ins){
        strcpy(name, n);
        kind = knd;
        type = tpe;
        for(int i=0; i<ins.size(); i++)
            paraAttr.push_back(ins[i].type);
    }
    instance(char* n, int knd, int tpe){
        strcpy(name, n);
        kind = knd;
        type = tpe;
    }
    char name[32];
    int kind = -1;
    int type = -1;
    vector<int> typeDepth;
    vector<int> paraAttr;
    vector<double> val;
    
    void print(int level){
        char k[32], l[32], tp[32], attr[32];
        //printf("enter print ");
        //kind output
        if(kind==variableKind)          strcpy(k, "variable\0");
        else if(kind==parameterKind)    strcpy(k, "parameter\0");
        else if(kind==functionKind)     strcpy(k, "function\0");
        else if(kind==constantKind)     strcpy(k, "constant\0");
        //printf("KO ");
        
        //level output
        string levelString;
        stringstream ss;
        ss << level;
        levelString = ss.str();
        if(level==0) strcpy(l, "0(global)\0");
        else {
            strcpy(l, levelString.c_str());
            strcat(l, "(local)\0");
        }
        //printf("LO ");
        ss.str("");
        //type output
        if(type == floatType)       strcpy(tp, "float\0");
        else if(type == doubleType) strcpy(tp, "double\0");
        else if(type == intType)    strcpy(tp, "int\0");
        else if(type == stringType) strcpy(tp, "string\0");
        else if(type == boolType)   strcpy(tp, "bool\0");
        else if(type == voidType)   strcpy(tp, "void\0");
        //type array
        string d;
        for(int i=0; i<typeDepth.size(); i++){
            ss.str("");
            strcat(tp, "[");
            ss << typeDepth[i];
            d = ss.str();
            strcat(tp, d.c_str());
            strcat(tp, "]");
        }
        //printf("TO ");
        ss.str("");
        strcpy(attr, "");
        //attr
        if(kind==constantKind){
            if(type==intType)
                ss << (int)val[0];
            else
                ss << val[0];
        }
        else if(kind == functionKind){
            for(int i=0 ;i<paraAttr.size(); i++){
                if(i!=0) strcat(attr, ",");
                if(paraAttr[i]==intType) strcat(attr, "int");
                else if(paraAttr[i]==floatType) strcat(attr, "float");
                else if(paraAttr[i]==doubleType) strcat(attr, "double");
                else if(paraAttr[i]==stringType) strcat(attr, "string");
                else if(paraAttr[i]==boolType)  strcat(attr, "bool");
            }
        }
        //printf("AO ");
        printf("%-32s %-11s%-12s%-19s%s\n",name,k,l,tp,attr);
    }
};

class symbolTable {
public:
    symbolTable(int l){
        level = l;
    }
    void addInstance(instance ins){
        bool exist=false;
        for (int i=0; i<instanceList.size(); i++) {
            if(strcmp(ins.name, instanceList[i].name)==0){
                exist = true;
                
                printf("##########Error at Line #N: Duplicate variable name %s.##########\n", ins.name);
            }
        }
        if(exist == false)
            instanceList.push_back(ins);
    }
    void print(){
        if(instanceList.size()==0) return;
        printf("=======================================================================================\n");
        printf("Name                             Kind       Level       Type               Attribute   \n");
        printf("---------------------------------------------------------------------------------------\n");
        //printf("Instance List Size:%d\n", instanceList.size());
        for(int i=0; i<instanceList.size(); i++)
            instanceList[i].print(level);
        printf("=======================================================================================\n");
    }
    bool doesExist(char* insName){
        bool existRTN = false;
        for (int i=0; i<instanceList.size(); i++) {
            if(strcmp(insName, instanceList[i].name)==0){
                existRTN = true;
                break;
            }
        }
        return existRTN;
    }
    int dimension(char* name){
        int rtnDim = -1;
        for (int i=0; i<instanceList.size(); i++) {
            if(strcmp(name, instanceList[i].name)==0){
                rtnDim = instanceList[i].typeDepth.size();
                break;
            }
        }
        return rtnDim;
    }
    int dataType(char* name){
        int rtnType = -1;
        for (int i=0; i<instanceList.size(); i++) {
            if(strcmp(name, instanceList[i].name)==0){
                rtnType = instanceList[i].type;
                break;
            }
        }
        return rtnType;
    }
    int dataKind(char* name){
        int rtnKind = -1;
        for (int i=0; i<instanceList.size(); i++) {
            if(strcmp(name, instanceList[i].name)==0){
                rtnKind = instanceList[i].kind;
                break;
            }
        }
        return rtnKind;
    }
    
    int paraAttrCntForFunction(char* name){
        for (int i=0; i<instanceList.size(); i++) {
            if(strcmp(name, instanceList[i].name)==0){
                return instanceList[i].paraAttr.size();
            }
        }
        return 0;
    }
private:
    int level;
    vector<instance> instanceList;
};



#endif /* symbolTable_h */
