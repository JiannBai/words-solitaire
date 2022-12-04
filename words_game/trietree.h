#ifndef TRIETREE_H
#define TRIETREE_H

#include <QObject>
#include<QString>
//#include<QChar>
#include<QFile>


struct TrieNode
{
    QChar ch;//该节点存储的字符
    TrieNode *children[27];//
    bool is_word;//是否为一个单词
    qint8 childrenNums;
    TrieNode():ch('#'),is_word(false),childrenNums(0){
        for(int i=0;i<27;++i)
        {
            children[i]=nullptr;
        }
    }

  /*  TrieNode(const TrieNode &other)
    {
        this->ch=other.ch
    }
    bool operator> (const TrieNode &other){return ch>other.ch;}
    bool operator>= (const TrieNode &other){return ch>=other.ch;}
    bool operator== (const TrieNode &other){return ch==other.ch;}
    bool operator< (const TrieNode &other){return ch<other.ch;}
    bool operator<= (const TrieNode &other){return ch<=other.ch;}
    bool operator!= (const TrieNode &other){return ch!=other.ch;}*/
//    TrieNode & operator=(const TrieNode &other)
//    {
//        this->ch=other.ch;
//        this->childrenNums=
//    }
};



class TrieTree : public QObject
{
    Q_OBJECT
public:
    explicit TrieTree(QObject *parent = nullptr);
    explicit TrieTree(const QString &fileName,QObject *parent = nullptr);



    bool insert(QChar *word);
    bool find_string(const QString &s,bool isEliminate=true);
    //找到相同前缀的单词
    std::shared_ptr<QString[]>  findWordsWithSuffix(const QString &prefix,int nums);
    bool delet(const QString &s);
    bool insert(const QString &s);
    QString getPrefixWords(const QString &prefix);
    //随机得到一个单词
    QString get_a_random_word(int d_time=0);


private:
    TrieNode *root;//根节点
    int nums;//叶节点数量
    QString  words_list;//以'#'为分隔符，存储公共前缀单词
    TrieNode* startWith(TrieNode *tempNode,const QString prefix);
    void DFS(const TrieNode* tempRoot,QString prefix);


signals:

};

#endif // TRIETREE_H
