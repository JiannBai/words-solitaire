#include "startmainwindow.h"
#include "ui_startmainwindow.h"
#include"word.h"
#include<QFile>
#include<QString>
#include<QFileDevice>
#include<QVector>

//构造函数
StartMainWindow::StartMainWindow(QWidget *parent) :QMainWindow(parent),
    ui(new Ui::StartMainWindow),is_first_click(true),show_word_num(20),
    goals(0),word_num(0)
{

    ui->setupUi(this);
    //设置窗口名称
    setWindowTitle("游戏页面");
    //连接信号游戏开始信号与读取单词文件的槽函数
    connect(this,&StartMainWindow::startGame,this,&StartMainWindow::readWordsFile);
    //设置为十进制
    ui->goals->setDecMode();
    //默认四级词库
    this->word_control.setWordsTxt(":/words/CET4.txt");
    //初始化第一个接龙单词(按钮）
   QString firstName="word_";
   QString secondName[20]={"1","2","3","4","5","6","7","8","9","10",
                          "11","12","13","14","15","16","17","18","19","20"};
   if(ui->NowWord->text()=="NowWord")//刚开始游戏
   {
       int d_time[20]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};
       for(int i=0;i<20;++i)
       {
           ui->gridWidget->findChild<QPushButton*>(firstName+secondName[i])->setText(word_control.initial_word(d_time[i]));
       }
   }
   else//游戏中途
   {

   }
}

//析构函数
StartMainWindow::~StartMainWindow()
{
    delete ui;
}

//返回主菜单
void StartMainWindow::on_pushButton_clicked()//单击返回按钮
{
    emit this->backToMainWindow();//返回主菜单信号
}

//输入单词
void StartMainWindow::on_pushButton_inputWord_clicked()
{
    QString text=ui->inputWords->toPlainText();
    //判断是否符合规则，计算得分
    if(word_control.get_goals(ui->NowWord->text(),text)!=0)//添加在词库中的限制
    {
        if(!this->word_control.wordsTree->find_string(text,false) && this->word_control.find(text,false)=="")
        {
            ui->tipBox->setText("该单词不在词库中，无法辨别真伪!");
            return;
        }
        else if(text==this->word_control.find(text))//修改
        {
            ui->tipBox->setText("重复单词,输入无效！");
            return;
        }
        //重新计算得分
        this->goals+=this->word_control.get_goals(ui->NowWord->text(),text);
        ui->goals->display(QString::number(goals));//显示得分
        this->ui->NowWord->setText(text);//更换显示目前接龙单词的label
        ++this->word_num;
        ui->wordNum->display(word_num);
        get_next_words();//刷新单词按钮和接龙单词数目按钮信息，剔除接龙单词
        if(!this->is_first_click)
        {
            ui->historyWords->insertPlainText("->"+text);
        }
        else
        {
            ui->historyWords->insertPlainText(text);
            this->is_first_click=false;
        }
    }
    else
    {
        //加个文本提示“接龙单词不符合规则”
        ui->tipBox->setText("不符合规则单词,无效输入");
        return;
    }

    return;
}

//选择单词文本
void StartMainWindow::on_choiceWordsText_currentIndexChanged(int index)
{
    if(ui->choiceWordsText->currentIndex()==0)//第一个文件
    {
        word_control.setWordsTxt(":/words/words_1.txt");
        //初始化第一个单词，并且将该单词的接龙单词显示在按钮上
    }
    else if(ui->choiceWordsText->currentIndex()==1)//第二个文件
    {
        word_control.setWordsTxt(":/words/CET6.txt");
    }
    else if(ui->choiceWordsText->currentIndex()==2)//第三个文件
    {
        word_control.setWordsTxt(":/words/CET4.txt");
    }

    //初始化第一个接龙单词(按钮）
   QString firstName="word_";
   QString secondName[20]={"1","2","3","4","5","6","7","8","9","10",
                          "11","12","13","14","15","16","17","18","19","20"};
   if(ui->NowWord->text()=="NowWord")//刚开始游戏
   {
       int d_time[20]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};
       for(int i=0;i<20;++i)
       {
           ui->gridWidget->findChild<QPushButton*>(firstName+secondName[i])->setText(word_control.initial_word(d_time[i]));
       }
   }
   else//游戏中途
   {

   }


}

//选择规则
void StartMainWindow::on_comboBox_currentIndexChanged(int index)
{
    if(ui->comboBox->currentIndex()==0)//第一规则
    {        
        word_control.setRule(false);
        //初始化第一个单词，并且将该单词的接龙单词显示在按钮上
    }
    //第二个规则
    else if(ui->comboBox->currentIndex()==1)
    {        
        word_control.setRule(true);
    }

}

//排序规则
void StartMainWindow::on_comboBox_2_currentIndexChanged(int index)
{
    if(ui->comboBox_2->currentIndex()==0)
    {
        this->word_control.setOrderRule(false);
    }
    else if(ui->comboBox_2->currentIndex()==1)
    {
        this->word_control.setOrderRule(true);
    }
}


//更换20个按钮显示的单词,将接龙单词存入另一棵树，刷新提示文本框，
void StartMainWindow::get_next_words()
{
    //将上一个接龙的单词插入另一颗树当中
    word_control.UselesswordsTree->insert(ui->NowWord->text());
    //从当前树中找到并删除接龙单词
    this->word_control.wordsTree->find_string(ui->NowWord->text(),true);
    //显示当前接龙单词数目
    ui->nextWordNum->display(QString::number(word_control.get_next_words_num(ui->NowWord->text())));
    //符合条件的show_word_num个单词
    std::shared_ptr<wordsWithGoal[]> wordWithGoalList(word_control.words_matching(ui->NowWord->text(),show_word_num));
    QString firstName="word_";
    QString secondName[20]={"1","2","3","4","5","6","7","8","9","10",
                           "11","12","13","14","15","16","17","18","19","20"};
    wordsWithGoal temp_WG;
    QPushButton* PushButt_ptr;
    for(int i=0;i<show_word_num;++i)
    {
        temp_WG=wordWithGoalList[i];
        if(temp_WG.words!="")
        {
            PushButt_ptr=ui->gridWidget->findChild<QPushButton*>(firstName+secondName[i]);
            //判断按钮是否隐藏
            if(!PushButt_ptr->isHidden())  PushButt_ptr->setText(temp_WG.words);
            else
            {
                PushButt_ptr->show();
                PushButt_ptr->setText(temp_WG.words);
            }

            //加分逻辑
        }
        else//若单词不足,将多余按钮隐藏起来
        {
            for(int j=i;j<show_word_num;++j)
            {
                 ui->gridWidget->findChild<QPushButton*>(firstName+secondName[j])->hide();
            }
        }

    }
    ui->tipBox->setText("");
    ui->inputWords->setText("");

}


//点击按钮，用该单词进行接龙（如果有按钮，那么该按钮的单词一定可以接龙）
void StartMainWindow::on_word_1_clicked()
{

    ui->NowWord->setText(ui->word_1->text());
    QString s=ui->NowWord->text();
    if(!this->is_first_click)
    {
        ui->historyWords->insertPlainText("->"+s);
    }
    else
    {
        ui->historyWords->insertPlainText(s);
        this->is_first_click=false;
    }
    this->get_next_words();
}


void StartMainWindow::on_word_2_clicked()
{
    ui->NowWord->setText(ui->word_2->text());
    if(!this->is_first_click) ui->historyWords->insertPlainText("->"+ui->word_2->text());
    else
    {
        ui->historyWords->insertPlainText(ui->word_2->text());
        this->is_first_click=false;
    }
    this->get_next_words();

}


void StartMainWindow::on_word_3_clicked()
{
    ui->NowWord->setText(ui->word_3->text());
    if(!this->is_first_click) ui->historyWords->insertPlainText("->"+ui->word_3->text());
    else
    {
        ui->historyWords->insertPlainText(ui->word_3->text());
        this->is_first_click=false;
    }
    this->get_next_words();
}


void StartMainWindow::on_word_4_clicked()
{
    ui->NowWord->setText(ui->word_4->text());
    if(!this->is_first_click) ui->historyWords->insertPlainText("->"+ui->word_4->text());
    else
    {
        ui->historyWords->insertPlainText(ui->word_4->text());
        this->is_first_click=false;
    }
    this->get_next_words();
}


void StartMainWindow::on_word_5_clicked()
{
    ui->NowWord->setText(ui->word_5->text());
    if(!this->is_first_click) ui->historyWords->insertPlainText("->"+ui->word_5->text());
    else
    {
        ui->historyWords->insertPlainText(ui->word_5->text());
        this->is_first_click=false;
    }
    emit this->get_next_words();
}


void StartMainWindow::on_word_6_clicked()
{
    ui->NowWord->setText(ui->word_6->text());
    if(!this->is_first_click) ui->historyWords->insertPlainText("->"+ui->word_6->text());
    else
    {
        ui->historyWords->insertPlainText(ui->word_6->text());
        this->is_first_click=false;
    }
    emit this->get_next_words();
}


void StartMainWindow::on_word_7_clicked()
{
    ui->NowWord->setText(ui->word_7->text());
    if(!this->is_first_click) ui->historyWords->insertPlainText("->"+ui->word_7->text());
    else
    {
        ui->historyWords->insertPlainText(ui->word_7->text());
        this->is_first_click=false;
    }
    emit this->get_next_words();
}


void StartMainWindow::on_word_8_clicked()
{
    ui->NowWord->setText(ui->word_8->text());
    if(!this->is_first_click) ui->historyWords->insertPlainText("->"+ui->word_8->text());
    else
    {
        ui->historyWords->insertPlainText(ui->word_8->text());
        this->is_first_click=false;
    }
    emit this->get_next_words();
}


void StartMainWindow::on_word_9_clicked()
{
    ui->NowWord->setText(ui->word_9->text());
    if(!this->is_first_click) ui->historyWords->insertPlainText("->"+ui->word_9->text());
    else
    {
        ui->historyWords->insertPlainText(ui->word_9->text());
        this->is_first_click=false;
    }
    emit this->get_next_words();
}


void StartMainWindow::on_word_10_clicked()
{
    ui->NowWord->setText(ui->word_10->text());
    if(!this->is_first_click) ui->historyWords->insertPlainText("->"+ui->word_10->text());
    else
    {
        ui->historyWords->insertPlainText(ui->word_10->text());
        this->is_first_click=false;
    }
    this->get_next_words();
}


void StartMainWindow::on_word_11_clicked()
{
    ui->NowWord->setText(ui->word_11->text());
    if(!this->is_first_click) ui->historyWords->insertPlainText("->"+ui->word_11->text());
    else
    {
        ui->historyWords->insertPlainText(ui->word_11->text());
        this->is_first_click=false;
    }
    this->get_next_words();
}


void StartMainWindow::on_word_12_clicked()
{
    ui->NowWord->setText(ui->word_12->text());
    if(!this->is_first_click) ui->historyWords->insertPlainText("->"+ui->word_12->text());
    else
    {
        ui->historyWords->insertPlainText(ui->word_12->text());
        this->is_first_click=false;
    }
    this->get_next_words();
}


void StartMainWindow::on_word_13_clicked()
{
    ui->NowWord->setText(ui->word_13->text());
    if(!this->is_first_click) ui->historyWords->insertPlainText("->"+ui->word_13->text());
    else
    {
        ui->historyWords->insertPlainText(ui->word_13->text());
        this->is_first_click=false;
    }
    this->get_next_words();
}



void StartMainWindow::on_word_14_clicked()
{
    ui->NowWord->setText(ui->word_14->text());
    if(!this->is_first_click) ui->historyWords->insertPlainText("->"+ui->word_14->text());
    else
    {
        ui->historyWords->insertPlainText(ui->word_14->text());
        this->is_first_click=false;
    }
    this->get_next_words();
}


void StartMainWindow::on_word_15_clicked()
{
    ui->NowWord->setText(ui->word_15->text());
    if(!this->is_first_click) ui->historyWords->insertPlainText("->"+ui->word_15->text());
    else
    {
        ui->historyWords->insertPlainText(ui->word_15->text());
        this->is_first_click=false;
    }
    this->get_next_words();
}


void StartMainWindow::on_word_16_clicked()
{
    ui->NowWord->setText(ui->word_16->text());
    if(!this->is_first_click) ui->historyWords->insertPlainText("->"+ui->word_16->text());
    else
    {
        ui->historyWords->insertPlainText(ui->word_16->text());
        this->is_first_click=false;
    }
    this->get_next_words();
}


void StartMainWindow::on_word_17_clicked()
{
    ui->NowWord->setText(ui->word_17->text());
    if(!this->is_first_click) ui->historyWords->insertPlainText("->"+ui->word_17->text());
    else
    {
        ui->historyWords->insertPlainText(ui->word_17->text());
        this->is_first_click=false;
    }
    this->get_next_words();
}


void StartMainWindow::on_word_18_clicked()
{
    ui->NowWord->setText(ui->word_18->text());
    if(!this->is_first_click) ui->historyWords->insertPlainText("->"+ui->word_18->text());
    else
    {
        ui->historyWords->insertPlainText(ui->word_18->text());
        this->is_first_click=false;
    }
    this->get_next_words();
}


void StartMainWindow::on_word_19_clicked()
{
    ui->NowWord->setText(ui->word_19->text());
    if(!this->is_first_click) ui->historyWords->insertPlainText("->"+ui->word_19->text());
    else
    {
        ui->historyWords->insertPlainText(ui->word_19->text());
        this->is_first_click=false;
    }
    this->get_next_words();
}


void StartMainWindow::on_word_20_clicked()
{
    ui->NowWord->setText(ui->word_20->text());
    if(!this->is_first_click) ui->historyWords->insertPlainText("->"+ui->word_20->text());
    else
    {
        ui->historyWords->insertPlainText(ui->word_20->text());
        this->is_first_click=false;
    }
    this->get_next_words();
}


//无用槽函数
void StartMainWindow::on_word_19_clicked(bool checked)
{
    return ;
}

void StartMainWindow::readWordsFile()
{

    return;
}




void StartMainWindow::on_inputWords_textChanged()
{


}


//将接过龙的单词存入一个字典树中
void StartMainWindow::on_NowWord_windowIconTextChanged(const QString &iconText)
{
    this->word_control.UselesswordsTree->insert(ui->NowWord->text());
}


