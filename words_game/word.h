#ifndef WORD_H
#define WORD_H

#include <QObject>
#include <memory>
#include<QString>
#include"trietree.h"



struct wordsWithGoal
{
    QString words="";
    int goal=-1;
    int next_words_num=999999;
    bool is_depend_less_word=false;
    bool operator>(const wordsWithGoal &other)
    {
        if(!is_depend_less_word)return this->goal>other.goal;
        else return this->next_words_num>other.next_words_num;

    }

    bool operator<(const wordsWithGoal &other)
    {
        if(!is_depend_less_word)return this->goal<other.goal;
        else return this->next_words_num<other.next_words_num;
    }

    bool operator>=(const wordsWithGoal &other)
    {
        if(!is_depend_less_word)return this->goal>=other.goal;
        else return this->next_words_num>=other.next_words_num;
    }

    bool operator<=(const wordsWithGoal &other)
    {
        if(!is_depend_less_word)return this->goal<=other.goal;
        else return this->next_words_num<=other.next_words_num;
    }
    void operator=(const wordsWithGoal &other)
    {
        this->goal=other.goal;
        this->words=other.words;
        this->is_depend_less_word=other.is_depend_less_word;
        this->next_words_num=other.next_words_num;
    }
};







class word : public QObject
{
    Q_OBJECT
public:
    explicit word(QObject *parent = nullptr);

    //设置单词文本
    void setWordsTxt(const QString &fileName);
    //初始化游戏页面,随机初始化一个单词,未实现
    QString initial_word(int d_time=0);
    //查找某一单词是否存在于词库中，若存在则返回该单词，否则返回空字符串；
    QString find(const QString &word,bool is_useless_words=true);
    //字符串后几位的匹配,按接龙匹配程度返回匹配到的前m个合法单词
    std::shared_ptr<wordsWithGoal[]> words_matching(QString prefix,int num=20);
    //返回该单词和接龙单词的单词重叠部分
    QString get_prefix(const QString &pred_word,const QString & connect_word);
    //设置规则
    void setRule(const bool &is_overlap_goal);
    //计算得分
    int get_goals(QString prifix,QString word);
    //得到该单词的后续单词数
    int get_next_words_num(QString word);
    void setOrderRule(const bool &is_depend_less_words);
    //存储单词的数据结构
    TrieTree *wordsTree=nullptr;
    //接过龙的单词
    TrieTree *UselesswordsTree;


private:



    //是否使用重叠得分规则
    bool is_overlap_goal=false;
    //排序会泽
    bool is_depend_less_words=false;
    //快速排序
    void quick_sort(wordsWithGoal *&connectWords,int begin,int all_words_number);

signals:
    //查找某个单词用于接龙的信号
    void find_input_word();
    //游戏开始信号
    void start_game();

};

#endif // WORD_H
