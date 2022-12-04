#include "word.h"
#include"trietree.h"
#include<QDebug>
#include<QVector>
word::word(QObject *parent) : QObject(parent),UselesswordsTree(new TrieTree),is_overlap_goal(false)
{

}

//用于为存储结构放入单词
void word::setWordsTxt(const QString &fileName)
{

    //若该树为空
    if(this->wordsTree==nullptr) this->wordsTree=new TrieTree(fileName);
    //若该树不为空
    else
    {
        delete this->wordsTree;
        wordsTree=new TrieTree(fileName);
    }

}

//初始化游戏页面,随机初始化一个单词
QString word::initial_word(int d_time)
{
      return this->wordsTree->get_a_random_word(d_time);

}

//设置规则
void word::setRule(const bool &is_overlap_goal)
{
    if(is_overlap_goal) this->is_overlap_goal=true;
    else this->is_overlap_goal=false;
}
//词库中是否有该单词，有则返回，没有返回空字符串
QString word::find(const QString &word,bool is_useless_words)
{
    //查询剔除后的单词树
    if(is_useless_words)
    {
        if(this->UselesswordsTree->find_string(word,false))
        {
            return word;
        }
        else return "";
    }
    else
    {
        if(this->wordsTree->find_string(word))
        {
            return word;
        }
        else return "";
    }

}

//得到两个接龙单词最大重叠字符串(暴力搜索，可改用KMP)
QString word::get_prefix(const QString &pre_word,const QString & connect_word)
{
    int pre_length=pre_word.length();
    int con_lenght=connect_word.length();
    int min_length=pre_length<con_lenght?pre_length:con_lenght;
    int temp_str_num;
    int max_str_num=0;

    for(int i=0;i<min_length;++i)
    {
        temp_str_num=0;
        for(int j=i;j<min_length;++j)
        {
            //暴力匹配
            if(pre_word[pre_length-min_length+j]!=connect_word[j-i])
            {
                break;
            }
            ++temp_str_num;
        }
        if(temp_str_num>max_str_num) max_str_num=temp_str_num;

    }
    return connect_word.mid(0,max_str_num);
}

//返回前num个接龙单词和对应的分数
std::shared_ptr<wordsWithGoal[]> word::words_matching(QString prefix_words,int num)
{

    QVector<QString> allPrifixWords;
    //所有符合规则的单词
    QString allConnectWords;
    if(!is_overlap_goal)allConnectWords=this->wordsTree->getPrefixWords(prefix_words[prefix_words.length()-1]);
    else
    {
        int pre_word_length=prefix_words.length();
        for(int i=1;i<pre_word_length;++i)
        {
            allConnectWords+=this->wordsTree->getPrefixWords(prefix_words.mid(i,pre_word_length-1));
        }
    }
    //得到所有符合条件的单词
    QString temp_string;
    for(QChar ch:allConnectWords)
    {
        if(ch=='#')
        {
            allPrifixWords.push_back(temp_string);
            temp_string="";

            continue;
        }
        else temp_string.push_back(ch);

    }

    //将所有公共前缀单词结构体存入自己的数组中
    int all_words_number=allPrifixWords.length();//全部符合规则的单词
    wordsWithGoal *connectWords= new wordsWithGoal[all_words_number];

    for(int i=0;i<all_words_number;++i)
    {
        connectWords[i].words=allPrifixWords[i];
        //按不同规则计算得分
        connectWords[i].goal=this->get_goals(prefix_words,allPrifixWords[i]);
        connectWords[i].next_words_num=this->get_next_words_num(allPrifixWords[i]);
        connectWords[i].is_depend_less_word=this->is_depend_less_words;
    }

    //排序
    quick_sort(connectWords,0,all_words_number-1);

    //存入要返回的数组中（智能指针）
    std::shared_ptr<wordsWithGoal[]> resule_words(new wordsWithGoal[num]);
    if(!is_depend_less_words)//按得分由高到低
    {
        num=num<all_words_number?num:all_words_number;
        for(int i=0;i<num;++i)
        {
            resule_words[i]=connectWords[i];
        }
    }
    else//按后续单词由少到多
    {
        if(num<all_words_number)
        {
            for(int i=all_words_number;i>all_words_number-num;--i)
            {
                resule_words[all_words_number-i]=connectWords[i-1];
            }
        }
        else
        {
            for(int i=all_words_number;i>0;--i)
            {
                resule_words[all_words_number-i]=connectWords[i-1];
            }
        }

    }

    delete [] connectWords;
    connectWords=nullptr;
    return resule_words;

}

//计算一个接龙单词的得分
int word::get_goals(QString prefix_word,QString word)
{

    //若是规则一
    if(!this->is_overlap_goal)
    {
        if(prefix_word[prefix_word.length()-1]==word[0])return word.length();
        else return 0;

    }

    else
    {
        return  this->get_prefix(prefix_word,word).length()*word.length();
    }
}


//快速排序 不稳定
void word::quick_sort(wordsWithGoal *&a,int l,int r)
{

    if (l >= r) return;
       int t1 = l, t2 = r;
       wordsWithGoal key = a[(l + r) / 2];
       do {
           while (a[t1] > key) t1++;
           while (a[t2] < key) t2--;
           if (t1 <= t2)
           {
               wordsWithGoal temp=a[t1];
               a[t1]=a[t2];
               a[t2]=temp;
               t1++,t2--;

           }
       } while (t1 <= t2);
       quick_sort(a,l, t2);
       quick_sort(a,t1, r);
}

int word::get_next_words_num(QString word)
{
    int num=0;
    if(!this->is_overlap_goal)
    {
        QString s=this->wordsTree->getPrefixWords(word[word.length()-1]);
        for(QChar c:s)
        {
            if(c=='#')++num;
        }
    }
    else
    {
        int pre_word_length=word.length();
        QString s;
        for(int i=1;i<pre_word_length;++i)
        {
            s+=this->wordsTree->getPrefixWords(word.mid(i,pre_word_length-i));
        }
        for(QChar c:s)
        {
            if(c=='#')++num;
        }
    }
    return num;
}
void word::setOrderRule(const bool &is_depend_less_words)
{
    this->is_depend_less_words=is_depend_less_words;
}



//int main()
//{
//    QString fileName=":/words/CET4.txt";
//    word w;
//    w.setWordsTxt(fileName);
//    w.setRule(true);
//    qDebug()<<w.find("ornament");
//    int num=10;
//    QString prefix="ornament";
//    std::shared_ptr<wordsWithGoal[]> s=w.words_matching(prefix,num);
//    for(int i=0;i<num;++i)
//    {
//        qDebug()<<s.get()[i].words<<'\t'<<s.get()[i].goal<<'\n';
//    }

//    w.setOrderRule(true);
//    std::shared_ptr<wordsWithGoal[]> s_2=w.words_matching("apple",num);
//    for(int i=0;i<num;++i)
//    {
//        qDebug()<<s_2.get()[i].words<<'\t'<<s_2.get()[i].next_words_num<<'\n';
//    }

//    qDebug()<<w.get_prefix("abce","ell")<<'\t';
//    qDebug()<<w.get_prefix("abce","cell");

//    qDebug()<<"----------------------------------------------"<<'\t';
//    qDebug()<<w.find("abandon",false)<<'\t';

//    qDebug()<<w.initial_word()<<"\n";
//    qDebug()<<w.get_prefix("and","dwarxz");
//}
