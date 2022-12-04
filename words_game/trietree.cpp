#include "trietree.h"
#include<QDebug>
#include<cstdlib>
#include<time.h>
#include <memory>
#include<QString>

TrieTree::TrieTree(QObject *parent) : QObject(parent)
{
    root=new TrieNode();
}

TrieTree::TrieTree(const QString &fileName,QObject *parent) : QObject(parent),root(new TrieNode),nums(0),words_list("")
{


    QString wordsPath=fileName;
    //读取单词文件,相对路径
    QFile wordsFile(wordsPath);
    //打开文件(只读模式）
    wordsFile.open(QFileDevice::ReadOnly);
    //读取文件内容，一行一行读
    QString wordsArray;
    int charToInt;
    TrieNode *tempNode=nullptr;

    if(wordsFile.isOpen())
    {
        while(!wordsFile.atEnd())//读取文件中的每一行
        {

            wordsArray=QString(wordsFile.readLine());
            //this->insert(wordsArray);
            tempNode=root;

            for(QChar c:wordsArray)//遍历每一个单词构建字典树
            {
                charToInt=c.toLatin1();//应该不会改变c的类型

                if(charToInt>96 && charToInt<123)//小写字母
                {
                    //qchar无法直接转为int 或者qint
                    if(tempNode->children[charToInt-97]==nullptr)
                    {
                        TrieNode *newTrieNode=new TrieNode;
                        newTrieNode->ch=c;
                        tempNode->children[charToInt-97]=newTrieNode;//将新节点连接在当前节点对应的指针上
                        ++(tempNode->childrenNums);//该节点的孩子数+1
                        tempNode=newTrieNode;//孩子结点变为当前节点
                        newTrieNode=nullptr;
                    }
                    else
                    {
                        tempNode=tempNode->children[charToInt-97];
                    }

                }


                else if(charToInt>64 && charToInt<91)
                {
                    if(tempNode->children[charToInt-65]==nullptr)
                    {
                        TrieNode *newTrieNode=new TrieNode;
                        newTrieNode->ch=c;
                        tempNode->children[charToInt-65]=newTrieNode;//将新节点连接在当前节点对应的指针上
                        ++(tempNode->childrenNums);//该节点的孩子数+1
                        tempNode=newTrieNode;//孩子结点变为当前节点
                        newTrieNode=nullptr;
                    }
                    else
                    {
                        tempNode=tempNode->children[charToInt-65];
                    }
                }


                else if(charToInt==45)//连接符'-'
                {
                    if(tempNode->children[26]==nullptr)
                    {
                        TrieNode *newTrieNode=new TrieNode;
                        newTrieNode->ch=c;
                        tempNode->children[26]=newTrieNode;//将新节点连接在当前节点对应的指针上
                        ++(tempNode->childrenNums);//该节点的孩子数+1
                        tempNode=newTrieNode;//孩子结点变为当前节点
                        newTrieNode=nullptr;
                    }
                    else
                    {
                        tempNode=tempNode->children[26];
                    }
                }

            }
            tempNode->is_word=true;
            ++this->nums;
        }


    }

    else
    {
        qDebug()<<"open fail!"<<'\n';
    }

    wordsFile.close();

}

//查找某个单词是否在字典树中,isEliminate表示是否提出该单词，默认为真
bool TrieTree::find_string(const QString &s,bool isEliminate)
{
    TrieNode *tempNode=root;
    char ch;

    for(QChar c:s)
    {
        ch=c.toLatin1();
        //若当前节点为空，则该单词一定没有
        if(tempNode==nullptr) return false;

        //若不为空
        else if(ch>='a' && ch<='z')
        {
            //若当前节点的下一节点不为空，但此节点该字符为'#'
            if(tempNode->children[ch-'a']!=nullptr && tempNode->children[ch-'a']->ch=='#')
            {
                return false;
            }
            //将当前节点下移
            tempNode=tempNode->children[ch-'a'];
        }

        else if(ch>='A' && ch<='Z')
        {
            if(tempNode->children[ch-'A']!=nullptr && tempNode->children[ch-'A']->ch=='#')
            {
                return false;
            }
            tempNode=tempNode->children[ch-'A'];
        }
        else if(ch=='-')
        {
            if(tempNode->children[26]!=nullptr && tempNode->children[26]->ch=='#')
            {
                return false;
            }
            tempNode=tempNode->children[26];
        }

    }
    //判断结尾节点是否为一个单词的末尾
    if(tempNode!=nullptr && tempNode->is_word==true)
    {
       if(isEliminate) tempNode->is_word=false;
       return true;
    }
    return  false;
}


//若找到，返回最后一个字母的节点
TrieNode* TrieTree::startWith(TrieNode *tempNode,const QString prefix)
{
    //将指针移到前缀的最后一个字符上
    char ch;
    for(QChar c:prefix)
    {
        ch=c.toLatin1();
        if(tempNode!=nullptr && ch>='a' && ch<='z')
        {

            tempNode=tempNode->children[ch-'a'];
        }
        else if(tempNode!=nullptr && ch>='A' && ch<='Z' )
        {
            tempNode=tempNode->children[ch-'A'];
        }
        else if(tempNode!=nullptr && ch=='-' )
        {
            tempNode=tempNode->children[26];
        }
        else
        {
            qDebug()<<"无该前缀，查询失败！"<<'\n';
            tempNode=nullptr;
            return tempNode;
        }

    }
    return tempNode;
}

//传入一个节点，以该节点为根节点做深度遍历,找到所有公共前缀的单词，存入words_list
//贼难写这里
void TrieTree::DFS(const TrieNode* tempRoot,QString prefix)
{

    if(tempRoot==nullptr) return;
    //每个单词以‘#‘为分隔符
    if(tempRoot->is_word)
    {
        this->words_list.push_back(prefix+'#');
    }

    //若为叶节点，则返回
    if(tempRoot->childrenNums==0) return;

    //从根节点往下深度优先遍历
    for(int i=0;i<27;++i)
    {
        if(tempRoot->children[i]!=nullptr)
        {
            //深度优先遍历
            DFS(tempRoot->children[i],prefix+tempRoot->children[i]->ch);
        }
    }

}

//返回该前缀的所有单词，以字符串的形式，各单词以'#'分隔
QString TrieTree::getPrefixWords(const QString &prefix)
{
    //每次开始清空字符串单词
    this->words_list=NULL;
    TrieNode *tempNode=root;
    tempNode=startWith(tempNode,prefix);
    if(tempNode==nullptr)
    {
        return words_list;
    }

        ;//没有该前缀开头的单词，返回空
    DFS(tempNode,prefix);
    return this->words_list;
}




//插入单词
bool TrieTree::insert(const QString &s)
{
    TrieNode *tempNode = root;
    if(find_string(s,false))
    {
        qDebug()<<"该字符串已经存在！"<<'\n';
        return 0;
    }
    else
    {
        int charToInt;
        for(QChar c:s)//遍历每一个单词构建字典树
        {
            charToInt=c.toLatin1();//应该不会改变c的类型

            if(charToInt>96 && charToInt<123)//小写字母
            {
                //qchar无法直接转为int 或者qint
                if(tempNode->children[charToInt-97]==nullptr)
                {
                    TrieNode *newTrieNode=new TrieNode;
                    newTrieNode->ch=c;
                    tempNode->children[charToInt-97]=newTrieNode;//将新节点连接在当前节点对应的指针上
                    ++(tempNode->childrenNums);//该节点的孩子数+1
                    tempNode=newTrieNode;//孩子结点变为当前节点
                    newTrieNode=nullptr;
                }
                else
                {
                    tempNode=tempNode->children[charToInt-97];
                }

            }


            else if(charToInt>64 && charToInt<91)
            {
                if(tempNode->children[charToInt-65]==nullptr)
                {
                    TrieNode *newTrieNode=new TrieNode;
                    newTrieNode->ch=c;
                    tempNode->children[charToInt-65]=newTrieNode;//将新节点连接在当前节点对应的指针上
                    ++(tempNode->childrenNums);//该节点的孩子数+1
                    tempNode=newTrieNode;//孩子结点变为当前节点
                    newTrieNode=nullptr;
                }
                else
                {
                    tempNode=tempNode->children[charToInt-65];
                }
            }


            else if(charToInt==45)//连接符'-'
            {
                if(tempNode->children[26]==nullptr)
                {
                    TrieNode *newTrieNode=new TrieNode;
                    newTrieNode->ch=c;
                    tempNode->children[26]=newTrieNode;//将新节点连接在当前节点对应的指针上
                    ++(tempNode->childrenNums);//该节点的孩子数+1
                    tempNode=newTrieNode;//孩子结点变为当前节点
                    newTrieNode=nullptr;
                }
                else
                {
                    tempNode=tempNode->children[26];
                }
            }

        }
        tempNode->is_word=true;
        ++this->nums;
        return 1;
    }

}

//大小写问题，存进去有的本该大写，但是与小写的为公共前缀
bool TrieTree::delet(const QString &s)
{
    TrieNode *tempNode = root;
    char ch;
    for(QChar c:s)
    {
        ch=c.toLatin1();
        if(tempNode!=nullptr && ch>='a' && ch<='z')
        {
            tempNode=tempNode->children[ch-'a'];
        }
        else if(tempNode!=nullptr && ch>='A' && ch<='Z' )
        {
           tempNode=tempNode->children[ch-'A'];
        }
        else if(tempNode!=nullptr && ch=='-' )
        {
            tempNode=tempNode->children[26];
        }
        else
        {
            qDebug()<<"无该单词，删除失败！"<<'\n';
            return false;
        }

    }
    //最后的结点若是单词，则删除
    if(tempNode->is_word==true) tempNode->is_word=false;
    else return false;

    return true;
}


QString TrieTree::get_a_random_word(int d_time)
{
    //随机种子

    srand(time(NULL)+d_time);
    TrieNode *tempNode=root;
    QString word;
    while(!tempNode->is_word)
    {
        int random_number=rand()%26;;
        if(tempNode->children[random_number] && tempNode->children[random_number]->ch!='#')
        {
            word.push_back(tempNode->children[random_number]->ch);
            tempNode=tempNode->children[random_number];
        }

    }

    return word;

}

//int main()
//{

//    QString fileName=":/words/CET6.txt";
//    TrieTree t(fileName);
//    QString s="apple";
//    qDebug()<<"查找"<<s<<'\t'<<t.find_string(s);
//    QString prefix="A";
//    qDebug()<<t.getPrefixWords(prefix);
//    qDebug()<<"删除"<<s<<'\t'<<t.delet(s);
//    prefix="ab";
//    QString s0=t.getPrefixWords(prefix);
//    prefix="bcc";
//    QString s2=t.getPrefixWords(prefix);
//    qDebug()<<s0<<'\n';
//    qDebug()<<s2<<'\n';
//    qDebug()<<t.insert("A");//有问题
//    qDebug()<<t.find_string("A");


//}
























